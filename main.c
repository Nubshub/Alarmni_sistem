/*
 * File:   main.c
 * Author: Q
 *
 * Created on January 23, 2022, 8:18 PM
 */


#include "xc.h"
#include "tajmeri.h"
#include "driverGLCD.h"
#include "adc.h"
#include "slike.h"
#include <stdio.h>
#include <stdlib.h>
#include<p30fxxxx.h>

_FOSC(CSW_FSCM_OFF & XT_PLL4);//instruction takt je isti kao i kristal
_FWDT(WDT_OFF);
_FGS(CODE_PROT_OFF);

enum state {init, ready, brojac, aktivan, sirena, vrata, z_vrata };
enum state stanje = init;

unsigned int sirovi0,sirovi1,stoperica, i;


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
    ADPCFGbits.PCFG8 = 1; //digitalni
    ADPCFGbits.PCFG9 = 1; //digitalni
    ADPCFGbits.PCFG12 = 1; //digitalni
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
                if(sirovi0 > 3300)//fotootpornik
                stanje = brojac;
                //implementirati touch screen aktiviraj
                break;
                
            case brojac:

                GLCD_DisplayPicture(tri); 
                Delay_ms(1000);
                GLCD_DisplayPicture(dva);
                Delay_ms(1000);
                GLCD_DisplayPicture(jedan);
                Delay_ms(1000);

                stanje = aktivan;
                break;
                
            case aktivan:
                GLCD_DisplayPicture(aktiviran);
        
                if(sirovi1 > 2500)//senzor za dim
                {   
                    GLCD_DisplayPicture(dim);
                    Delay_ms(2000);
                    stanje = vrata;
                }
                else if(PORTBbits.RB7 == 1)//senzor pokreta
                 {
                    GLCD_DisplayPicture(pokret);
                    Delay_ms(2000);
                    stanje = sirena;
                 }
                break;
                
            case vrata:
                    
                    for(i = 0; i<10; i++)
                    {
                        LATBbits.LATB6 = 1;
                        Delay_ms(2);
                        LATBbits.LATB6 = 0;
                        Delay_ms(18);
                    }
                    stanje = z_vrata;
                break;
            case z_vrata:
                    GLCD_DisplayPicture(zatvori_vrata);
                break;
                
            case sirena:
                GLCD_DisplayPicture(ugasi_sirenu);
                
                //touch screen zatvori vrata
                //touch screen ugasi sirenu
                break;
               
                
        
        }

    }
    
    
    
    return 0;
}
