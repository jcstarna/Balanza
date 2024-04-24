
int8  rSegundos=0;
int8  rMinuto=0;
int8  rHora=0;
int8  rUni_Sec=0;
int8  rDec_Sec=0;
int8  rUni_Min=0;
int8  rDec_Min=0;
int8  rUni_HS=0;
int8  rDec_HS=0;

VOID ds1307_init(void){
int8 seconds=0;
   i2c_start();
   i2c_write(0xD0);
   i2c_write(0x00);
   i2c_start();
   i2c_write(0xD1);
   seconds = i2c_read(0);
   i2c_stop();
   seconds &= 0x7F;

   delay_us(3);

   i2c_start();
   i2c_write(0xD0);
   i2c_write(0x00);
   i2c_write(seconds);
   i2c_start();
   i2c_write(0xD0);
   i2c_write(0x07);
   i2c_write(0);
   i2c_stop();
}

void ds1307_read (void){
int8 rTempDS=0;
   I2C_Start();
   I2C_Write(0xd0); 
   i2c_write(0x00);
   I2C_Start();
   i2c_write(0xd1);
   rTempDS=i2c_read();//leo segundos 
   rUni_SEC= rTempDs & 0x0f;
   rDec_SEC= swap(rTempDS)& 0x0f; 
   rSegundos=(rDec_Sec*10)+rUni_Sec;//asumo que no es en BCD
   rTempDS=i2c_read();//leo minutos 
   rUni_Min= rTempDs & 0x0f;            
   rDec_Min= swap(rTempDS) & 0x0f;
   rMinuto= rDec_Min;
   rMinuto= rMinuto *16 + rUni_Min;
   rTempDS=i2c_read(0);//leo horas 
   rUni_HS= rTempDs & 0x0f;            
   rDec_HS= swap(rTempDS) & 0x03; 
   rHora= rDec_HS *16 + rUni_Hs;
   i2c_stop();
   //rTime= rHora << 8;
   //rTime=rTime + rMinuto;
}

void ds1307_Write_time(int8 sec, int8 min, int8 hs){
   i2c_start();
   i2c_write(0xD0);
   i2c_write(0x00);
   i2c_write(sec);
   //i2c_start();
   i2c_write(min);
   //i2c_start();
   i2c_write(hs);   
   i2c_start();
   i2c_write(0xD0);
   i2c_write(0x07);
   i2c_write(0);
   i2c_stop();
}
