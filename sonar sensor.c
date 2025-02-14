#include <avr/io.h>
#include <avr/interrupt.h>

//header to enable data flow control over pins

#define F_CPU 1000000

//telling controller crystal frequency attached

#include <util/delay.h>

//header to enable delay function in program

#define    E   5

//giving name “enable”  to 5th pin of PORTD, since it Is connected to LCD enable pin

#define RS  6

//giving name “registerselection” to 6th pin of PORTD, since is connected to LCD RS pin

void send_a_command(unsigned char command);

void send_a_character(unsigned char character);

void send_a_string(char *string_of_characters);

static volatile int pulse = 0;//interger  to access all though the program

static volatile int i = 0;// interger  to access all though the program

int main(void)

{
	
	DDRA = 0xff;
	PORTA = 0x00;
	DDRB = 0xFF;

	//putting portB output pins

	DDRD = 0b11111011;

	_delay_ms(50);//giving delay of 50ms

	DDRA = 0xFF;//Taking portA as output.

	GICR|=(1<<INT0);//enabling interrupt0

	MCUCR|=(1<<ISC00);//setting interrupt triggering logic change

	int16_t COUNTA = 0;//storing digital output

	char SHOWA [3];//displaying digital output as temperature in 16*2 lcd

	send_a_command(0x01); //Clear Screen 0x01 = 00000001

	_delay_ms(50);

	send_a_command(0x38);//telling lcd we are using 8bit command /data mode

	_delay_ms(50);

	send_a_command(0b00001111);//LCD SCREEN ON and courser blinking

	sei();// enabling global interrupts

	while(1)

	{

		PORTD|=(1<<PIND0);

		_delay_us(15);///triggering the sensor for 15usec

		PORTD &=~(1<<PIND0);

		COUNTA = pulse/58;//getting the distance based on formula on introduction
		
		PORTA = 0x00;
		if(COUNTA < 5)			PORTA = 0x1;
		else if(COUNTA < 10)	PORTA = 0x2;
		else if(COUNTA < 15)	PORTA = 0x4;
		else					PORTA = 0x8;
		_delay_ms(1000);


	}

}

ISR(INT0_vect)//interrupt service routine when there is a change in logic level

{

	if (i==1)//when logic from HIGH to LOW

	{

		TCCR1B=0;//disabling counter

		pulse=TCNT1;//count memory is updated to integer

		TCNT1=0;//resetting the counter memory

		i=0;

	}

	if (i==0)//when logic change from LOW to HIGH

	{

		TCCR1B|=(1<<CS10);//enabling counter

		i=1;

	}

}

void send_a_command(unsigned char command)

{

	PORTA = command;

	PORTD &= ~ (1<<RS); //putting 0 in RS to tell lcd we are sending command

	PORTD |= 1<<E; //telling lcd to receive command /data at the port

	_delay_ms(50);

	PORTD &= ~1<<E;//telling lcd we completed sending data

	PORTA= 0;

}

void send_a_character(unsigned char character)

{

	PORTA= character;

	PORTD |= 1<<RS;//telling LCD we are sending data not commands

	PORTD |= 1<<E;//telling LCD to start receiving command/data

	_delay_ms(50);

	PORTD &= ~1<<E;//telling lcd we completed sending data/command

	PORTA = 0;

}

void send_a_string(char *string_of_characters)

{

	while(*string_of_characters > 0)

	{

		send_a_character(*string_of_characters++);

	}

}