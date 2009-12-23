#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

/* PORTB pins 0..3 used for the stepper motor windings
 */

/* PORTD pin 2 used for the step (PD2)
 * PORTD pin 3 is used for the direction (PD3)
 */

/* Define Pins	*/
#define STEP_BIT	2
#define DIR_BIT		3
#define DIR_CW		1
#define DIR_CCW		-1
#define MIN_STEP	0
#define MAX_STEP	3

const unsigned char stepwindings[MAX_STEP+1] = {1,2,4,8};

int main (void)
{
	int current_step=0;
	int current_direction = 0;

	/* PortB is used for output	*/
	DDRB = 0xFF;

	/* PortD is used for input	*/
	DDRD = 0x00;

	/* Enable the pull ups		*/
	PORTD = 0xFF;

	_delay_ms(1000);

	/* Initialise output port	*/
	PORTB=stepwindings[current_step];

	while(1)
	{
		// wait for next step
		loop_until_bit_is_clear(PIND,STEP_BIT);

		// get current direction
		current_direction = (bit_is_set(PIND,DIR_BIT) ? DIR_CW : DIR_CCW);
		current_step += current_direction;

		// compute next step bit index
		if (current_step < MIN_STEP)
		  current_step = MAX_STEP;
		else if (current_step > MAX_STEP)
		  current_step = MIN_STEP;

		// apply next step
		PORTB=stepwindings[current_step];

		// wait for step bit to be high again
		loop_until_bit_is_set(PIND,STEP_BIT);
	}
}
