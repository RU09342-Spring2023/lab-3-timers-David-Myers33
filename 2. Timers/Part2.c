/*
 * Part2.c
 *
 *  Created on: Feb 16, 2023
 *      Author: David Myers
 *
 *      This code will need to change the speed of an LED between 3 different speeds by pressing a button.
 */

#include <msp430.h>
int count=0;
void gpioInit();
void timerInit();

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);

}
char REDLED = 0x01;

void gpioInit(){
  // @TODO Initialize the Red or Green LED
    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction



  // @TODO Initialize Button 2.3
    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES &= ~BIT3;                         // P2.3 Low --> High edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled
    P2IFG &= ~BIT3;

}

void timerInit(){

    TB1CCTL0 = CCIE;                        //enables interrupt
    TB1CTL = TBSSEL_1 | MC_2;               //ACLK, continuous mode
    TB1CCR0 = 5000;                         //sets start time
    // @TODO Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on

}


/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    count ++;                   //adds 1 to counter
    P1OUT^= BIT0;               //toggles red led
    P2IFG&= ~BIT3;              //clears P2.3 flag

    // @TODO Remember that when you service the GPIO Interrupt, you need to set the interrupt flag to 0.

    // @TODO When the button is pressed, you can change what the CCR0 Register is for the Timer. You will need to track what speed you should be flashing at.

}


// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    if (count == 1)             //checks if count is 1
            {
    TB1CCR0 += 5000;            //adds 5000 to clock
    P1OUT ^= BIT0;              //toggles red led
            }
    else if (count == 2)        //checks if count is 2
            {
            TB1CCR0 += 10000;   //adds 10000 to clock
            P1OUT ^= BIT0;      //toggles red led
                    }
    else if (count == 3)        //checks if count is 3
            {
            TB1CCR0 += 15000;   //adds 15000 to clock
            P1OUT ^= BIT0;      //toggles red led
                    }
    else
        count = 0;              // if button presses surpass 3, the count resets
    // @TODO You can toggle the LED Pin in this routine and if adjust your count in CCR0.
}


