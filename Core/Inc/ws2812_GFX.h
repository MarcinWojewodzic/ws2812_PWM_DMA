/*
 * ws2812_GFX.h
 *
 *  Created on: Feb 10, 2022
 *      Author: Marcin
 */

#ifndef INC_WS2812_GFX_H_
#define INC_WS2812_GFX_H_

void ws2812_rainbow(uint32_t first_hue, int8_t reps, uint8_t saturation, uint8_t brightness);
void ws2812_randomHSV();
void ws2812_randomRGB();
void ws2812_stroboskopRGB(uint8_t s,uint8_t r,uint8_t g, uint8_t b);
void ws2812_stroboskopHSV(uint8_t sa,uint16_t h,uint8_t s, uint8_t v);
#endif /* INC_WS2812_GFX_H_ */
