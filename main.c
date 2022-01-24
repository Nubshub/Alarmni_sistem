/*
 * File:   main.c
 * Author: Q
 *
 * Created on January 23, 2022, 8:18 PM
 */


#include "xc.h"
#include "tajmeri.h"
#include "driverGLCD.h"
#include <stdio.h>
#include <stdlib.h>
#include<p30fxxxx.h>

_FOSC(CSW_FSCM_OFF & XT_PLL4);//instruction takt je isti kao i kristal
_FWDT(WDT_OFF);
_FGS(CODE_PROT_OFF);

enum state {init, ready, brojac, aktivan, sirena, vrata };
enum state stanje = init;

unsigned int sirovi0,sirovi1,stoperica, i;


unsigned char const aktiviraj[1024] = {
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255, 
 255, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 
  31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 
  31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 
  31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 
  31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 
  31, 31, 31, 31, 31, 31, 31, 31, 31,255,255,255,255,255,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255, 
 255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 32, 32,192, 
   0,  0,  0,  0,224,  0,  0,  0,128, 64, 32,  0, 32, 32, 32,224, 
  32, 32, 32,  0,  0,224,  0,  0, 96,128,  0,  0,  0,  0,  0,  0, 
 128, 96,  0,224,  0,  0,  0,224, 32, 32, 32, 32, 64,128,  0,  0, 
   0,  0,192, 32, 32,192,  0,  0,  0,  0,  0,  0,224,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255,255,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255, 
 255,  0,  0,  0,  0,  0,  0,  0,  0,192, 56, 22, 17, 16, 16, 17, 
  22, 56,192,  0,255,  4, 10, 17, 32, 64,128,  0,  0,  0,  0,255, 
   0,  0,  0,  0,  0,255,  0,  0,  0,  1, 14, 48,192,192, 48, 14, 
   1,  0,  0,255,  0,  0,  0,255,  4,  4,  4, 12, 50,193,  0,192, 
  56, 22, 17, 16, 16, 17, 22, 56,192,128,128,128,127,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255,255,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255, 
 255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255,255,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255, 
 255,248,248,248,248,248,248,248,248,248,248,248,248,248,248,248, 
 248,248,248,248,248,248,248,248,248,248,248,248,248,248,248,248, 
 248,248,248,248,248,248,248,248,248,248,248,248,248,248,248,248, 
 248,248,248,248,248,248,248,248,248,248,248,248,248,248,248,248, 
 248,248,248,248,248,248,248,248,248,248,248,248,248,248,248,248, 
 248,248,248,248,248,248,248,248,248,255,255,255,255,255,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 
};

/*******************************************************************
//funkcija za kasnjenje u milisekundama
*********************************************************************/
void Delay_ms (int vreme)//funkcija za kasnjenje u milisekundama
	{
		stoperica = 0;
		while(stoperica < vreme);
	}



void __attribute__ ((__interrupt__)) _T2Interrupt(void) // svakih 1ms
{

	TMR2 =0;

    stoperica++;//brojac za funkciju Delay_ms

	IFS0bits.T2IF = 0; 
       
}

void __attribute__((__interrupt__)) _ADCInterrupt(void) 
{
							

										sirovi0=ADCBUF0;
										sirovi1=ADCBUF1;
										
										

    IFS0bits.ADIF = 0;

} 

int main(void) {
    
    ADCinit();
    Init_T2();
    ADCON1bits.ADON=1;//pocetak Ad konverzije 
    
    //pin za PIR senzor
    ADPCFGbits.PCFG7 = 1; //digitalni
    TRISBbits.TRISB7 = 1; //ulaz
    
    //pin za servo motor
    ADPCFGbits.PCFG6 = 1; //digitalni
    TRISBbits.TRISB6 = 0; //izlaz
    
    //test pinovi stanja
    ADPCFGbits.PCFG7 = 1; //digitalni
    ADPCFGbits.PCFG8 = 1; //digitalni
    ADPCFGbits.PCFG9 = 1; //digitalni
    ADPCFGbits.PCFG12 = 1; //digitalni
    TRISBbits.TRISB7 = 0; //izlaz
    TRISBbits.TRISB8 = 0; //izlaz
    TRISBbits.TRISB9 = 0; //izlaz
    TRISBbits.TRISB12 = 0; //izlaz
    TRISFbits.TRISF6 = 0; //izlaz
    
    
    Delay_ms(20);
    ConfigureLCDPins();
    Delay_ms(20);
    GLCD_LcdInit();
    Delay_ms(20);
    GLCD_ClrScr();
    
    while(1)
    {
        switch(stanje)
        {
            case init:
                GLCD_DisplayPicture(aktiviraj); 
                stanje = ready;
                break;
                
            case ready:
                LATBbits.LATB7 = 1;
                if(sirovi0 > 3300)
                stanje = brojac;
                //implementirati touch screen aktiviraj
                break;
                
            case brojac:
                
                
                LATBbits.LATB7 = 0;
                Delay_ms(1000);
                //GLCD_DisplayPicture(tri); 
                LATBbits.LATB7 = 1;
                Delay_ms(1000);
                LATBbits.LATB7 = 0;
                Delay_ms(1000);
                //GLCD_DisplayPicture(dva);
                LATBbits.LATB7 = 1;
                Delay_ms(1000);
                LATBbits.LATB7 = 0;
                Delay_ms(1000);
                //GLCD_DisplayPicture(jedan);
                LATBbits.LATB7 = 1;
                Delay_ms(1000);
                LATBbits.LATB7 = 0;
                
                stanje = aktivan;
                break;
                
            case aktivan:
                LATFbits.LATF6 = 1;
                
                if(sirovi1 > 2500)//senzor za dim
                    stanje = vrata;
                
                 if(PORTBbits.RB7 == 1)//senzor pokreta
                    stanje = sirena;
                
                break;
                
            case vrata:
                    LATFbits.LATF6 = 0;
                    for(i = 0; i<10; i++)
                    {
                        LATBbits.LATB6 = 1;
                        Delay_ms(2);
                        LATBbits.LATB6 = 0;
                        Delay_ms(18);
                    }
                    stanje = sirena;
                break;
                
            case sirena:
                LATBbits.LATB6 = 1;
                //touch screen zatvori vrata
                //touch screen ugasi sirenu
                break;
               
                
        
        }

    }
    
    
    
    return 0;
}
