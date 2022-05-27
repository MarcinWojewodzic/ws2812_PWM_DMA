/*
 * ws2812.h
 *
 *  Created on: Feb 6, 2022
 *      Author: Marcin
 */

#ifndef INC_WS2812_H_
#define INC_WS2812_H_
#define WS2812_LEDS 12
#define zero 34
#define one 79
typedef struct
{
	uint8_t r,g,b;
}ws2812_rgb;

void ws2812_init(TIM_HandleTypeDef* tim);
void ws2812_set_pixel(uint16_t id, uint8_t red, uint8_t green, uint8_t blue);
void ws2812_display(void);
uint8_t sine8(uint8_t x);
uint8_t gamma8(uint8_t x);
void WS2812B_SetDiodeHSV(int16_t diode_id, uint16_t Hue, uint8_t Saturation, uint8_t Brightness);
uint32_t gamma32(uint32_t x);
void ws2812_SetPixes32(uint16_t id,uint32_t color);
uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val);
void ws2812_clear();
#endif /* INC_WS2812_H_ */
