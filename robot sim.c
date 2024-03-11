    //TEJ3M mainWritePort.c
    /*********************************************************************

    Author: Noor Azam
    Date:	Jan 23, 2023

    This program contains code to simulate a robot that follows a black line

    Hardware Notes:
     * PIC24FV32KA302 operating at 8MHz
     * 
    Outputs:
     * RB6:RB7:RB8:RB9
     Inputs: 
     * RA2:RA3
    ********************************************************************/

    /*******************************************************************
        Include Files
    ********************************************************************/
    
    #include "p24f32ka302.h"
    #include "configBits.h"
    #include "delay.h"

    /*******************************************************************
        Symbolic Constants used by main()
    ********************************************************************/
        
    //delay constants
    #define SHORT_DELAY 50
    #define LONG_DELAY 70 

    /*******************************************************************
            Local Function Prototypes
    ********************************************************************/
    
    void initTimer (void);
    void delay (unsigned long milli);
    void left (void);
    void right (void);
    void straight (void);
    void turnAround (void);

    /*******************************************************************
        Global Variable Declarations
    ********************************************************************/

    unsigned int allOff;
    unsigned int hasTurned;

    /*******************************************************************
        main() function
    ********************************************************************/
    int main (void)
    {
    //set variable values
        allOff = 0x0000;
        hasTurned = 0;
    //initialize peripherals
        initTimer();
    //PORTB all outputs
        TRISB=0;
    // initialize PORTB
        LATB=0;
    //Set PORTA to all digital inputs
        TRISA=0xFF;
    //initialize all PORTA
        ANSA=0;

        while (1) //ensures that program loops infinitely between the {} brackets
     {

            //run straight method if both sensors sense white
            if (PORTAbits.RA2 == 1 && PORTAbits.RA3 == 1)
            {
                straight();
            }

            //run left method if only right sensor senses white
            else if (PORTAbits.RA2 ==0 && PORTAbits.RA3 == 1)
            {
                left();
            }

            //run right method is only left sensor senses white 
            else if (PORTAbits.RA3 ==0 && PORTAbits.RA2 == 1)
            {
                right();
            }

            //if none of the sensors sense white, turn around only if it has not turned previously 
            else if (PORTAbits.RA3 ==0 && PORTAbits.RA2 == 0 && hasTurned == 0)
            {
                turnAround();
            } 
            
            //stop robot if none of the above conditions are met
            else 
            {
                LATB=allOff;
            }
     }

    } //end mainWritePort
    /*******************************************************************
        List of Functions
    ********************************************************************/

    //both wheels move forward 
    straight()
    {
        LATBbits.LATB6=1;
        LATBbits.LATB8=1;

        delay (SHORT_DELAY);

        LATB = allOff;

        delay (LONG_DELAY);
    }

    //right wheel moves forward, left wheel moves backwards
    left()
    {
        LATBbits.LATB6=1;
        LATBbits.LATB9=1;

        delay (100);

        LATB = allOff;

        delay (LONG_DELAY);
    }

    //left wheel moves forward, right wheel moves backwards
    right()
    {
        LATBbits.LATB7=1;
        LATBbits.LATB8=1;

        delay (100);

        LATB = allOff;

        delay (LONG_DELAY);
    }

    //turn around 
    turnAround()
    {  
        //repeat while both sensors are off
        while (PORTAbits.RA3 ==0 && PORTAbits.RA2 == 0)
        {
        //move backwards for 50ms
        LATBbits.LATB7=1;
        LATBbits.LATB9=1;

        delay (SHORT_DELAY);

        //keep turning left until left sensor touches black line 
        while (PORTAbits.RA2 == 1)
            {
                //turn left
                LATBbits.LATB6=1;
                LATBbits.LATB9=1;

                delay(SHORT_DELAY);

                LATB = allOff;
            } 
        }
        
        //increment variable to ensure method doesn't run again 
        hasTurned = 1;
    }

