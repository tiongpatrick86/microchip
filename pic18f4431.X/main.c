

    #include <xc.h>
//    #include <adc.h>
    #include <usart.h>
    #include <stddef.h>
    #include <stdbool.h>


//    #pragma config OSC = HS		//if 20MHz external crystal is used, need to select HS
	#pragma config OSC = HSPLL	// Maximum Fosc is 40MHz, so HSPLL is select, please change external crystal to 10MHz
	//#pragma config OSC = INTIO67	// Using internal 8MHz Oscillator, RA6 and RA7 as normal IO pin
	#pragma config FCMEN = OFF 	// Fail-Safe Clock Monitor Enable bit
	#pragma config IESO = OFF		// Internal/External Oscillator Switchover Enable bit
	#pragma config PWRTEN = ON		// Power Up Timer Enable bit 
	#pragma config BOREN = OFF 	// Brown-out Reset Enable bits
    #pragma config BORV = 45
	#pragma config WDTEN = OFF 		// Watch-Dog Timer Enable bit
//	#pragma config CCP2MX = PORTC	//CCP2 at RC1
//	#pragma config PBADEN = ON	// PORTB analog pin as analog input after Reset	
//	#pragma config LPT1OSC = OFF	// Timer 1 configured for high power operation 
//	#pragma config XINST = OFF		// Instruction set extension and Indexed Addressing mode enabled 


//CONFIG3L @ 0x300004
    #pragma config T1OSCMX =OFF
    #pragma config PWMPIN =OFF
    #pragma config HPOL =LOW
    #pragma config LPOL =LOW

//CONFIG3H @ 0x300005
	#pragma config MCLRE = ON		// MCLR pin enable, RE3 input disabled

//CONFIG4L @ 0x300006
	#pragma config STVREN = ON	//Stack overflow will cause Reset
	#pragma config DEBUG = OFF	// Debug disable, RB6 and RB7 as normal IO
	#pragma config LVP = OFF 		// Low voltage programming disable

//CONFIG5L @ 0x300008
    #pragma config CP0 =OFF
    #pragma config CP1 =OFF

//CONFIG5H @ 0x300009
    #pragma config CPB =OFF
    #pragma config CPD =OFF

// CONFIG6L @ 0x30000A
    #pragma config WRT0 =OFF
    #pragma config WRT1 =OFF

// CONFIG6H @ 0x30000B
    #pragma config WRTB =OFF
    #pragma config WRTC =OFF
    #pragma config WRTD =OFF

// CONFIG7L @ 0x30000C
    #pragma config EBTR0 =OFF
    #pragma config EBTR1 =OFF

// CONFIG7H @ 0x30000D
    #pragma config EBTRB =OFF



#if defined (__18F4431)
    #pragma config SSPMX = RD1
#endif



#define _OSC_FREQ 40000000
#define UART_BAUD 9600

unsigned char config1=0b00100000;
unsigned char config2=0b00000000;
unsigned char config3=0b10110010;
unsigned char config4=0b00000010;
unsigned short int adc_reading=0x0f;


void SYSTEM_INT(void);
void Timer5_Int(void);
void Timer5_Start (unsigned short PR_value);
void uart_initialize(void);
void uart_transmit(unsigned char uc_data);



void main(void) {
    SYSTEM_INT();
    uart_initialize();
    
    CloseADC();
    ADCON0 = config1;
    ADCON1 = config2;
    ADCON2 = config3;
    ADCON3 = config4;
    ADCHS = 0b00000000;
    ADCON0bits.ADON=1;
    
    IPR1bits.ADIP =0;
    PIE1bits.ADIE =0;
    
    
    Timer5_Int();
    Timer5_Start(500000);
    
    
    while (true)
    {
//        ADCON0bits.GO=1;
//        while (ADCON0bits.DONE);
//        adc_reading=ReadADC();
//        uart_transmit(ADRESH);
//        uart_transmit(ADRESL);
//        LATBbits.LATB6=1;
//        Delay100TCYx(255);
    }   
    return;
}

void SYSTEM_INT (void)
{
    #if defined (__18F2331)
        PORTA =0;
        PORTB =0;
        PORTC =0;

        TRISA = 0b00011100;
        TRISB = 0b00000000;
        TRISC = 0b10110000;
    
    #elif defined (__18F4431)
        //Initialization of I/O
        PORTA = 0;
        PORTB = 0;
        PORTC = 0;
        PORTD = 0;
        PORTE = 0;

        TRISA = 0b00111111;
        TRISB = 0b00000000;
        TRISC = 0b11110011;
        TRISD = 0b00001100;
        TRISE = 0;
    #endif
}


void uart_initialize(void)
{
	unsigned char dummy = 0;
	TXSTAbits.TX9 =0;

	TXSTAbits.BRGH = 0;			// Select low speed baud rate, if we are using SPBRGH.
	BAUDCTLbits.BRG16 = 1;		//Uses SPBRGH and SPBRG for baudrate generation
	SPBRGH = ( (_OSC_FREQ / 16 / UART_BAUD) - 1 ) /256;	//calculate and assign the higher byte for SPBRGH
	SPBRG = ((_OSC_FREQ / 16 / UART_BAUD) - 1)%256 ;	// Configure the baud rate.
	TXSTAbits.SYNC = 0;
	RCSTAbits.SPEN = 1;	// Enable serial port.
	RCSTAbits.CREN = 1;	// Enable reception.
	TXSTAbits.TXEN = 1;	// Enable transmission.
	dummy = RCREG;		// dummy read, to clear the receive buffer
	dummy = RCREG; 
}

void uart_transmit(unsigned char uc_data)
{
	// Wait until the transmit buffer is ready for new data.
	while (PIR1bits.TXIF == 0);
	
	// Transmit the data.
	TXREG = uc_data;
}

void Timer5_Int (void)
{
    T5CONbits.T5SEN = 0;
    T5CONbits.RESEN = 0;
    T5CONbits.T5MOD = 0;
    T5CONbits.T5PS = 0b00;
    
    T5CONbits.TMR5CS = 0;
    T5CONbits.T5SYNC = 0;
       
}

void Timer5_Start (unsigned short PR_value)
{
    INTCONbits.GIEH =1;
    INTCONbits.PEIE =1;
    IPR3bits.TMR5IP =0;
    PIE3bits.TMR5IE =1;
    
    TMR5 = PR_value;
    
    T5CONbits.TMR5ON =1;
}

void PCPWM_Int(void)
{
   PTCON0bits.PTOPS = 0b0000;
    
    //PWM Time base output prescaler
    PTCON0bits.PTCKPS = 0b00;
    
    //PWM time base mode: 00 Free Running Mode 01 Single Shot Mode 
    //10 Continuous Up/Down Counting mode 11 Continuous Up/Down with double updates Counting mode
    PTCON0bits.PTMOD = 0b00;
    
    //PWM Time Base timer Enable bits
    PTCON1bits.PTEN = 0;
    
    //PWM Time Base timer count direction
    PTCON1bits.PTDIR = 0;
    
    //PWM Module enable bits
    PWMCON0bits.PWMEN = 0b101;
    
    //PWM output pair mode (0 complementary/1 Independent)
    PWMCON0bits.PMOD0 = 0;//(PWM0,PWM1)
    PWMCON0bits.PMOD1 = 0;//(PWM2,PWM3)
    PWMCON0bits.PMOD2 = 0;//(PWM4,PWM5)
    PWMCON0bits.PMOD3 = 0;//(PWM6,PWM7)
    
    //PWM Special Event Trigger, PWM Update Disable, PWM Output Override Synchronization
    PWMCON1bits.SEVOPS = 0b0000; //PWM Special Event Trigger postscaler bit
    PWMCON1bits.SEVTDIR = 0; //PWM Special Event Trigger count direction
    
    PWMCON1bits.UDIS = 0;//update for duty cycle and period buffer is enabled 
    PWMCON1bits.OSYNC = 0; // Output overrides via OVDCON is synchronized
    
    PTPERH = 0x0f;
    PTPERL = 0xff;
    
    PDC0H=0x2f;
    PDC0L=0xfe;
    
    PDC1H=0x05;
    PDC1L=0x00;
    
    PDC2H=0x05;
    PDC2L=0x00;
    
    PDC3H=0x05;
    PDC3L=0x00;
    
    OVDCOND=0b11111111;
    OVDCONS=0b11111111;
    
    //Dead time control
    DTCONbits.DTAPS = 0b00;
    DTCON = 0b01111111;

    //Interrupt
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    IPR3bits.PTIP = 1;
    PIE3bits.PTIE = 1;
    
    PTCON1bits.PTEN = 1;
} 




