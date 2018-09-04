#include <project_config.hpp>

#include <cstdint>
#include <cstdio>

#include "L0_LowLevel/LPC40xx.h"
#include "L0_LowLevel/delay.hpp"
#include "L2_Utilities/debug_print.hpp"
#include "L1_Drivers/pin_configure.hpp"
#include "L1_Drivers/gpio.hpp"

Gpio cs(0, 30), dc(0, 29), reset(0, 16);
Gpio cs_mirror(2, 9), dc_mirror(2, 8);
Gpio mosi1(0, 9);

constexpr uint8_t kSspFunction = 0b010;
// PinConfigure mosi0(0,18), sck0(0, 15);
PinConfigure sck1(0, 7);

uint16_t Ssp0Transaction(uint16_t data)
{
    LPC_SSP1->DR = data;
    while (LPC_SSP1->SR & (0b1 << 4))
    {
        continue;
    }
    return static_cast<uint16_t>(LPC_SSP1->DR);
}

void TransmitByte(uint16_t data)
{
    dc.SetLow();
    dc_mirror.SetLow();
    cs.SetLow();
    cs_mirror.SetLow();
    Ssp0Transaction(data);
    cs.SetHigh();
    cs_mirror.SetHigh();
    Delay(1);
};

void DataSend(uint16_t data)
{
    dc.SetHigh();
    dc_mirror.SetHigh();
    cs.SetLow();
    cs_mirror.SetLow();
    Ssp0Transaction(data);
    cs.SetHigh();
    cs_mirror.SetHigh();
    Delay(1);
};

int main(void)
{
    DEBUG_PRINT("Initializing OLED Hardware Test...");
    cs.SetAsOutput();
    cs_mirror.SetAsOutput();
    dc.SetAsOutput();
    dc_mirror.SetAsOutput();
    reset.SetAsOutput();

    cs.SetHigh();
    cs_mirror.SetHigh();
    dc.SetHigh();
    dc_mirror.SetHigh();
    reset.SetHigh();

    DEBUG_PRINT("Initializing cs and dc and reset GPIOs...");

    // mosi0.SetPinFunction(kSspFunction);
    // sck0.SetPinFunction(kSspFunction);
    mosi1.SetPinFunction(kSspFunction);
    // mosi1.SetAsOutput();
    // mosi1.SetPinMode(PinConfigureInterface::kPullDown);
    // mosi1.SetLow();
    sck1.SetPinFunction(kSspFunction);
    DEBUG_PRINT("Initializing mosi and sck pins as spi function...");

    // // Power on SSP0
    // LPC_SC->PCONP |= 1 << 21;
    // DEBUG_PRINT("SSP0 powered on...");
    // // Set the bitrate to 8
    // LPC_SSP0->CR0 = (LPC_SSP0->CR0 & 0xF) | 0b0111;
    // // Set prescalar
    // LPC_SSP0->CPSR = 254;
    // // Enable SSP0
    // LPC_SSP0->CR1 |= 1 << 1;

    // Power on SSP1
    LPC_SC->PCONP |= 1 << 10;
    DEBUG_PRINT("SSP1 powered on...");
    // Set the bitrate to 8
    LPC_SSP1->CR0 = (LPC_SSP1->CR0 & 0xF) | 0b0111;
    // Set prescalar
    LPC_SSP1->CPSR = 254;
    // Enable SSP1
    LPC_SSP1->CR1 |= 1 << 1;

    DEBUG_PRINT("SSP1 enabled and configured...");

	// Delay(2000);
	// reset.SetLow();
	// Delay(2000);
   	// reset.SetHigh();
	// Delay(2000);

 	TransmitByte(0xae);//--turn off oled panel

	TransmitByte(0xd5);//--set display clock divide ratio/oscillator frequency
	TransmitByte(0x80);//--set divide ratio

	TransmitByte(0xa8);//--set multiplex ratio(1 to 64)
	TransmitByte(0x3f);//--1/64 duty

	TransmitByte(0xd3);//-set display offset
	TransmitByte(0x00);//-not offset


	TransmitByte(0x8d);//--set Charge Pump enable/disable
	TransmitByte(0x14);//--set(0x10) disable


	TransmitByte(0x40);//--set start line address

	TransmitByte(0xa6);//--set normal display

	TransmitByte(0xa4);//Disable Entire Display On

	TransmitByte(0xa1);//--set segment re-map 128 to 0

	TransmitByte(0xC8);//--Set COM Output Scan Direction 64 to 0

	TransmitByte(0xda);//--set com pins hardware configuration
	TransmitByte(0x12);

	TransmitByte(0x81);//--set contrast control register
	TransmitByte(0xCF);


	TransmitByte(0xd9);//--set pre-charge period
	TransmitByte(0xf1);

	TransmitByte(0xdb);//--set vcomh
	TransmitByte(0x40);

	TransmitByte(0xaf);//--turn on oled panel

    // // Set Display Off
    // TransmitByte(0xAE);
    // // Set Display clock divide
    // TransmitHalfword(0xD5, 0x80);
    // // Set multiplex ratio
    // TransmitHalfword(0xA8, 0x3F);
    // // Set Display Offset
    // TransmitHalfword(0xD3, 0x00);
    // // Set Display Offset
    // TransmitByte(0x40);
    // // Send Charge pump setting -> Enable charge pump
    // TransmitHalfword(0x8D, 0x14);
    // // Set Segment Re-map
    // TransmitByte(0xA1);
    // // Set COM Output Scan direction
    // TransmitByte(0xC8);
    // // Set COM pins hardware config
    // TransmitHalfword(0xDA, 0x12);
    // // Set Set Constrast Control
    // TransmitHalfword(0x81, 0xCF);
    // // Set Pre-Charge period
    // TransmitHalfword(0xD9, 0xf1);
    // // Set VCOMH deselect level
    // TransmitHalfword(0xDB, 0x40);
    // // Set Display On
    // TransmitByte(0xA4);
    // // Invert Display (blanking)
    // TransmitByte(0xA6);
    // Delay(500);
    // // Back to normal
    // TransmitByte(0xAF);
    // DEBUG_PRINT("Updating charge pump setting to enable charge pump...");
    DEBUG_PRINT("Turning display on...");
    Delay(1000);
    // for(int i = 0; i < 100; i++)
    // {
    //     DataSend(0xFF);
    // }
    while (1)
    {
        // Set Invert display colors to see a white screen!!
        TransmitByte(0xA7);
        DEBUG_PRINT("Inverting screen...");
        Delay(1000);
        // Set display NORMAL colors to see a black screen!!
        TransmitByte(0xA6);
        DEBUG_PRINT("Screen normalized...");
        Delay(1000);
    }
    return 0;
}
