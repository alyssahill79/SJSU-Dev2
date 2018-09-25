#pragma once

#include "L0_LowLevel/LPC40xx.h"
#include "L0_LowLevel/interrupt.hpp"
#include "L1_Drivers/system_clock.hpp"
#include "L1_Drivers/system_timer.hpp"

extern SystemTimer system_timer;
extern SystemClock clock;

extern const IsrPointer kInterruptVectorTable[];
extern IsrPointer dynamic_isr_vector_table[];

extern "C" void xPortPendSVHandler(void);  // NOLINT
extern "C" void vPortSVCHandler(void);  // NOLINT
extern "C" void xPortSysTickHandler(void);  // NOLINT
