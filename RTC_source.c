#include "Rtc.h"
#include "RFIDSource.c"

 void Initialization()
 {
  ANSELA=0;
  ANSELB=0;
  ANSELC=0;
  ANSELD=0;
  TRISC0_Bit=1;
  TRISC1_Bit=1;
  TRISC2_Bit=1;
  TRISC5_Bit=0;
  RC5_bit=1;
  RFIDEnable=1;
  GrapIDs();
  Keypad_Init();                           // Initialize Keypad
  Lcd_Init();                              // Initialize LCD
  Lcd_Cmd(_LCD_CLEAR);                     // Clear display
  Lcd_Cmd(_LCD_CURSOR_OFF);                // Cursor off  Lcd_Out(1, 1, "1");
  Lcd_Out(1,2,"Pass Your ID");
  UART1_Init(2400);
  Delay_ms(100);
   }
   
   
char checkPassword(){
   for(;;){
    kp = 0;
    kp = Keypad_Key_Click();             // Store key code in kp variable
    if(kp==0) return 0;
   kp=Mask(kp);
    Enter_ID[cnt]=kp;
    Lcd_Chr(2,cnt+1,kp);
    i++;
    cnt++;
       if(cnt>3)
       {
        Lcd_Chr(2,i+1,Enter_ID[cnt]);
        e=strstr(Enter_ID,pass_word);
       if(e!=0)
        {
         correctPassword();
         delay_ms(1000);
         Lcd_Cmd(_LCD_CLEAR);                     // Clear display
         Lcd_Out(1,2,"Pass Your ID");
         return 0;
         }
         else{
         wrongPassword();
         Lcd_Cmd(_LCD_CLEAR);                     // Clear display
         Lcd_Out(1,2,"Pass Your ID");
         return 0;
         }
      }
   }
}

 void correctPassword(){
 cnt=0;
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1,1,"Correct Password");
 delay_ms(1000);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1,1,"Choose Option");
 delay_ms(1000);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1,1,"1 AddCard 3 Back");
 Lcd_Out(2,1,"2 Remove Card");
 readKeypad();
 }
 void wrongPassword(){
  cnt=0;
  i=0;
  Lcd_Cmd(_LCD_CLEAR);                     // Clear display
  Lcd_Out(1,1,"Worng Password");
  delay_ms(1000);
 }

 char readKeypad(){

    kp = 0;                                // Reset key code variable
    do
    kp = Keypad_Key_Click();             // Store key code in kp variable
    while (!kp);
    kp=Mask(kp);
    if(kp=='1'){
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1,"Put The Card");
    delay_ms(1000);
    addCard();
    return 0;
    }
    else if(kp=='2'){
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1,"Put The Card");
    delay_ms(1000);
    removeCard();
    return 0;
    }
    else if(kp=='3'){
    return 0;
 }
 }
   
unsigned short read_ds1307(unsigned short address)
{
  unsigned short r_data;
  I2C1_Start();
  I2C1_Wr(0xD0); //address 0x68 followed by direction bit (0 for write, 1 for read) 0x68 followed by 0 --> 0xD0
  I2C1_Wr(address);
  I2C1_Stop();
  I2C1_Start();
  I2C1_Wr(0xD1); //0x68 followed by 1 --> 0xD1
  r_data=I2C1_Rd(0);
  I2C1_Stop();
  return r_data;
}


void write_ds1307(unsigned short address,unsigned short w_data)
{
  I2C1_Start(); // issue I2C start signal
  //address 0x68 followed by direction bit (0 for write, 1 for read) 0x68 followed by 0 --> 0xD0
  I2C1_Wr(0xD0); // send byte via I2C (device address + W)
  I2C1_Wr(address); // send byte (address of DS1307 location)
  I2C1_Wr(w_data); // send data (data to be written)
  I2C1_Stop(); // issue I2C stop signal
}


unsigned char BCD2UpperCh(unsigned char bcd)
{
  return ((bcd >> 4) + '0');
}


unsigned char BCD2LowerCh(unsigned char bcd)
{
  return ((bcd & 0x0F) + '0');
}
int Binary2BCD(int a)
{
   int t1, t2;
   t1 = a%10;
   t1 = t1 & 0x0F;
   a = a/10;
   t2 = a%10;
   t2 = 0x0F & t2;
   t2 = t2 << 4;
   t2 = 0xF0 & t2;
   t1 = t1 | t2;
   return t1;
}

    //
int BCD2Binary(int a)
{
   int r,t;
   t = a & 0x0F;
   r = t;
   a = 0xF0 & a;
   t = a >> 4;
   t = 0x0F & t;
   r = t*10 + r;
   return r;
}

void Display_Time()
{
      second =read_ds1307(0);
      minute = read_ds1307(1);
      hour = read_ds1307(2);
      hr = hour & 0b00011111;
      ap = hour & 0b00100000;

      time[0] = BCD2UpperCh(hr);
      time[1] = BCD2LowerCh(hr);
      time[3] = BCD2UpperCh(minute);
      time[4] = BCD2LowerCh(minute);
      time[6] = BCD2UpperCh(second);
      time[7] = BCD2LowerCh(second);


      if(ap)
      {
         time[9] = 'P';
         time[10] = 'M';
      }
      else
      {
         time[9] = 'A';
         time[10] = 'M';
      }



}

//kp
unsigned char Mask(char kp)
  {
      switch (kp) {

      case  1: kp = '1'; return kp; break; // 1        // Uncomment this block for keypad4x4
      case  2: kp = '4'; return kp; break; // 2
      case  3: kp = '7'; return kp; break; // 3
      case  4: kp = '*'; return kp; break; // A
      case  5: kp = '2'; return kp; break; // 4
      case  6: kp = '5'; return kp; break; // 5
      case  7: kp = '8'; return kp; break; // 6
      case  8: kp = '0'; return kp; break; // B
      case  9: kp = '3'; return kp; break; // 7
      case 10: kp = '6'; return kp; break; // 8
      case 11: kp = '9'; return kp; break; // 9
      case 12: kp = '#'; return kp; break; // C
      case 13: kp = 'A'; return kp; break; // *
      case 14: kp = 'B'; return kp; break; // 0
      case 15: kp = 'C'; return kp; break; // #
      case 16: kp = 'D'; return kp; break; // D

    }
        
  }        