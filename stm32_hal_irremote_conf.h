#ifndef STM32_HAL_IRREMOTE_CONF_H_
#define STM32_HAL_IRREMOTE_CONF_H_

#ifndef IRREMOTE_RAWBUF
#define IRREMOTE_RAWBUF 256
#endif

// IR detector output is active low
#define IR_DATA_MARK   GPIO_PIN_RESET
#define IR_DATA_SPACE  GPIO_PIN_SET

#define IR_USECPERTICK 1
// Due to sensor lag, when received, Marks  tend to be 100us too long and Spaces tend to be 100us too short
#define IR_MARK_EXCESS 0


#define _IR_GAP            15000
#define IR_GAP_TICKS       (_IR_GAP/IR_USECPERTICK)



#define DECODE_RC5           1 // чтоб отключить декодирование протокола нужно указать 0
#define DECODE_RC6           1
#define DECODE_NEC           1
#define DECODE_SONY          0
#define DECODE_PANASONIC     0
#define DECODE_JVC           0
#define DECODE_SAMSUNG       0
#define DECODE_WHYNTER       0
#define DECODE_AIWA_RC_T501  0
#define DECODE_LG            0
#define DECODE_SANYO         0
#define DECODE_MITSUBISHI    0
#define DECODE_DENON         0

#endif /* STM32_HAL_IRREMOTE_CONF_H_ */
