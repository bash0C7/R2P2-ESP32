// main/btstack_smoke.c — Phase 0 BLE peripheral smoke
//
// Verifies BTstack vendored ESP32 port runs on ESP-IDF v5.4 + ESP32-S3.
// Spawns a dedicated FreeRTOS task that initializes BTstack and advertises
// a minimal GATT counter service named "StackChan-bts".
//
// On success, the device is visible from Chrome chrome://bluetooth-internals
// and from Mac CoreBluetooth scan.
//
// Remove or gate this file once Phase 1 ships picoruby-ble support.

#include "btstack_smoke.h"
#include "btstack.h"
#include "btstack_port_esp32.h"
#include "btstack_run_loop_freertos.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "btstack_smoke_gatt.h"

static const char *TAG = "btstack-smoke";

static btstack_packet_callback_registration_t hci_event_callback_registration;
static uint8_t counter_value = 0;
static btstack_timer_source_t counter_timer;

static const uint8_t adv_data[] = {
    // Length, Type, Value
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, 0x06,                              // LE General Discoverable, BR/EDR not supported
    0x0E, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME,
    'S','t','a','c','k','C','h','a','n','-','b','t','s'
};

static uint16_t att_read_callback(hci_con_handle_t connection_handle,
                                  uint16_t att_handle,
                                  uint16_t offset,
                                  uint8_t *buffer,
                                  uint16_t buffer_size) {
    (void)connection_handle;
    if (att_handle == ATT_CHARACTERISTIC_F00DBABE_1234_5678_1234_56789ABCDEF1_01_VALUE_HANDLE) {
        return att_read_callback_handle_byte(counter_value, offset, buffer, buffer_size);
    }
    return 0;
}

static void packet_handler(uint8_t packet_type,
                           uint16_t channel,
                           uint8_t *packet,
                           uint16_t size) {
    (void)channel; (void)size;
    if (packet_type != HCI_EVENT_PACKET) return;
    switch (hci_event_packet_get_type(packet)) {
        case BTSTACK_EVENT_STATE: {
            if (btstack_event_state_get_state(packet) == HCI_STATE_WORKING) {
                bd_addr_t addr;
                gap_local_bd_addr(addr);
                ESP_LOGI(TAG, "BTstack up. BD_ADDR=%02x:%02x:%02x:%02x:%02x:%02x",
                         addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
            }
            break;
        }
        case HCI_EVENT_LE_META: {
            if (hci_event_le_meta_get_subevent_code(packet) == HCI_SUBEVENT_LE_CONNECTION_COMPLETE) {
                ESP_LOGI(TAG, "BLE connected");
            }
            break;
        }
        case HCI_EVENT_DISCONNECTION_COMPLETE: {
            ESP_LOGI(TAG, "BLE disconnected, advertising again");
            break;
        }
        default:
            break;
    }
}

static void counter_handler(struct btstack_timer_source *ts) {
    counter_value++;
    btstack_run_loop_set_timer(ts, 1000);
    btstack_run_loop_add_timer(ts);
}

static void btstack_main(void) {
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    l2cap_init();
    sm_init();
    att_server_init(profile_data, &att_read_callback, NULL);
    att_server_register_packet_handler(&packet_handler);

    bd_addr_t null_addr = {0};
    gap_advertisements_set_params(0x0030, 0x0030, 0, 0, null_addr, 0x07, 0x00);
    gap_advertisements_set_data(sizeof(adv_data), (uint8_t *)adv_data);
    gap_advertisements_enable(1);

    counter_timer.process = &counter_handler;
    btstack_run_loop_set_timer(&counter_timer, 1000);
    btstack_run_loop_add_timer(&counter_timer);

    hci_power_control(HCI_POWER_ON);
}

static void btstack_task(void *param) {
    (void)param;
    ESP_LOGI(TAG, "btstack_task starting");
    btstack_init();      // includes esp_bt_controller_init/enable + VHCI bridge
    btstack_main();
    btstack_run_loop_execute();   // blocks forever
    vTaskDelete(NULL);   // unreachable
}

void btstack_smoke_start(void) {
    static bool started = false;
    if (started) return;
    started = true;
    xTaskCreate(btstack_task, "btstack", 8192, NULL, 5, NULL);
}
