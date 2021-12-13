# stm32_hal_irremote

IRRemote project for STM32 HAL

Based on project https://github.com/stDstm/Example_STM32F103/blob/master/IR_rec_trans and Arduino project https://github.com/Arduino-IRremote/Arduino-IRremote


Need documentation and samples


## Supported IR Remote Controllers
* NEC standard
* RC5 standard
* RC6 standard

## Working modes
The library supports DMA and Interrupted mode

### DMA
For STM32F1xx DMA supported only with TIM2 and TIM4 timer.

### Interrupted mode
It works in interrupted mode

## Pinouts
The data acquisition pin depends on Timer in use:
<table border=1>
<tr><th>Timer</th><th>GPIO output</th></tr>
<tr><td>TIM1</td><td>PA8</td></tr>
<tr><td>TIM2</td><td>PA0</td></tr>
<tr><td>TIM3</td><td>PA6</td></tr>
<tr><td>TIM4</td><td>PB6</td></tr>
</table>

## Interrupts
Do not forget to handle iterrupts.
If you found code in unknown unhandled iterrupt, 
most probably you've forgot the handler for selected timer interrupt

DMA interrupts should be handled too, shouldn't they?