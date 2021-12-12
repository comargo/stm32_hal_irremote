/*
 * decoders.h
 *
 *  Created on: Dec 9, 2021
 *      Author: cyril
 */

#ifndef DECODERS_H_
#define DECODERS_H_

#include <stm32_hal_irremote.h>

int MATCH(int measured, int desired);
int MATCH_MARK(int measured_ticks, int desired_us);
int MATCH_SPACE(int measured_ticks, int desired_us);

///////////////////////////////////////////////////////////////////////////////////////////////
uint8_t decodeHash(struct decode_results_t *results);
#if DECODE_RC5
	uint8_t decodeRC5(struct decode_results_t *results);
#endif

#if DECODE_RC6
	uint8_t decodeRC6(struct decode_results_t *results);
#endif
//......................................................................
#if DECODE_NEC
	uint8_t decodeNEC(struct decode_results_t *results);
#endif
//......................................................................
#if DECODE_SONY
	uint8_t decodeSony(struct decode_results_t *results);
#endif
//......................................................................
#if DECODE_PANASONIC
	uint8_t decodePanasonic(struct decode_results_t *results);
#endif
//......................................................................
#if DECODE_JVC
	uint8_t decodeJVC(struct decode_results_t *results);
#endif
//......................................................................
#if DECODE_SAMSUNG
	uint8_t decodeSAMSUNG(struct decode_results_t *results);
#endif
//......................................................................
#if DECODE_WHYNTER
	uint8_t decodeWhynter(struct decode_results_t *results);
#endif
//......................................................................
#if DECODE_AIWA_RC_T501
	uint8_t decodeAiwaRCT501(struct decode_results_t *results);
#endif
//......................................................................
#if DECODE_LG
	uint8_t  decodeLG(struct decode_results_t *results);
#endif
//......................................................................
#if DECODE_SANYO
	uint8_t decodeSanyo(struct decode_results_t *results);
#endif
//......................................................................
#if DECODE_MITSUBISHI
	uint8_t decodeMitsubishi(struct decode_results_t *results);
#endif
//......................................................................
#if DECODE_DISH
	uint8_t decodeDish(struct decode_results_t *results); // NOT WRITTEN
#endif
//......................................................................
#if DECODE_SHARP
	uint8_t decodeSharp(struct decode_results_t *results); // NOT WRITTEN
#endif
//......................................................................
#if DECODE_DENON
	uint8_t decodeDenon(struct decode_results_t *results);
#endif


#endif /* DECODERS_H_ */
