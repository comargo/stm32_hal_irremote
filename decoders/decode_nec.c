/*
 * decode_nex.c
 *
 *  Created on: Dec 9, 2021
 *      Author: cyril
 */


#include "decoders.h"


//==============================================================================
//                           N   N  EEEEE   CCCC
//                           NN  N  E      C
//                           N N N  EEE    C
//                           N  NN  E      C
//                           N   N  EEEEE   CCCC
//==============================================================================

#define NEC_BITS          32
#define NEC_HDR_MARK    9000
#define NEC_HDR_SPACE   4500
#define NEC_BIT_MARK     560
#define NEC_ONE_SPACE   1690
#define NEC_ZERO_SPACE   560
#define NEC_RPT_SPACE   2250

//+=============================================================================
// NECs have a repeat only 4 items long
//
#if DECODE_NEC
static inline uint8_t inverseBits(uint8_t val)
{
  uint8_t newVal = 0;
  uint8_t mask = UINT8_C(0x1);
  while(mask != 0) {
    newVal <<= 1;
    newVal |= (val&mask)?1:0;
    mask <<= 1;
  }
  return newVal;
}

uint8_t decodeNEC (struct decode_results_t *results)
{
	long  data   = 0;  // We decode in to here; Start with nothing
	int   offset = 1;  // Index in to results; Skip first entry!?

	// Check header "mark"
	if (!MATCH_MARK(results->rawbuf[offset], NEC_HDR_MARK))  return 0 ;
	offset++;

	// Check for repeat
	if ( (results->rawlen == 4)
	    && MATCH_SPACE(results->rawbuf[offset  ], NEC_RPT_SPACE)
	    && MATCH_MARK (results->rawbuf[offset+1], NEC_BIT_MARK )
	   ) {
		results->bits        = 0;
		results->value       = IR_REPEAT;
		results->decode_type = NEC;
		return 1;
	}

	// Check we have enough data
	if (results->rawlen < (2 * NEC_BITS) + 4)  return 0 ;

	// Check header "space"
	if (!MATCH_SPACE(results->rawbuf[offset], NEC_HDR_SPACE))  return 0 ;
	offset++;

	// Build the data
	for (int i = 0;  i < NEC_BITS;  i++) {
		// Check data "mark"
		if (!MATCH_MARK(results->rawbuf[offset], NEC_BIT_MARK))  return 0 ;
		offset++;
        // Suppend this bit
		if      (MATCH_SPACE(results->rawbuf[offset], NEC_ONE_SPACE ))  data = (data << 1) | 1 ;
		else if (MATCH_SPACE(results->rawbuf[offset], NEC_ZERO_SPACE))  data = (data << 1) | 0 ;
		else                                                            return 0 ;
		offset++;
	}

	// Success
	results->bits        = NEC_BITS;
	results->value       = data;
	results->decode_type = NEC;
	results->address = inverseBits((data&0xFF000000)>>24);
	results->command = inverseBits((data&0x0000FF00)>>8);

	return 1;
}
#endif
