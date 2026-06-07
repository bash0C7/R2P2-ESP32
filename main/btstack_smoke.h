// main/btstack_smoke.h
#ifndef BTSTACK_SMOKE_H
#define BTSTACK_SMOKE_H

#ifdef __cplusplus
extern "C" {
#endif

// Spawns a FreeRTOS task that runs btstack_init, configures a minimal GATT,
// and starts advertising as "StackChan-bts". Idempotent at the task-creation
// level (creates only on first call). Phase 0 smoke only.
void btstack_smoke_start(void);

#ifdef __cplusplus
}
#endif

#endif // BTSTACK_SMOKE_H
