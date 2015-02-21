#include "IR_read.h"
#include <msp430.h>

void resetBuffer(unsigned int* Array, unsigned char amount)
{
	int i ;
	for(i =0; i<(amount -1);i++)
	{
		Array[i] =0;
	}
}

void send_rf(unsigned char pin, unsigned char* data)
{
unsigned char i=0;

for(i=0;i<12;i++)
{
if(data[i] == 0)
{
	zero(pin);
}
else if(data[i] ==1)
{
	one(pin);
}
else
{
	f(pin);
}

}

P1OUT |= pin;
_delay_cycles(250);
P1OUT &= ~pin;
_delay_cycles(7000);
}

void zero(unsigned char pin)
{
	P1OUT |= pin;
	_delay_cycles(250);
	P1OUT &= ~pin;
	_delay_cycles(690);
	P1OUT |= pin;
	_delay_cycles(250);
	P1OUT &= ~pin;
	_delay_cycles(690);
}
void one(unsigned char pin)
{
	P1OUT |= pin;
	_delay_cycles(810);
	P1OUT &= ~pin;
	_delay_cycles(210);
	P1OUT |= pin;
	_delay_cycles(810);
	P1OUT &= ~pin;
	_delay_cycles(210);
}
void f(unsigned char pin)
{
	P1OUT |= pin;
	_delay_cycles(250);
	P1OUT &= ~pin;
	_delay_cycles(700);
	P1OUT |= pin;
	_delay_cycles(810);
	P1OUT &= ~pin;
	_delay_cycles(200);
}

unsigned char ArrayCompare(unsigned int* Array1, unsigned int* Array2, int amount, unsigned char characters)
{
	int i;
	unsigned char differences =0;
	for(i=0;i<(characters -1);i++ )
	{
		if(((int)Array1[i] - (int)Array2[i]) > amount)
		{
		differences++;
		}
		else if(((int)Array2[i] - (int)Array1[i]) > amount)
		{
			differences++;
		}
	}
	return differences;
}

void ArrayCopy(unsigned int* Array1, unsigned int* Array2, unsigned char characters)
{
	int i;
	for(i=0;i<(characters -1);i++ )
	{
		Array2[i] = Array1[i];
	}
}

unsigned char readSwitchP1(unsigned char bit)
{
	int i =0;
	int count =0;

	for(i=10;i>0;i--)
	             {
	           	    if((P1IN & bit) != bit)
	           	    {
	           	    	count++;
	           	    }
	           	    _delay_cycles(10);
	             }
	if(count > 8)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void BlinkLED(unsigned char bit)
{
	P1OUT |= bit;
	_delay_cycles(100000);
	P1OUT &= ~bit;
	_delay_cycles(100000);
	P1OUT |= bit;
	_delay_cycles(100000);
	P1OUT &= ~bit;
	_delay_cycles(100000);

	P1OUT |= bit;
	_delay_cycles(100000);
	P1OUT &= ~bit;
	_delay_cycles(100000);
	P1OUT |= bit;
	_delay_cycles(100000);
	P1OUT &= ~bit;
	_delay_cycles(100000);

	P1OUT |= bit;
	_delay_cycles(100000);
	P1OUT &= ~bit;
	_delay_cycles(100000);
}
void Blink2LED(unsigned char bit, unsigned char bit2 )
{
	P1OUT |= bit + bit2;
	_delay_cycles(100000);
	P1OUT &= ~(bit + bit2);
	_delay_cycles(100000);
	P1OUT |= bit + bit2;
	_delay_cycles(100000);
	P1OUT &= ~(bit + bit2);
	_delay_cycles(100000);

	P1OUT |= bit + bit2;
	_delay_cycles(100000);
	P1OUT &= ~(bit + bit2);
	_delay_cycles(100000);
	P1OUT |= bit + bit2;
	_delay_cycles(100000);
	P1OUT &= ~(bit + bit2);
	_delay_cycles(100000);

	P1OUT |= bit + bit2;
	_delay_cycles(100000);
	P1OUT &= ~(bit + bit2);
	_delay_cycles(100000);
}
