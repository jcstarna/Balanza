////////////////////////////////////////////////////////////////////////////////
////                          DISPLAY
//                            VARIABLES
////FECHA:31/07/2012
////REVISION: 1.00
////AUTOR:CRTARG
////////////////////////////////////////////////////////////////////////////////

//******************************************************************************
//  NOTAS REVISIONES:
//
/*
Ver: 1.00
   Version inicial
*/

#Define pAIN1       PIN_A0            //ENTRADA ANALOGICA

#Define pDIN1       PIN_A2            //ENTRADA DIGITAL 1
#Define pDIN2       PIN_A3            //ENTRADA DIGITAL 2
#Define pDIN3       PIN_A4            //ENTRADA DIGITAL 3
#Define pDIN4       PIN_A5            //ENTRADA DIGITAL 4

#Define pIORB0      PIN_B0
#define pIORB1      PON_B1

#Define pData       PIN_B2            //DATA DEL REGISTRO DE DESP
#Define pEN         PIN_B3             //ENABLE DEL REGISTRO
#define pRCK        PIN_B4             //REGISTER CLOCK
#Define pSCK        PIN_B5             //CLOCK DEL REGISTRO
#Define pCLR        PIN_B6             //CLEAR DEL REGISTRO

#Define pRele1      PIN_C0             //RELE 1
#Define pRele2      PIN_C1             //RELE 2
#Define pBuzzer     PIN_C2             //BUZZER

#define pRX          PIN_C7

int const k100ms=100;
int const k10ms=10;
int const k05s=20;//50
int const k06s=60;
int const kContTimeOut=5;

int const kmBit=150; //400us
int16 const k1bit=400; //400us

//char BCD_7SEG[16]= {0x03,0x9F,0x25,0x0D,0x99,0x49,0x41,0x1F,0x01,0x19};
char BCD_7SEG[16]= {0xF6,0xC0,0x7A,0xF8,0xCC,0xBC,0xBE,0xE0,0xfE,0xFC,0x00};

struct strDigitos{
   int8 digito5;
   int8 digito4;
   int8 digito3;
   int8 digito2;
   int8 digito1;
} digitos;


//CONSTANTES
INT CONST kDelayIni=5;
int const kDelayFin=5;

//VARIABLES
int rDelayIni=kDelayIni;
int f10ms=k10ms;
int r100ms=k100ms;
int16 r500ms=k05s;
int16 r600ms=k06s;
int16 numero=0;
//Tiempos
int rTmr10ms=0;//contador para llegar a 10ms
int rTmr05s=0; //contador para llegar a 500ms
int rTiempos=0;//registros con bits de tiempo
int rBTAl=1;   //Base de tiempo para alarma  
int rAuxBTAl=0;//auxiliar

int8 rContTimeOut= kContTimeOut;

INT rSpTmrBus=5;  //tiempo de inact del bus, depende del baud rate
int rMuxDigito=0; //digito a cargar

int8 rDisplayModel=0;   //0 - display moretti (2400,7,n,1), 1 Display gamma (9600,8,n,1)
int8 rDmil=0;
int8 rUmil=0;
int8 rCent=0;
int8 rDec =0;
int8 rUni=0;
int8 rDispFun=0;  //funcion del display
int8 etapa=0;
int8 rDmilPrev=0;
//FLAGS
int1  fIni=0;
int1  fContar=0; ///habilita contar tiempo
int1  fpIni=0;    //para detectar flanco positivo
int1  fActDisp=0;
int1  finbyte=0;

char rcvchar=0;
char rxByte=0;
int8 bitCount=0;
//prototipos
char get_c_sw(void);
void resetTimeOut(void);
