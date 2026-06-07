#include "picoruby-esp32.h"
#include "sdkconfig.h"

#ifdef CONFIG_BTSTACK_SMOKE
#include "btstack_smoke.h"
#endif

void app_main(void)
{
#ifdef CONFIG_BTSTACK_SMOKE
  btstack_smoke_start();
#endif
  picoruby_esp32();
}
