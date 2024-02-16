

#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"


void feedTheDog()
{
  // feed dog 0
  TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE; // write enable
  TIMERG0.wdt_feed = 1;                     // feed dog
  TIMERG0.wdt_wprotect = 0;                 // write protect
  // feed dog 1
  TIMERG1.wdt_wprotect = TIMG_WDT_WKEY_VALUE; // write enable
  TIMERG1.wdt_feed = 1;                     // feed dog
  TIMERG1.wdt_wprotect = 0;                 // write protect
}
