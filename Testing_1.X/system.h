/* 
 * File:   System.h
 * Author: PatrickTiongJoonKiat
 *
 * Created on May 1, 2015, 10:48 AM
 */


/******************************************************************************
 * Stepper Motor Driver Carrier 
 * Info: Will be use to determine the microstepping, two types of stepper motor 
 * Driver Carrier is available, A4988 or DVR8825
 * A4988 maximum frequencies is 500kHz and pulse duration is 200ns
 * DVR8825 maximum frequencies is 250kHz and pulse duration is 1.9us
 ******************************************************************************/
#define DVR8825
#define forward 0;
#define reverse 1;

/******************************************************************************
 *OSCILLATOR frequencies
 * Info: Define for the external crystal and mode with will be use to determine
 * the OSC speed.
 * FOSC is the frequency of the chip osc after the Phase-Locked Loop multiplier
 * FOSC == frequency of oscillator
 * TOSC == time of oscillator [period],1/Fosc
 ******************************************************************************/
#if defined (__18F2331)
#define _OSC_FREQ               20000000

#elif defined (__18F4431)
#define _OSC_FREQ               40000000

#endif


/******************************************************************************
 * UART Baudrate
 * Info: Define UART Baudrate which will be use in the UART Header file
 *
 ******************************************************************************/
#define UART_BAUD               9600
#define Buffer_size				16

#define Slave1                  LATBbits.LATB5

/******************************************************************************
 * I/O Connections
 * Info: Define the I/O pin connection used in the electronic boards, Encoder
 * Module
 ******************************************************************************/
#if defined (__18F2331)
    #define ENABLE_PIN              LATBbits.LATB5
    #define STEP_PIN                LATBbits.LATB1
    #define DIR_PIN                 LATBbits.LATB0
    #define MODE1_PIN               LATBbits.LATB4
    #define MODE2_PIN               LATBbits.LATB3
    #define MODE3_PIN               LATBbits.LATB2
    #define STATUS_RECEIVING        LATCbits.LATC3
    #define STATUS_BUSY             LATCbits.LATC2
    #define STATUS_READY            LATCbits.LATC1

#elif defined (__18F4431)
    #define ENABLE_PIN              LATDbits.LATD7
    #define STEP_PIN                LATDbits.LATD6
    #define DIR_PIN                 LATDbits.LATD5
    #define MODE1_PIN               LATBbits.LATB4
    #define MODE2_PIN               LATBbits.LATB5
    #define MODE3_PIN               LATDbits.LATD1
    #define STATUS_BUSY             LATBbits.LATB6
    #define STATUS_READY            LATBbits.LATB7
    #define STATUS_RECEIVING        LATBbits.LATB7
#endif  

#if defined (A4988)
    #define SPEED1                  {MODE1_PIN =0; MODE2_PIN =0; MODE3_PIN = 0;}    // Full step
    #define SPEED2                  {MODE1_PIN =1; MODE2_PIN =0; MODE3_PIN = 0;}    // Half step
    #define SPEED3                  {MODE1_PIN =0; MODE2_PIN =1; MODE3_PIN = 0;}    // Quarter step
    #define SPEED4                  {MODE1_PIN =1; MODE2_PIN =1; MODE3_PIN = 0;}    // Eight step
    #define SPEED5                  {MODE1_PIN =1; MODE2_PIN =1; MODE3_PIN = 1;}    // Sixteenth step

#elif defined(DVR8825)
    #define SPEED1                  {MODE1_PIN =0; MODE2_PIN =0; MODE3_PIN = 0;}    // Full step
    #define SPEED2                  {MODE1_PIN =1; MODE2_PIN =0; MODE3_PIN = 0;}    // Half step
    #define SPEED3                  {MODE1_PIN =0; MODE2_PIN =1; MODE3_PIN = 0;}    // Quarter step
    #define SPEED4                  {MODE1_PIN =1; MODE2_PIN =1; MODE3_PIN = 0;}    // Eight step
    #define SPEED5                  {MODE1_PIN =0; MODE2_PIN =0; MODE3_PIN = 1;}    // Sixteenth step
    #define SPEED6                  {MODE1_PIN =1; MODE2_PIN =0; MODE3_PIN = 1;}    // 1/32 step
#endif

