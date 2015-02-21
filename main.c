#include <msp430.h>
#include "IR_read.h"

#define BatVoltage BIT0
#define Switch1 BIT1
#define LED0 BIT3
#define LED1 BIT2
#define IRrec BIT4
#define IRrec1 BIT5
#define TX BIT6

#define ArraySize 64

unsigned char differences =0;
unsigned char differences2 =0;
int amount =10;

int ledcount= 1000;
unsigned char hold =0;
unsigned char holdArrayamount =ArraySize;

unsigned char TXdataE[12] = {0,2,2,2,2,2,0,0,1,0,0,0}; // channel E
unsigned char TXdataD[12] = {2,2,2,2,2,2,0,0,1,0,0,0}; // channel D (A1 A0 x x x x x x)
unsigned int receive[ArraySize];
unsigned int HOLDreceive[ArraySize];
unsigned int HOLD2receive[ArraySize];
unsigned char i=0;
int timeout =0;
unsigned char HoldArrayNumber =1;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	  P1DIR |= (LED1 + LED0 + TX);                            // Set all pins but RXD to output
	  P1OUT &= ~(LED1 + LED0 + TX);
	  P1REN |= Switch1;
	  P1OUT |= Switch1;
	  P1IE |= IRrec; // P1.3 interrupt enabled
	  P1IES &= ~IRrec; // P1.3 interrupt enabled
	  P1IFG &= ~IRrec; // P1.3 IFG cleared
	  P1IE |= IRrec1; // P1.3 interrupt enabled
	  P1IES |= IRrec1; // P1.3 interrupt enabled
	  P1IFG &= ~IRrec1; // P1.3 IFG cleared

	  TA0CTL = TASSEL_2 + MC_2 + ID_3;           // SMCLK/8, upmode

	  _BIS_SR(LPM4_bits + GIE); // Enter LPM3 w/interrupt

	  //__enable_interrupt(); // enable all interrupts                                   // Enable CPU interrupts
	while(1)
	{

		_delay_cycles(100);


timeout++;
if(timeout == 1000)
{
	TA0CTL |= TACLR;
	P1IE &= ~(IRrec + IRrec1); // P1.3 interrupt enabled
	hold = readSwitchP1(Switch1);
if(i>20)
{
	if(hold == 1)
	{
		holdArrayamount = (i-2);
		i=0;
		if(HoldArrayNumber == 1)
		{
			ArrayCopy(receive,HOLDreceive, ArraySize);
			BlinkLED(LED1);
			HoldArrayNumber = 2;
		}
		else
		{
			ArrayCopy(receive,HOLD2receive, ArraySize);
			HoldArrayNumber =1;
			Blink2LED(LED1,LED0);
		}


	}
	differences = ArrayCompare(receive, HOLDreceive, amount, holdArrayamount);
	_delay_cycles(10);
	differences2 = ArrayCompare(receive, HOLD2receive, amount, holdArrayamount);
	_delay_cycles(10);
	resetBuffer(receive, ArraySize);
	_delay_cycles(10);

	if(differences == 0)
	{
		P1OUT |= LED1;
	    P1OUT &= ~LED0;
		send_rf(TX, TXdataD);
		_delay_cycles(1000);
		send_rf(TX, TXdataD);

	}
	else if(differences2 == 0)
	{
		P1OUT |= LED1 + LED0;
		send_rf(TX, TXdataE);
		_delay_cycles(1000);
		send_rf(TX, TXdataE);

	}
	else
	{
		P1OUT |= LED0;
		P1OUT &= ~LED1;
	}
	ledcount =0;
}
i=0;
P1IE |= (IRrec + IRrec1); // P1.3 interrupt enabled
}
if(timeout >1050)
{
timeout =1050;
}



ledcount++;
if(ledcount == 1000)
{
	P1OUT &= ~(LED0 + LED1);
	P1IE |= (IRrec + IRrec1); // P1.3 interrupt enabled
	_BIS_SR(LPM4_bits + GIE); // Enter LPM3 w/interrupt
}
if(ledcount > 1050)
{
ledcount = 1050;
}


}
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	_BIC_SR(LPM4_EXIT); // wake up from low power mode

    if(i<=(ArraySize +1))
    {

    if(i>=2)
    {
    	receive[i-2] = TAR;
    }
    i++;
    }

  TA0CTL |= TACLR;
  P1IFG &= ~IRrec; // P1.3 IFG cleared
  P1IFG &= ~IRrec1; // P1.3 IFG cleared
  timeout=0;
}

