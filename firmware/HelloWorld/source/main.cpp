#include <project_config.hpp>

#include <cstdint>
#include <cstdio>

#include "L0_LowLevel/delay.hpp"
#include "L3_HAL/onboard_led.hpp"
#include "L2_Utilities/debug_print.hpp"

int main(void)
{
    DEBUG_PRINT("Staring Hello World Application");
    DEBUG_PRINT("Initializing LEDs...");
    OnBoardLed leds;
    leds.Initialize();
    DEBUG_PRINT("LEDs Initialized!");
    while (1)
    {
        for(uint8_t i = 0; i < 15; i++)
        {
            printf("Hello World %u\n", i);
            leds.SetAll(i);
        }
        Delay(500);
    }
    return 0;
}
