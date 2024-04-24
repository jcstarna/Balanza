////////////////////////////////////////////////////////////////////////////////
////                    IMPLEMENTACION PROTOCOLOS VARIOS
////                    
////FECHA:  31/05/19
////REVISION: 0
////AUTOR:  CONTROLARG
////
////////////////////////////////////////////////////////////////////////////////
//revision 22/09/21
/* se agrego interprete para balanza moretti
*  se agrego interprete para balanza gamma
*  se agrego select para fin de trama segun configuracion.
*/

#include "string.h"
//CONSTANTES
int const   lenbuff=40;
int const   kcFinTramaM=0x0A;
int const   kcFinTramaG=0x0D;
int const   kcOD=0x0D;    //fin de trama del protocolo gamma
int const   kEOT=0x03;    //standard fin de texto  
int const   kSTX=0x02;    //standard star of text
char  kstrFinTrama[] = "\r\n";   //fin de trama del protocolo moretti

//DECLARACION FUNCIONES
void inicbuffTx(void);                   // Borra buffer
void inicbuffRx(void);                   // Borra buffer
void rxTXTPprot(char);                   // funcion de recepcion
void rxInterpreter(void);

//VARIABLES
char  Txbuff[lenbuff];                   // Buffer
char  Rxbuff[lenbuff];                   // Buffer
int   rxPuntero=0x00;                    // Índice: siguiente char en cbuff
char  calcCheck=0;
char  rxCheck=0;

//FLAGS
INT1  fDataOk=0;

//prototipos
char calc_XOR(char *buffer[], int len);

//FUNCIONES
void inicbuffRX(void){                   // Inicia a \0 cbuff -------------------
   int i;
   for(i=0;i<lenbuff;i++){               // Bucle que pone a 0 todos los
      Rxbuff[i]=0x00;                    // caracteres en el buffer
   }
   rxPuntero=0x00;                       // Inicializo el indice de siguiente
}

void inicbuffTX(void){                   // Inicia a \0 cbuff -------------------
   int i;
   for(i=0;i<lenbuff;i++){               // Bucle que pone a 0 todos los
       txbuff[i]=0;
   }
   rxPuntero=0x00;                       // Inicializo el indice de siguiente
}

//RUTINA DE RECEPCION
void rxTXTPprot(char Data){
    int1 finTrama=0;
    int i = 0;
    switch(rDisplayModel)
    {
        case 0: //display Moreti 
            if(Data == kcFinTramaM) finTrama=1;
            break;
        case 1: //display gamma 1 ()
            if(Data == kcFinTramaG) finTrama=1;
            break;
        case 2: //Display gamma2 (ST,GS,0000000Kg /r/n)
            if(Data == kcFinTramaG) finTrama=1;
            break;
        case 3: //display YAOHUA
            if(Data == kEOT) finTrama=1;
            break;
        case 4: //display INGAPSA
            if(Data == kcOD) finTrama=1;
            break;
        default:
            break;            
    }
    /*
    if (rDisplayModel==0)
    {
        if(Data == kcFinTramaM) finTrama=1;
    }
    else
    {
        if(Data == kcFinTramaG) finTrama=1;
    }
     */ 
   //verificar que el caracter sea el de fin de trama
   if (finTrama)
   {
      rxBuff[rxPuntero]=Data;
      fDataOk=1;
      numero++;
   }
   //no es, carga en buffer de recepcion.
   else{
      rxBuff[rxPuntero]=Data;
      rxPuntero++;
   }
}

//rutina para interpretar lo recibido
/* balanza gamma
*  0x45 ?? no si es constante
*  peso 6 digitos
*  0x0D
* 
* balanza moretti
* 0x02
* 0x31
* 0x30
* 0x20
* peso 12 digitos
* 0D 0A
*/
void rxInterpreter(void){
//protocolo
    switch (rDisplayModel)
    {
      case 0:// moretti
      if (strstr(rxBuff,kstrFinTrama) != NULL)
      {  
      if (rxBuff[0] == 0x02)
         {
            if(rxBuff[rxPuntero] == 0x0A)
            {
               //NUMERO++; 
               rUni=rxBuff[rxPuntero-8]-0x30; 
               rDec=rxBuff[rxPuntero-9]-0x30; 
               rCent=rxBuff[rxPuntero-10]-0x30; 
               rUMil=rxBuff[rxPuntero-11]-0x30;
               rDMil=rxBuff[rxPuntero-12]-0x30; 
            }
         }
      }
      break;   //protocolo gamma "i000000/n"
     case 1:
        if(rxBuff[rxPuntero] == 0x0D)
            {
                   rUni=rxBuff[rxPuntero-1]-0x30;
                   if ((rUni > 9) || (rUni <0))
                      return;
                   rDec=rxBuff[rxPuntero-2]-0x30;
                   if ((rDec > 9) || (rDec <0))
                       return;
                   rCent=rxBuff[rxPuntero-3]-0x30;
                   if ((rCent > 9) || (rCent <0))
                       return;
                   rUMil=rxBuff[rxPuntero-4]-0x30;
                   if ((rUmil > 9) || (rUMil <0))
                       return;
                   rDMil=rxBuff[rxPuntero-5]-0x30;
                   if ((rDMil > 9) || (rDMil <0))  
                       return;
                   if (rDMilPrev != rDMil)
                   {
                       rDMilPrev = rDMil;
                       return;
                   }
                   //inicbuffRX();
                   fActDisp=1;
            }
     break;
        case 2: //version gamma "ST,GS,000000Kg 0x0D"    
            if(rxBuff[rxPuntero] == 0x0D)
                {
                   rUni=rxBuff[rxPuntero-3]-0x30;
                   if ((rUni > 9) || (rUni <0))
                      return;
                   rDec=rxBuff[rxPuntero-4]-0x30;
                   if ((rDec > 9) || (rDec <0))
                       return;
                   rCent=rxBuff[rxPuntero-5]-0x30;
                   if ((rCent > 9) || (rCent <0))
                       return;
                   rUMil=rxBuff[rxPuntero-6]-0x30;
                   if ((rUmil > 9) || (rUMil <0))
                       return;
                   rDMil=rxBuff[rxPuntero-7]-0x30;
                   if ((rDMil > 9) || (rDMil <0))
                       return;
                   if (rDMilPrev != rDMil)
                   {
                       rDMilPrev = rDMil;
                       return;
                   }                   
                   //inicbuffRX();
                   fActDisp=1;
                }
        break;
        case 3: //trama YAOHUA 0x02 0x2B
            //VERIFICO INICIO DE TRAMA 0x02
            if(rxBuff[0] == kSTX){
                //inicio de trama 0x02
                //calculo checksum
                calcCheck = calc_XOR(&rxBuff[0],8);
                //retiro chk del buffer
                rxCheck = (rxBuff[rxPuntero-2] & 0x0f)<<4 | (rxBuff[rxPuntero-1] & 0x0f);
                if (rxCheck == calcCheck)
                {
                   rUni=rxBuff[rxPuntero-4]-0x30;
                   if ((rUni > 9) || (rUni <0))
                      return;
                   rDec=rxBuff[rxPuntero-5]-0x30;
                   if ((rDec > 9) || (rDec <0))
                       return;
                   rCent=rxBuff[rxPuntero-6]-0x30;
                   if ((rCent > 9) || (rCent <0))
                       return;
                   rUMil=rxBuff[rxPuntero-7]-0x30;
                   if ((rUmil > 9) || (rUMil <0))
                       return;
                   rDMil=rxBuff[rxPuntero-8]-0x30;
                   if ((rDMil > 9) || (rDMil <0))
                       return;
                   //printf("Buff: %2X, %2X \r\n",calcCheck  ,rxCheck );
                   fActDisp=1;
                }
                
            }
        case 4: //trama INGAPSA
                //Pxxxxxx^UU^M^SS^CR
                //Donde: P es ? (hex 2D) si el peso es negativo o un espacio (hex 20) si el peso es positivo xxxxxx es el valor del peso en ASCII ^ es un espacio ( hex 20)
                //UU son las unidades KG ( hex 4B, 47) SS si existe Movimiento, es MO ( hex 4D, 4F );
                //si no existeMovimiento es doble espacio (hex 20,20) CR es Carry Return ( hex 0D) M es una G (hex 47)
            if(rxBuff[rxPuntero] == 0x0D)
                {
                   rUni=rxBuff[6]-0x30;
                   if ((rUni > 9) || (rUni <0))
                      return;
                   rDec=rxBuff[5]-0x30;
                   if ((rDec > 9) || (rDec <0))
                       return;
                   rCent=rxBuff[4]-0x30;
                   if ((rCent > 9) || (rCent <0))
                       return;
                   rUMil=rxBuff[3]-0x30;
                   if ((rUmil > 9) || (rUMil <0))
                       return;
                   rDMil=rxBuff[2]-0x30;
                   if ((rDMil > 9) || (rDMil <0))
                       return;
                   if (rDMilPrev != rDMil)
                   {
                       rDMilPrev = rDMil;
                       return;
                   }                   
                   inicbuffRX();
                   fActDisp=1;
                }
        break;      
    }
}


/*
 * Calcula el checksum de un buffer
 * haciendo el XOR de cada elemento
 * Devuelve el resulado en un char que tiene
 * en el nible alto un caractar y en el 
 * bajo, otro caractar
 */
char calc_XOR(char *buffer[], int len)
{
    char chksum=0;
    char temp=0;
    for(int i=1;i<=len;i++)
    {
        chksum = chksum ^ *(buffer+i);
        //printf("Buff: %2X \r\n",*(buffer+i));
    }
    return chksum;
}


/*
      for (int i = 0;i<rxPuntero; i++)
      {
        printf("Buff: %2X \r\n",rxBuff[i]);
      }
 */