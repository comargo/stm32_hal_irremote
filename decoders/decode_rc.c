/*
 * decode_rc.c
 *
 *  Created on: Dec 9, 2021
 *      Author: cyril
 */

#include "decoders.h"

//+=============================================================================
// Gets one undecoded level at a time from the raw buffer.
// The RC5/6 decoding is easier if the data is broken into time intervals.
// E.g. if the buffer has MARK for 2 time intervals and SPACE for 1,
// successive calls to getRClevel will return MARK, MARK, SPACE.
// offset and used are updated to keep track of the current position.
// t1 is the time interval for a single bit in microseconds.
// Returns -1 for error (measured time interval is not a multiple of t1).
//
#if (DECODE_RC5 || DECODE_RC6)
int getRClevel(struct decode_results_t *results, int *offset, int *used, int t1)
{
	int width;
	int val;
	int correction;
	int avail;

	if (*offset >= results->rawlen)
		return IR_DATA_SPACE;  // After end of recorded buffer, assume SPACE.
	width = results->rawbuf[*offset];
	val = ((*offset) % 2) ? IR_DATA_MARK : IR_DATA_SPACE;
	correction = (val == IR_DATA_MARK) ? IR_MARK_EXCESS : - IR_MARK_EXCESS;

	if (MATCH(width, (t1) + correction))
		avail = 1;
	else if (MATCH(width, (2 * t1) + correction))
		avail = 2;
	else if (MATCH(width, (3 * t1) + correction))
		avail = 3;
	else
		return -1;

	(*used)++;
	if (*used >= avail) {
		*used = 0;
		(*offset)++;
	}

	return val;
}
#endif

//==============================================================================
// RRRR    CCCC  55555
// R   R  C      5
// RRRR   C      5555
// R  R   C          5
// R   R   CCCC  5555
//
// NB: First bit must be a one (start bit)
//
#define MIN_RC5_SAMPLES     11
#define RC5_T1             889
#define RC5_RPT_LENGTH   46000

//+=============================================================================
#if DECODE_RC5
uint8_t decodeRC5(struct decode_results_t *results)
{
	int nbits;
	long data = 0;
	int used = 0;
	int offset = 1;  // Skip gap space

	if (results->rawlen < MIN_RC5_SAMPLES + 2)
		return 0;

	// Get start bits
	if (getRClevel(results, &offset, &used, RC5_T1) != IR_DATA_MARK)
		return 0;
	if (getRClevel(results, &offset, &used, RC5_T1) != IR_DATA_SPACE)
		return 0;
	if (getRClevel(results, &offset, &used, RC5_T1) != IR_DATA_MARK)
		return 0;

	for (nbits = 0; offset < results->rawlen; nbits++) {
		int levelA = getRClevel(results, &offset, &used, RC5_T1);
		int levelB = getRClevel(results, &offset, &used, RC5_T1);

		if ((levelA == IR_DATA_SPACE) && (levelB == IR_DATA_MARK))
			data = (data << 1) | 1;
		else if ((levelA == IR_DATA_MARK) && (levelB == IR_DATA_SPACE))
			data = (data << 1) | 0;
		else
			return 0;
	}

	// Success
	results->bits = nbits;
	results->value = data;
	results->decode_type = RC5;
	return 1;
}
#endif

//+=============================================================================
// RRRR    CCCC   6666
// R   R  C      6
// RRRR   C      6666
// R  R   C      6   6
// R   R   CCCC   666
//
// NB : Caller needs to take care of flipping the toggle bit
//
#define MIN_RC6_SAMPLES      1
#define RC6_HDR_MARK      2666
#define RC6_HDR_SPACE      889
#define RC6_T1             444
#define RC6_RPT_LENGTH   46000

//+=============================================================================
#if DECODE_RC6
uint8_t decodeRC6(struct decode_results_t *results)
{
	int nbits;
	long data = 0;
	int used = 0;
	int offset = 1;  // Skip first space

	if (results->rawlen < MIN_RC6_SAMPLES)
		return 0;

	// Initial mark
	if (!MATCH_MARK(results->rawbuf[offset++], RC6_HDR_MARK))
		return 0;
	if (!MATCH_SPACE(results->rawbuf[offset++], RC6_HDR_SPACE))
		return 0;

	// Get start bit (1)
	if (getRClevel(results, &offset, &used, RC6_T1) != IR_DATA_MARK)
		return 0;
	if (getRClevel(results, &offset, &used, RC6_T1) != IR_DATA_SPACE)
		return 0;

	for (nbits = 0; offset < results->rawlen; nbits++) {
		int levelA, levelB;  // Next two levels

		levelA = getRClevel(results, &offset, &used, RC6_T1);
		if (nbits == 3) {
			// T bit is double wide; make sure second half matches
			if (levelA != getRClevel(results, &offset, &used, RC6_T1))
				return 0;
		}

		levelB = getRClevel(results, &offset, &used, RC6_T1);
		if (nbits == 3) {
			// T bit is double wide; make sure second half matches
			if (levelB != getRClevel(results, &offset, &used, RC6_T1))
				return 0;
		}

		if ((levelA == IR_DATA_MARK) && (levelB == IR_DATA_SPACE))
			data = (data << 1) | 1;  // inverted compared to RC5
		else if ((levelA == IR_DATA_SPACE) && (levelB == IR_DATA_MARK))
			data = (data << 1) | 0;  // ...
		else
			return 0;            // Error
	}

	// Success
	results->bits = nbits;
	results->value = data;
	results->decode_type = RC6;
	return 1;
}
#endif

