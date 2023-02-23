/*
 * Part3.c
 *
 *  Created on: Feb 12, 2023
 *      Author: FILL THIS IN
 *
 *      YOU NEED TO FILL IN THIS AUTHOR BLOCK
 */
#include <msp430.h>
void gpioInit();
void timerInit();
int count=0;
int count1=0;

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
       timerInit();

       // Disable the GPIO power-on default high-impedance mode
       // to activate previously configured port settings
       PM5CTL0 &= ~LOCKLPM5;

       __bis_SR_register(LPM3_bits | GIE);

}



    void gpioInit(){
        P1OUT &= ~BIT0;                               // Clear P1.0 output latch for a defined power-on state
           P1DIR |= BIT0;                          // Set P1.0 to output direction
           P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
           P2REN |= BIT3;                          // P2.3 pull-up register enable
           P2IES &= ~BIT3;                         // P2.3 Low --> High edge
           P2IE |= BIT3;                           // P2.3 interrupt enabled
           P2IFG&= ~BIT3;



    }



    void timerInit(){
        TB1CCTL0 = CCIE;
           TB1CTL = TBSSEL_1 | MC_2;
           TB1CCR0 = 5000;
}



#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{

    //count++;
    P1OUT^= BIT0;               //toggles red led
    P2IFG&= ~BIT3;
}

#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    if((P2IES & BIT3)==1){

        //count++;
     //count1 = count;
        //count = 0;

   // TB1CCR0 += count;
       //P1OUT ^= BIT0;
      // TB1CCTL0 &= ~CCIFG;
        TB1CCR0 += count;
        P1OUT ^= BIT0;

    }
else if((P2IES & BIT3)==0)
{
    count++;

             //TB1CCTL0 &= ~CCIFG;

}
}
