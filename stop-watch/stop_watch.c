/*
 * stop_watch.c
 *
 *  Created on: Jan 13, 2020
 *      Author: menna
 */

#include"common_macros.h"
#include"micro_config.h"
#include"std_types.h"

/* variables to hold the clock time */
uint8 seconds = 0;
uint8 minutes = 0;
uint8 hours = 0;

/* variable to count the number of timer ticks (compare match) */
uint8 g_tick = 0;

/* flag will be set when the timer count 1 second */
uint8 count_second_flag = 0;

/* Interrupt Service Routine for timer1 compare mode channel A */
ISR(TIMER1_COMPA_vect) {
	count_second_flag = 1;
}

void timer1_init_CTC_mode(uint16 tick) {
	TCNT1 = 0; /* timer initial value */

	OCR1A = tick; /* compare value */

	TIMSK |= (1 << OCIE1A); /* enable compare interrupt for channel A */

	/* Configure timer1 control registers
	 * 1. Non PWM mode FOC1A=1 and FOC1B=1
	 * 2. No need for OC1A & OC1B in this example so COM1A0=0 & COM1A1=0 & COM1B0=0 & COM1B1=0
	 * 3. CTC Mode and compare value in OCR1A WGM10=0 & WGM11=0 & WGM12=1 & WGM13=0
	 */
	TCCR1A = (1 << FOC1A) | (1 << FOC1B);
	/*
	 * 4. Clock = F_CPU/1024 CS10=1 CS11=0 CS12=1
	 */
	TCCR1B |= (1 << CS10) | (1 << CS12) | (1 << WGM12);
}

/* External INT0 Interrupt Service Routine */
ISR(INT0_vect) {
	seconds = 0;
	minutes = 0;
	hours = 0;
}

/* External INT1 Interrupt Service Routine */
ISR(INT1_vect) {
	/*
	 * pause the stop watch by disable the timer
	 * clear the timer clock bits (CS10=0 CS11=0 CS12=0) to stop the timer clock.
	 */
	TCCR1B &= 0xF8;
}

/* External INT2 Interrupt Service Routine */
ISR(INT2_vect) {
	/* resume the stop watch by enable the timer through the clock bits. */
	TCCR1B |= (1 << CS10) | (1 << CS12);
}

/* External INT0 enable and configuration function */
void INT0_Init(void) {
	/* Enable external interrupt pin INT0 */
	SET_BIT(GICR, INT0);
	/* Trigger INT0 with the falling edge */
	CLEAR_BIT(MCUCR, ISC00);
	SET_BIT(MCUCR, ISC01);
}

/* External INT1 enable and configuration function */
void INT1_Init(void) {
	/* Trigger INT1 with the raising edge */
	SET_BIT(MCUCR, ISC10);
	SET_BIT(MCUCR, ISC11);

	/* Enable external interrupt pin INT1 */
	SET_BIT(GICR, INT1);
}

/* External INT2 enable and configuration function */
void INT2_Init(void) {
	/* Trigger INT2 with the falling edge */
	CLEAR_BIT(MCUCSR, ISC2);
	/* Enable external interrupt pin INT2 */
	SET_BIT(GICR, INT2);
}

int main(void) {
	/* configure INT0/PD2 as input */
	CLEAR_BIT(DDRD, PD2);

	/*enable internal pull up resistor at INT0/PD2 pin */
	SET_BIT(PORTD, PD2);

	/* configure INT1/PD3 as input */
	CLEAR_BIT(DDRD, PD3);

	/*configure INT2/PB2 as input */
	CLEAR_BIT(DDRB, PB2);

	/*enable internal pull up resistor at INT2/PB2 pin */
	SET_BIT(PORTB, PB2);

	/* configure first 6 pins in PORTA as output pins */
	DDRA |= 0x3F;

	/* configure first 4 pins in PORTC as output pins */
	DDRC |= 0x0F;

	/* initialize all the 7-segment with zero values */
	PORTA |= 0x3F;
	PORTC &= 0xF0;

	/* enable global interrupts in MC */
	SET_BIT(SREG, 7);

	/* start timer1 to generate compare interrupt every 1000 MiliSeconds(1 Second) */
	timer1_init_CTC_mode(1000);

	/* activate external interrupt INT0 */
	INT0_Init();

	/* activate external interrupt INT1 */
	INT1_Init();

	/* activate external interrupt INT2 */
	INT2_Init();

	while (1) {
		if (count_second_flag == 1) {
			/*
			 * enter here every one second
			 * increment seconds count
			 */
			seconds++;

			if (seconds == 60) {
				seconds = 0;
				minutes++;
			}
			if (minutes == 60) {
				minutes = 0;
				hours++;
			}
			if (hours == 24) {
				hours = 0;
			}
			/* reset the flag again */
			count_second_flag = 0;
		} else {
			/* out the number of seconds */
			PORTA = (PORTA & 0xC0) | 0x01;
			PORTC = (PORTC & 0xF0) | (seconds % 10);

			/*
			 * make small delay to see the changes in the 7-segment
			 * 2Miliseconds delay will not effect the seconds count
			 */
			_delay_ms(2);

			PORTA = (PORTA & 0xC0) | 0x02;
			PORTC = (PORTC & 0xF0) | (seconds / 10);

			_delay_ms(2);

			/* out the number of minutes */
			PORTA = (PORTA & 0xC0) | 0x04;
			PORTC = (PORTC & 0xF0) | (minutes % 10);

			_delay_ms(2);

			PORTA = (PORTA & 0xC0) | 0x08;
			PORTC = (PORTC & 0xF0) | (minutes / 10);

			_delay_ms(2);

			/* out the number of hours */
			PORTA = (PORTA & 0xC0) | 0x10;
			PORTC = (PORTC & 0xF0) | (hours % 10);

			_delay_ms(2);

			PORTA = (PORTA & 0xC0) | 0x20;
			PORTC = (PORTC & 0xF0) | (hours / 10);

			_delay_ms(2);
		}
	}
}

