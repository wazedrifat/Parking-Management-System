/*
 * L3_T1.c
 *
 * Created: 05-Apr-19 7:44:51 AM
 * Author : Wazed Rifat
 */

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRA = 0x00; // used as input
	DDRD = 0xff;
	DDRC = 0xff;

	int path[8][3] = {
		{0b0, 0b0, 0b0},
		{0b1, 0b1, 0b1},
		{0b10, 0b10, 0b10},
		{0b100, 0b1000, 0b1000},
		{0b100, 0b10000, 0b10000},
		{0b100, 0b100000, 0b1000000},
		{0b100, 0b100000, 0b10000000}};
	int pos = 0;
	int len[8] = {0, 1, 1, 2, 2, 3, 3};
	int flag = 0;
	// int a[8]={0b1, 0b10, 0b100, 0b1000, 0b10000, 0b100000, 0b1000000, 0b10000000};

	while (1)
	{
		PORTC = 0xf0;
		PORTA = 0xff;
		PORTD = 0x00;

		if ((PINA & 0b1) == 0b1)
		{
			int val = PINA;

			int p = 0;
			for (int i = 1; i <= 6; i++)
			{
				val = (val >> 1);
				if ((val & 0b1) == 0b0)
				{
					p = i;
					break;
				}
			}

			///val = (1 << p);

			if (p < 1 || p > 6)
				continue;

			while (1)
			{
				if ((PINA & 0b1) == 0b1)
					PORTC = 0b10;

				if ((PINA >> p) & 0b1 == 0b1)
				{
					PORTD |= 0xff;
					_delay_ms(700);
					PORTD = 0x00;
					PORTC &= 0xfe;
					break;
				}

				PORTC |= 0b1;
				_delay_ms(200);
				PORTC &= 0xfe;
				_delay_ms(200);

				for (int i = 0; i < len[p]; i++)
				{
					PORTD = path[p][i];
					_delay_ms(200);

					PORTD = 0b0;
					_delay_ms(200);
				}

				if (!((PINA & 0b1) == 0b1))
					PORTC = 0xf0;
			}
		}
	}

	return 0;
}
