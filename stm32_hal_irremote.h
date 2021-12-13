/*
 * stm32_hal_irremote.h
 *
 *  Created on: Dec 8, 2021
 *      Author: cyril
 */

#ifndef STM32_HAL_IRREMOTE_H_
#define STM32_HAL_IRREMOTE_H_

#include <stdbool.h>

#define HAL_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#include <stm32f1xx_hal.h>

#include "stm32_hal_irremote_conf.h"

#define IR_REPEAT 0xFFFFFFFF // Decoded value for NEC when a repeat code is received

enum IRREMOTE_STATE {
  IRREMOTE_READY,
  IRREMOTE_CAPTURE,
  IRREMOTE_GAP,
  IRREMOTE_DONE
};

struct CM_HAL_IRREMOTE {
  GPIO_TypeDef *GPIOx;
  uint32_t GPIO_Pin;
  /* Callback */
  void (*callback)(struct CM_HAL_IRREMOTE *irremote, void *userData);
  void *userData;

	TIM_HandleTypeDef htim;
	DMA_HandleTypeDef dma_width;
	DMA_HandleTypeDef dma_pulse;


	enum IRREMOTE_STATE rcvstate; // State Machine state
	uint16_t rawbuf[IRREMOTE_RAWBUF]; // raw data
	uint16_t data_width[IRREMOTE_RAWBUF/2];
	uint16_t data_pulse[IRREMOTE_RAWBUF/2];
	uint8_t data_width_len;
	uint8_t data_pulse_len;
	uint8_t rawlen; // counter of entries in rawbuf
	uint8_t overflow; // Raw buffer overflow occurred
};

enum decode_type_t
{
		UNUSED = -1,
		UNKNOWN = 0,
		RC5,
		RC6,
		NEC,
		SONY,
		PANASONIC,
		JVC,
		SAMSUNG,
		WHYNTER,
		AIWA_RC_T501,
		LG,
		SANYO,
		MITSUBISHI,
		DISH,
		SHARP,
		DENON,
		PRONTO,
};

// Results returned from the decoder
struct decode_results_t
{
		enum decode_type_t decode_type; // UNKNOWN, NEC, SONY, RC5, ...
		uint16_t address; // Used by Panasonic & Sharp [16-bits], Used by Nec
		uint16_t command;
		uint32_t value; // Decoded value [max 32-bits]
		int16_t bits; // Number of bits in decoded value
		volatile uint16_t *rawbuf; // Raw intervals in 50uS ticks
		int16_t rawlen; // Number of records in rawbuf
		int16_t overflow; // true if IR raw code too long
};

void CM_HAL_IRREMOTE_Init(struct CM_HAL_IRREMOTE *irremote, TIM_TypeDef* tim);

void CM_HAL_IRREMOTE_Start_DMA(struct CM_HAL_IRREMOTE *irremote);

void CM_HAL_IRREMOTE_Start_IT(struct CM_HAL_IRREMOTE *irremote);

void CM_HAL_IRREMOTE_Stop_DMA(struct CM_HAL_IRREMOTE *irremote);

void CM_HAL_IRREMOTE_Stop_IT(struct CM_HAL_IRREMOTE *irremote);

void CM_HAL_IRREMOTE_IRQHandler(struct CM_HAL_IRREMOTE* irremote, IRQn_Type irq);

bool CM_HAL_IRREMOTE_Decode(struct CM_HAL_IRREMOTE *irremote, struct decode_results_t *results);

#endif /* STM32_HAL_IRREMOTE_H_ */
