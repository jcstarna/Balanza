////////////////////////////////////////////////////////////////////////////////
////                   DISPLAY 
////FECHA:29/05/2019
////REVISION: 1.00
////AUTOR:CRTARG
////////////////////////////////////////////////////////////////////////////////

//******************************************************************************
//  NOTAS REVISIONES:
//
/*
Ver: 1.00
   Version inicial
Ver: 2.01
 * Version con 3 protocolos seleccionables por jumper
Ver: 2.02
 * Agregado de protocolo para balanza YAOHUA
Ver: 2.03
 * Agregado de protocolo para balanza INGAPSA 768
*/
//OPCIONES DE CONFIGURACION
//#DEFINE useModbus
#define useRXString
#define useCustom

#include <16F1713.h>
#fuses XT,NOWDT,noprotect,NOLVP,NOBROWNOUT,NODEBUG,PUT
#use delay(clock=16000000)
#pragma use fast_io(A)
#pragma use fast_io(B)
#pragma use fast_io(C)
//#pin_select 
//#use rs232(baud=2400, BITS=8, xmit=PIN_C6, rcv=PIN_C7, enable=PIN_C5, ERRORS,STREAM=COMM)// RS232 Estándar
#pin_select U1RX = PIN_C7
#pin_select U1TX = PIN_C6
//#use rs232(UART1)
#use rs232(baud=2400, BITS=8, xmit=PIN_C6, rcv=PIN_C7, enable=PIN_C5, ERRORS)// RS232 Estándar

#pin_select SDA1IN = PIN_C4
#pin_select SCL1OUT = PIN_C3
#use I2C(master, sda=PIN_C4, scl=PIN_C3, force_hw, fast=100000,I2C1)
//valores por defecto de la EEPROM
//#rom 0x2100={01,10,10,10,10,10,10,10,10,0,0}
#include <DISPLAY.h>
// librerias modbus
#ifdef useModbus
   #include "modbus.h"
   #include "modbus.c"
#endif
// recepcion de texto Ej, balanzas
#ifdef useRXString
   #include "txtProtocol.c"
#endif


//#include <ds1307.c>
//#include <inputs.c>

void envia_disp(void);
void Esc_EEPROM(void);
void Pantallas (int nroPantalla);
void funcTeclado (int nroPantalla);

// INTERRUPCIONES /////////////////////////////////////////////////////////////
#INT_RDA
void RX_isr(){
char rcvchar=0x00;
   if(kbhit()){                           // Si hay algo pendiente de recibir ...
      rcvchar=getc();                     // lo descargo y ...
      rxTXTPprot(rcvchar);
   }
   //setup_timer_2 (T2_DIV_BY_64, 250, 16); //setup_timer_2 (T2_DIV_BY_4, 250, 16);
    resetTimeOut();
}


#int_TIMER2
void TMR2_isr(){
    if (--rContTimeOut == 0)
    {
        inicbuffRX();
        rContTimeOut= kContTimeOut;
    }
}


#INT_TBE
void TX_isr(){

}


#INT_TIMER0
void RTCC_isr() {          // Interrupción Timer 0
SET_TIMER0(15);
//ScanInputs();              //escanea entradas digitales
if (--r500ms==0){
   r500ms=k05s;
   fActDisp=1;
}
}

//Funcion para reset tiempo de time out comunicacion
void resetTimeOut(void)
{
    rContTimeOut= kContTimeOut;
    setup_timer_2 (T2_DIV_BY_64, 250, 16);
}


//descompone un numero en unidad de mil, centena, decena y unidad
//retorna esos valores en variables globales rUMil, rCent, rDec, rUni
void Dec_A_UDCU(int16 nro){
rDMil = nro/10000;
rUMil = nro/1000 - rDMil*10;
rCent = nro/100 - rUMil*10;
rDec  = nro/10 - rCent * 10 - rUmil *100;
rUni  = nro - rUmil * 1000 - rCent *100 - rDec * 10;
}

////////////////////////////////////////////////////////
//                         MAIN
////////////////////////////////////////////////////////
void Main()
{
INT temp_A=0;
//++++++++++++++++++Configura puertos analogicos++++++++++++++++++++++++++++++
setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);  //interrumpe cada 1.024ms
setup_timer_2(T2_DIV_BY_64,0xF0,10);       //control de inactividad del bus
setup_adc_ports( ADC_OFF );               //no analogicas
setup_comparator(NC_NC_NC_NC);            //no comparadores
setup_ccp1(ccp_off);
setup_ccp2(ccp_off);
//setup_ccp3(ccp_off);

//Configuracion de puertos 
set_tris_a(0B11111111);
set_tris_b(0X00);
set_tris_c(0B10011000);

output_a(0);
output_b(0);
output_c(0);

//Inicio Variables
rTmr10ms=k10ms;
rTmr05s=k05s;
r600ms=k06s;

DELAY_MS(500);
temp_A= input_a();
temp_A= temp_A/4;
//VEO QUE PROTOCOLO VOY A USAR
rDisplayModel = temp_A;
   
////seteo interrupciones
   if(rDisplayModel != 0 ) //RX por HARD
      enable_interrupts(INT_RDA);
   else
      disable_interrupts(INT_RDA);  //RX por SOFT
      
   enable_interrupts(int_TIMER2);                  // Habilita Interrupción TIMER2   
   //enable_interrupts(INT_TIMER0);   
   //enable_interrupts(global);   
   SET_TIMER0(15); //PRECARGO
   rDispFun=0;
   output_low(pEn);
   output_high(pCLR);
//   ds1307_init();


DELAY_MS(500);
///////////////////////////////////////////////////////////////////////////////
// Mensaje de inicio a 9600,8,N,1
setup_uart(9600);
printf("Controlarg Display Balanza\r\n");
printf("Fw Ver:2.03 \r\n");
printf("Switch %d \r\n",temp_A);
switch (rDisplayModel)
{
   case 0: //moretti (2400,7,n,1)
      printf("Protocolo 2400,7,N,1 M\r\n");
      setup_uart(false);
      break;
   case 1: //gamma. (9600, 8 , N,1)
      printf("Protocolo 9600,8,N,1 G\r\n");
      setup_uart(9600);
      break;
   case 2: //gamma. (9600, 8 , N,1)
      printf("Protocolo 9600,8,N,1 G2\r\n");
      setup_uart(9600);
      break;  
   case 3: //Balanza YAOHUA (1200,9,N,1)
      setup_uart(1200);   
      printf("Protocolo 1200,8,N,1 YA\r\n");
      break;
   case 4: //Balanza INGAPSA (1200,8,N,1)
      setup_uart(1200);   
      printf("Protocolo 1200,8,N,1 IN\r\n");
      break;
   default:
      printf("No implementado");
      break;      
}

DELAY_MS(500);

//test de leds
   digitos.digito5=BCD_7SEG[8];
   digitos.digito4=BCD_7SEG[8];
   digitos.digito3=BCD_7SEG[8];
   digitos.digito2=BCD_7SEG[8];
   digitos.digito1=BCD_7SEG[8];
   envia_disp();
DELAY_MS(1500);
fActDisp=1;
resetTimeOut();
   enable_interrupts(global); 

///////////////////////////////////////////////////////////////////////////////
// Bucle ppal
///////////////////////////////////////////////////////////////////////////////

int8 i = 0;
while(true){
   if(rDisplayModel==0)
   {
         rcvchar = get_c_sw();
         if (finbyte)
         {  
            finbyte=0;
            rxTXTPprot(rcvchar);
         }
      //si llego toda la trama la proceso
         if(fDataOk)
         {
            rxInterpreter();
            inicbuffRX();
            fActDisp=1;
            fDataOk=0;
         }
   }
   else
   {
        if(fDataOk)
            {
                rxInterpreter();
                fDataOk=0;
            }
   }
   
   //si hay que actualizar el display, se actualiza
   if(fActDisp){
         fActDisp=0;
         if (rDMil == 0) rDMil=11;
         if (rUMil == 0 && rDMil == 11)rUMil = 11;
         if (rCent == 0 && rUMil == 11)rCent = 11;
         if (rDec ==0 && rCent == 11) rDec =11;
         digitos.digito5=BCD_7SEG[rDMil];
         digitos.digito4=BCD_7SEG[rUMil];
         digitos.digito3=BCD_7SEG[rCent];
         digitos.digito2=BCD_7SEG[rDec];
         digitos.digito1=BCD_7SEG[rUni];
         envia_disp();
   } 
}
}
//////////////////////////////////////////////////////


//envio todos los datos al display
//17 bytes
void envia_disp(void){
int8 i=0;      //offset del puntero a enviar
int8 rbit=0;   //contador de bits
int8 Data=0;   //dato a enviar
//cargo el puntero con byte a enviar
   for (i=0;i<=4;i++){//16
   Data=*(&digitos.digito5+i);   //cargo el primer digito a enviar en DATA
   //envio bit a bit el dato
      for (rbit=0;rbit<=7;rbit++){
         if(bit_test(Data,0))
            output_high(pDATA);
         else
            output_low(pDATA);
            
         rotate_right(&Data,1);
         delay_us(1);
         output_high(pSCK);  //CLOCK EN ALTO
         delay_us(1);
         output_LOW(pSCK);  //CLOCK EN BAJO
      }
   }
output_high(pRCK);
delay_us(1);
output_low(pRCK);
output_high(pDATA);
}



char get_c_sw(void)
{
switch (etapa)   
   {
      case 0:  //star bit
            if(!input(pRX))   //start bit
            {
               rcvchar=0;
               rxByte=0;
               //output_low(pin_b0);
               delay_us(kmBit);
               etapa=1;
            }
            break;
      case 1:
            if(!input(pRX))   //confirmacion de bit de start
            {
               delay_us(k1Bit);
               etapa=2;
            }
            break;
      case 2:  //bytes
            for(bitCount=0;bitCount <= 6 ;bitCount++)
            {
               //if (input(pRX))
               //    output_high(pin_b0);
               // else
               //     output_low(pin_b0);
                    
               shift_right(&rxByte,1,input(pRX)); 
               delay_us(k1bit);
            }
            etapa=3;
            break;
      case 3:  //stopBit
             //if (input(pRX))
             //      output_high(pin_b0);
             //else
             //    output_low(pin_b0);
            if(input(pRX))
            {
               rotate_right(&rxByte,1);
               etapa=0;
               finbyte=1;
               //setup_timer_2 (T2_DIV_BY_64, 250, 16); //setup_timer_2 (T2_DIV_BY_4, 250, 16);
               resetTimeOut();
               return (rxByte);
            }
            break;
      case 5:
            etapa=0;
            return 0xFF;
            break;
      default:
            etapa=0;
            return 0xff;
            break;
   }  
}

///////////////////////////////////////////////////////////////////////////////
// SETUP DE LA USART PARA BALANZA MORETTI
///////////////////////////////////////////////////////////////////////////////

