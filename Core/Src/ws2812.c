/*
 * ws2812.c
 *
 *  Created on: Feb 6, 2022
 *      Author: Marcin
 */


#include"main.h"
#include"ws2812.h"
ws2812_rgb ws2812_array[WS2812_LEDS];
uint32_t buffer[48];
uint8_t reset_flag;
uint16_t led=0;
TIM_HandleTypeDef* ws2812_tim;
void ws2812_init(TIM_HandleTypeDef* tim)
{
	ws2812_tim=tim;
}
void ws2812_set_pixel(uint16_t id, uint8_t red, uint8_t green, uint8_t blue)
{
	ws2812_array[id].b=blue;
	ws2812_array[id].r=red;
	ws2812_array[id].g=green;
}
static const uint8_t _sineTable[256] = { 128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170, 173, 176, 179, 182, 185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211, 213, 215,
        218, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240, 241, 243, 244, 245, 246, 248, 249, 250, 250, 251, 252, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 254, 254,
        254, 253, 253, 252, 251, 250, 250, 249, 248, 246, 245, 244, 243, 241, 240, 238, 237, 235, 234, 232, 230, 228, 226, 224, 222, 220, 218, 215, 213, 211, 208, 206, 203, 201, 198, 196, 193, 190,
        188, 185, 182, 179, 176, 173, 170, 167, 165, 162, 158, 155, 152, 149, 146, 143, 140, 137, 134, 131, 128, 124, 121, 118, 115, 112, 109, 106, 103, 100, 97, 93, 90, 88, 85, 82, 79, 76, 73, 70,
        67, 65, 62, 59, 57, 54, 52, 49, 47, 44, 42, 40, 37, 35, 33, 31, 29, 27, 25, 23, 21, 20, 18, 17, 15, 14, 12, 11, 10, 9, 7, 6, 5, 5, 4, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 3,
        4, 5, 5, 6, 7, 9, 10, 11, 12, 14, 15, 17, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35, 37, 40, 42, 44, 47, 49, 52, 54, 57, 59, 62, 65, 67, 70, 73, 76, 79, 82, 85, 88, 90, 93, 97, 100, 103, 106,
        109, 112, 115, 118, 121, 124 };

static const uint8_t _gammaTable[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4,
        4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25, 25,
        26, 27, 27, 28, 29, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40, 41, 42, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 68,
        69, 70, 71, 72, 73, 75, 76, 77, 78, 80, 81, 82, 84, 85, 86, 88, 89, 90, 92, 93, 94, 96, 97, 99, 100, 102, 103, 105, 106, 108, 109, 111, 112, 114, 115, 117, 119, 120, 122, 124, 125, 127, 129,
        130, 132, 134, 136, 137, 139, 141, 143, 145, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 191, 193, 195, 197, 199, 202, 204,
        206, 209, 211, 213, 215, 218, 220, 223, 225, 227, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252, 255 };

uint8_t sine8(uint8_t x)
{
	return _sineTable[x];
}

uint8_t gamma8(uint8_t x)
{
	return _gammaTable[x];
}
uint32_t led_data[256];
void ws2812_display(void)
{
	reset_flag=0;
	led=0;
	for(int i=0;i<48;i++)
	{
		buffer[i]=0x00;
	}
	HAL_TIM_PWM_Start_DMA(ws2812_tim, TIM_CHANNEL_1, buffer, 48);
	while(HAL_DMA_STATE_READY != ws2812_tim->hdma[1]->State){}
}
void ws2812_clear()
{
	for(int i=0;i<WS2812_LEDS;i++)
	{
		ws2812_set_pixel(i, 0, 0, 0);
	}
}
void WS2812B_SetDiodeHSV(int16_t diode_id, uint16_t Hue, uint8_t Saturation, uint8_t Brightness)
{
	if(diode_id >= WS2812_LEDS || diode_id < 0) return;
	uint16_t Sector, Fracts, p, q, t;

	if(Saturation == 0)
	{
		ws2812_array[diode_id].r = Brightness;
		ws2812_array[diode_id].g = Brightness;
		ws2812_array[diode_id].b = Brightness;
	}
	else
	{
		if(Hue >= 360) Hue = 359;

		Sector = Hue / 60; // Sector 0 to 5
		Fracts = Hue % 60;
		p = (Brightness * (255 - Saturation)) / 256;
		q = (Brightness * (255 - (Saturation * Fracts)/60)) / 256;
		t = (Brightness * (255 - (Saturation * (59 - Fracts))/60)) / 256;


		switch(Sector)
		{
		case 0:
			ws2812_array[diode_id].r = Brightness;
			ws2812_array[diode_id].g = (uint8_t)t;
			ws2812_array[diode_id].b = (uint8_t)p;
			break;
		case 1:
			ws2812_array[diode_id].r = (uint8_t)q;
			ws2812_array[diode_id].g = Brightness;
			ws2812_array[diode_id].b = (uint8_t)p;
			break;
		case 2:
			ws2812_array[diode_id].r = (uint8_t)p;
			ws2812_array[diode_id].g = Brightness;
			ws2812_array[diode_id].b = (uint8_t)t;
			break;
		case 3:
			ws2812_array[diode_id].r = (uint8_t)p;
			ws2812_array[diode_id].g = (uint8_t)q;
			ws2812_array[diode_id].b = Brightness;
			break;
		case 4:
			ws2812_array[diode_id].r = (uint8_t)t;
			ws2812_array[diode_id].g = (uint8_t)p;
			ws2812_array[diode_id].b = Brightness;
			break;
		default:		// case 5:
			ws2812_array[diode_id].r = Brightness;
			ws2812_array[diode_id].g = (uint8_t)p;
			ws2812_array[diode_id].b = (uint8_t)q;
			break;
		}
	}
}
uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val)
{

	uint8_t r, g, b;

	hue = (hue * 1530L + 32768) / 65536;

	if (hue < 510)
	{ // Red to Green-1
		b = 0;
		if (hue < 255)
		{ //   Red to Yellow-1
			r = 255;
			g = hue;       //     g = 0 to 254
		}
		else
		{         //   Yellow to Green-1
			r = 510 - hue; //     r = 255 to 1
			g = 255;
		}
	}
	else if (hue < 1020)
	{ // Green to Blue-1
		r = 0;
		if (hue < 765)
		{ //   Green to Cyan-1
			g = 255;
			b = hue - 510;  //     b = 0 to 254
		}
		else
		{          //   Cyan to Blue-1
			g = 1020 - hue; //     g = 255 to 1
			b = 255;
		}
	}
	else if (hue < 1530)
	{ // Blue to Red-1
		g = 0;
		if (hue < 1275)
		{ //   Blue to Magenta-1
			r = hue - 1020; //     r = 0 to 254
			b = 255;
		}
		else
		{ //   Magenta to Red-1
			r = 255;
			b = 1530 - hue; //     b = 255 to 1
		}
	}
	else
	{ // Last 0.5 Red (quicker than % operator)
		r = 255;
		g = b = 0;
	}

	// Apply saturation and value to R,G,B, pack into 32-bit result:
	uint32_t v1 = 1 + val;  // 1 to 256; allows >>8 instead of /255
	uint16_t s1 = 1 + sat;  // 1 to 256; same reason
	uint8_t s2 = 255 - sat; // 255 to 0
	return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) | (((((g * s1) >> 8) + s2) * v1) & 0xff00) | (((((b * s1) >> 8) + s2) * v1) >> 8);
}
uint32_t gamma32(uint32_t x)
{
	uint8_t *y = (uint8_t*) &x;
	for (uint8_t i = 0; i < 4; i++)
		y[i] = gamma8(y[i]);
	return x;
}
void ws2812_SetPixes32(uint16_t id,uint32_t color)
{
	ws2812_array[id].r=(color>>16)&0xff;
	ws2812_array[id].g=(color>>8)&0xff;
	ws2812_array[id].b=(color)&0xff;
}
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM5)
	{
		if (reset_flag!=2)
		{
			for (uint8_t k = 0; k < 24; k++) // To 72 impulses of reset
			{
				buffer[k] = 0x00;
			}
			reset_flag++; // End reset signal
		}

		else if (led > WS2812_LEDS)
		{
			HAL_TIM_PWM_Stop_DMA(ws2812_tim, TIM_CHANNEL_1);
			//HAL_TIM_PWM_Stop(ws2812_tim, TIM_CHANNEL_4);
		}
		else
		{
			// Even LEDs 0,2,0
			uint8_t j = 24;
			//GREEN
			for (int8_t k = 7; k >= 0; k--)
			{
				if ((ws2812_array[led].g & (1 << k)) == 0)
					buffer[j] = zero;
				else
					buffer[j] = one;
				j++;
			}

			//RED
			for (int8_t k = 7; k >= 0; k--)
			{
				if ((ws2812_array[led].r & (1 << k)) == 0)
					buffer[j] = zero;
				else
					buffer[j] = one;
				j++;
			}

			//BLUE
			for (int8_t k = 7; k >= 0; k--)
			{
				if ((ws2812_array[led].b & (1 << k)) == 0)
					buffer[j] = zero;
				else
					buffer[j] = one;
				j++;
			}
			led++;
		}
	}
}
void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM5)
	{
		if (reset_flag!=2)
		{
			for (uint8_t k = 0; k < 24; k++) // To 72 impulses of reset
			{
				buffer[k] = 0x00;
			}
			reset_flag++; // End reset signal
		}
		else // LEDs Odd 1,3,5,7...
		{
			if (led > WS2812_LEDS)
			{

				HAL_TIM_PWM_Stop_DMA(ws2812_tim, TIM_CHANNEL_1);
				//HAL_TIM_PWM_Stop(ws2812_tim, TIM_CHANNEL_4);
			}
			else
			{
				uint8_t j = 0;
				//GREEN
				for (int8_t k = 7; k >= 0; k--)
				{
					if ((ws2812_array[led].g & (1 << k)) == 0)
						buffer[j] = zero;
					else
						buffer[j] = one;
					j++;
				}

				//RED
				for (int8_t k = 7; k >= 0; k--)
				{
					if ((ws2812_array[led].r & (1 << k)) == 0)
						buffer[j] = zero;
					else
						buffer[j] = one;
					j++;
				}

				//BLUE
				for (int8_t k = 7; k >= 0; k--)
				{
					if ((ws2812_array[led].b & (1 << k)) == 0)
						buffer[j] = zero;
					else
						buffer[j] = one;
					j++;
				}
				led++;
			}
		}
	}
}

