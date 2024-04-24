///////////////////////////////////////////////////////////////////////
//                FUNCIONES SCAN ENTRADAS
//
//Llamar periodicamente desde un timer
//las inmunidades estan dadas en cantidad de llamadas
//
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------
// CONSTANTES
//--------------------------------------------------------------------

int const kInmuni=10;

//--------------------------------------------------------------------
// VARIABLES
//--------------------------------------------------------------------

int8  rInmuniT1=10;
int8  rInmuniT2=10;
int8  rInmuniT3=10;
int8  rInmuniT4=10;
int8  rInmuni1T1=40;
int8  rInmuni1T2=40;
int8  rInmuni1T3=40;
int8  rInmuni1T4=40;
int8  rInmuni2T1=40;
int8  rInmuni2T2=40;
int8  rInmuni2T3=40;
int8  rInmuni2T4=40;
int8  rMaskInputs=0B00111100; //mascara para entradas

//--------------------------------------------------------------------
// FLAGS DE ENTRADAS
//--------------------------------------------------------------------
int1  fIn1=0;  //entrada confirmada superado filtro de tiempo
int1  fIn2=0;
int1  fIn3=0;
int1  fIn4=0;
int1  fIn1C=0;  //entrada confirmada pulso corto
int1  fIn2C=0;
int1  fIn3C=0;
int1  fIn4C=0;
int1  fIn1L=0;  //entrada confirmada pulso largo
int1  fIn2L=0;
int1  fIn3L=0;
int1  fIn4L=0;


//++++++++++++++++++ ESCANEO DE ENTRADAS ++++++++++++++++++++++++++++++
void  ScanInputs(){
   int rTempTec;                          //Temporal teclado
   rTempTec=input_A();
   rTempTec=rTempTec & rMaskInputs;
   
   if (bit_test(rTempTec,5)){
      if(--rInmuniT4==0 && !fIn4){
         rInmuniT4=kInmuni;
         fIn4=1;
      }
   }
   else{
      rInmuniT4=kInmuni;
      fIn4=0;
   }
   
   if (bit_test(rTempTec,4)){
      if(--rInmuniT3==0 && !fIn3){
         rInmuniT3=kInmuni;
         fIn3=1;
      }
   }
   else{
      rInmuniT3=kInmuni;
      fIn3=0;
   }
   
   if (bit_test(rTempTec,3)){
      if(--rInmuniT2==0){
         rInmuniT2=kInmuni;
         fIn2=1;
      }
   }
   else{
      rInmuniT2=kInmuni;
      fIn2=0;
   }  
   
   if (bit_test(rTempTec,2)){
      if(--rInmuniT1==0 && !fIn1){
         rInmuniT1=kInmuni;
         fIn1=1;
      }
   }
   else{
      rInmuniT1=kInmuni;
      fIn1=0;
   }   
}
