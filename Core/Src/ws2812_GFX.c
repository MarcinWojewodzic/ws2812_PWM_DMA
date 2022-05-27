/*
 * ws2812_GFX.c
 *
 *  Created on: Feb 10, 2022
 *      Author: Marcin
 */

#include"main.h"
#include"ws2812_GFX.h"
#include"ws2812.h"
#include"stdlib.h"
void ws2812_rainbow(uint32_t first_hue, int8_t reps, uint8_t saturation, uint8_t brightness)
{
	for (uint16_t i = 0; i < WS2812_LEDS; i++)
	{
		uint16_t hue = first_hue + (i * reps * 65536) / WS2812_LEDS;
		uint32_t color = ColorHSV(hue, saturation, brightness);
		color=gamma32(color);
		ws2812_SetPixes32(i, color);
	}
}
void ws2812_randomHSV()
{
	for(int i=0;i<WS2812_LEDS;i++)
	{
		WS2812B_SetDiodeHSV(i, rand()%360, 255, 50);
	}
}
void ws2812_randomRGB()
{
	for(int i=0;i<WS2812_LEDS;i++)
	{
		ws2812_set_pixel(i, gamma8(rand()%256), gamma8(rand()%256), gamma8(rand()%256));
	}

}
void ws2812_stroboskopRGB(uint8_t s,uint8_t r,uint8_t g, uint8_t b)
{
	switch(s%2)
	{
	case 0:
	{
		ws2812_clear();
		for(int i=0;i<WS2812_LEDS;i+=2)
		{
			ws2812_set_pixel(i, r, g, b);
		}
		break;
	}
	case 1:
	{
		ws2812_clear();
		for(int i=1;i<WS2812_LEDS;i+=2)
		{
			ws2812_set_pixel(i, r, g, b);
		}
		break;
	}
	}
}
void ws2812_stroboskopHSV(uint8_t sa,uint16_t h,uint8_t s, uint8_t v)
{
	switch(sa%2)
	{
	case 0:
	{
		ws2812_clear();
		for(int i=0;i<WS2812_LEDS;i+=2)
		{
			WS2812B_SetDiodeHSV(i, h, s, v);
		}
		break;
	}
	case 1:
	{
		ws2812_clear();
		for(int i=1;i<WS2812_LEDS;i+=2)
		{
			WS2812B_SetDiodeHSV(i, h, s, v);
		}
		break;
	}
	}
}
