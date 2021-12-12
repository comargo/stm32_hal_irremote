/*
 * decoders.c
 *
 *  Created on: Dec 9, 2021
 *      Author: cyril
 */


#include "decoders.h"

// Upper and Lower percentage tolerances in measurements
#define TOLERANCE 25
#define LTOL (1.0 - (TOLERANCE / 100.))
#define UTOL (1.0 + (TOLERANCE / 100.))

#define TICKS_LOW(us)   ((int)(((us)*LTOL / IR_USECPERTICK)))
#define TICKS_HIGH(us)  ((int)(((us)*UTOL / IR_USECPERTICK + 1)))

int MATCH(int measured, int desired)
{
 	return ((measured >= TICKS_LOW(desired)) && (measured <= TICKS_HIGH(desired)));
}

int MATCH_MARK(int measured_ticks, int desired_us)
{
	return ((measured_ticks >= TICKS_LOW(desired_us + IR_MARK_EXCESS)) && (measured_ticks <= TICKS_HIGH(desired_us + IR_MARK_EXCESS)));
}

int MATCH_SPACE(int measured_ticks, int desired_us)
{
	return ((measured_ticks >= TICKS_LOW(desired_us - IR_MARK_EXCESS)) && (measured_ticks <= TICKS_HIGH(desired_us - IR_MARK_EXCESS)));
}

#define FNV_PRIME_32 16777619
#define FNV_BASIS_32 2166136261

static int compare(unsigned int oldval, unsigned int newval)
{
	if(newval < oldval * .8) return 0;
	else if(oldval < newval * .8) return 2;
	else return 1;
}

uint8_t decodeHash(struct decode_results_t *results)
{
	int32_t hash = FNV_BASIS_32;

	// Require at least 6 samples to prevent triggering on noise
	if(results->rawlen < 6) return false;

	for(int16_t i = 1; (i + 2) < results->rawlen; i++)
	{
		int16_t value = compare(results->rawbuf[i], results->rawbuf[i+2]);
		hash = (hash * FNV_PRIME_32) ^ value; // Add value into the hash
	}

	results->value = hash;
	results->bits = 32;
	results->decode_type = UNKNOWN;
	return true;
}
