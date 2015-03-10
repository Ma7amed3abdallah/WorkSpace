#line 1 "C:/Users/Mohamed/OneDrive/Documents/WorkSpace/RFIDKEYLCD/WorkSpace/Test.c"
#line 1 "c:/users/mohamed/onedrive/documents/workspace/rfidkeylcd/workspace/rtc_source.c"
#line 1 "c:/users/mohamed/onedrive/documents/workspace/rfidkeylcd/workspace/rtc.h"


sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;




int second;
int minute;
int hour;
int hr;
int day;
int dday;
int month;
int year;
int ap;
unsigned short set_count = 0;
short set;


char time[] = "00:00:00 PM";
char date[] = "00-00-00";

unsigned char Enter_ID[10]="", txt[17]="Correct Password";
char pass_word[5]="1111";
char i=0;
char keypadPort at PORTD;
unsigned char kp;
short cnt=0, oldstate = 0;
char e;

void Display_Time();
int BCD2Binary(int a);
int Binary2BCD(int a);
unsigned char BCD2LowerCh(unsigned char bcd);
unsigned char BCD2UpperCh(unsigned char bcd);
void write_ds1307(unsigned short address,unsigned short w_data);
unsigned short read_ds1307(unsigned short address);
void change_state(char k);
unsigned char Mask(char kp);
void Initialization();
char checkPassword();
void correctPassword();
void wrongPassword();
char readKeypad();
#line 1 "c:/users/mohamed/onedrive/documents/workspace/rfidkeylcd/workspace/rfidsource.c"
#line 1 "c:/users/mohamed/onedrive/documents/workspace/rfidkeylcd/workspace/rfidheader.h"
sbit RFIDEnable at RC0_bit;
void Init();
void GrapIDs();
char addCard();
char removeCard();
void CheckCard();
void registeredCardAction();
void notRegisteredCardAction();
unsigned char buffer,cardExists=0,Row=0,j,Exist,Exist1,Exist2,uart_rd[20],id[16][14]={"","","","","","","","","","","","","","","",""};
#line 4 "c:/users/mohamed/onedrive/documents/workspace/rfidkeylcd/workspace/rfidsource.c"
void GrapIDs(){
for(j=0;j<16;j++){
for(i=0;i<10;i++){
id[j][i]=EEPROM_Read((j*16)+i);
delay_ms(10);
}
}
}



char addCard(){
cardExists=0;
RFIDEnable=1;
for(;;){
if (UART1_Data_Ready() == 1&&cardExists==0){
for(i=0;i<12;i++){
for(;!UART1_Data_Ready(););
uart_rd[i]=UART1_Read();
}
RFIDEnable=0;
if(uart_rd[0]==0x0A&&uart_rd[11]==0x0D){
for(i=0;i<16;i++){
Exist1=strstr(uart_rd,id[i]);
if(Exist1!=0){
Lcd_Cmd(_LCD_CLEAR);
Lcd_Out(1,2,"Already Exists");
cardExists=1;
return 0;
}
}
if(cardExists==0){
for(i=0;i<=0xF0;i=i+16){
buffer=EEPROM_Read(i);
if(buffer==0x9F){
Row=i;
break;
}
}
for(i=1;i<11;i++){
EEPROM_Write(Row+(i-1),uart_rd[i]);
id[Row/16][i-1]=uart_rd[i];
}
Exist1=strstr(uart_rd,id[Row/16]);
if(Exist1!=0){
Lcd_Cmd(_LCD_CLEAR);
Lcd_Out(1,3,"Card Added");
cardExists=1;
return 0;
}
}
}
else{
Lcd_Cmd(_LCD_CLEAR);
Lcd_Out(1,5,"Failed");
return 0;
}
}
}
}

char removeCard(){
cardExists=0;
RFIDEnable=1;
for(;;){
if (UART1_Data_Ready() == 1&&cardExists==0){
for(i=0;i<12;i++){
for(;!UART1_Data_Ready(););
uart_rd[i]=UART1_Read();
}
RFIDEnable=0;
if(uart_rd[0]==0x0A&&uart_rd[11]==0x0D){
for(i=0;i<16;i++){
Exist1=strstr(uart_rd,id[i]);
if(Exist1!=0)
{
for(j=0;j<10;j++){
id[i][j]=0x9F;
EEPROM_Write((i*16)+j,0x9F);
}
cardExists=1;
Lcd_Cmd(_LCD_CLEAR);
Lcd_Out(1,3,"Card Removed");
return 0;
}
}
}
else{
Lcd_Cmd(_LCD_CLEAR);
Lcd_Out(1,5,"Failed");
return 0;
}
}
}
}
char CheckCard(){
 RFIDEnable=1;
 if (UART1_Data_Ready() == 1)
 {
 for(i=0;i<12;i++){
 for(;!UART1_Data_Ready(););
 uart_rd[i]=UART1_Read();
 }
 RFIDEnable=0;
 if(uart_rd[0]==0x0A&&uart_rd[11]==0x0D){
 for(i=0;i<16;i++){
 Exist=strstr(uart_rd,id[i]);
 if(Exist!=0){
 registeredCardAction();
 return 0;
 }
 }
 notRegisteredCardAction();
 return 0;
 }
 }
}
void registeredCardAction(){
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1,1,"Done");
 delay_ms(1000);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1,1,"Pass Your ID");
}

void notRegisteredCardAction(){
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1,1,"Not Registered");
 delay_ms(1000);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1,1,"Pass Your ID");
}
#line 4 "c:/users/mohamed/onedrive/documents/workspace/rfidkeylcd/workspace/rtc_source.c"
 void Initialization()
 {
 TRISC0_bit=0;
 RFIDEnable=1;
 GrapIDs();
 Keypad_Init();
 Lcd_Init();
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Cmd(_LCD_CURSOR_OFF);
 Lcd_Out(1,2,"Pass Your ID");
 UART1_Init(2400);
 Delay_ms(100);
 }


char checkPassword(){
 for(;;){
 kp = 0;
 kp = Keypad_Key_Click();
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
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1,2,"Pass Your ID");
 return 0;
 }
 else{
 wrongPassword();
 Lcd_Cmd(_LCD_CLEAR);
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
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1,1,"Worng Password");
 delay_ms(1000);
 }

 char readKeypad(){
 kp = 0;
 do
 kp = Keypad_Key_Click();
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
 I2C1_Wr(0xD0);
 I2C1_Wr(address);
 I2C1_Stop();
 I2C1_Start();
 I2C1_Wr(0xD1);
 r_data=I2C1_Rd(0);
 I2C1_Stop();
 return r_data;
}


void write_ds1307(unsigned short address,unsigned short w_data)
{
 I2C1_Start();

 I2C1_Wr(0xD0);
 I2C1_Wr(address);
 I2C1_Wr(w_data);
 I2C1_Stop();
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
char* Display_date()
{ dday = read_ds1307(3);
 day = read_ds1307(4);
 month = read_ds1307(5);
 year = read_ds1307(6);

 date[0] = BCD2UpperCh(day);
 date[1] = BCD2LowerCh(day);
 date[3] = BCD2UpperCh(month);
 date[4] = BCD2LowerCh(month);
 date[6] = BCD2UpperCh(year);
 date[7] = BCD2LowerCh(year);
 return date;
}

unsigned char Mask(char kp)
 {
 switch (kp) {

 case 1: kp = '1'; return kp; break;
 case 2: kp = '4'; return kp; break;
 case 3: kp = '7'; return kp; break;
 case 4: kp = '*'; return kp; break;
 case 5: kp = '2'; return kp; break;
 case 6: kp = '5'; return kp; break;
 case 7: kp = '8'; return kp; break;
 case 8: kp = '0'; return kp; break;
 case 9: kp = '3'; return kp; break;
 case 10: kp = '6'; return kp; break;
 case 11: kp = '9'; return kp; break;
 case 12: kp = '#'; return kp; break;
 case 13: kp = 'A'; return kp; break;
 case 14: kp = 'B'; return kp; break;
 case 15: kp = 'C'; return kp; break;
 case 16: kp = 'D'; return kp; break;

 }

 }
#line 2 "C:/Users/Mohamed/OneDrive/Documents/WorkSpace/RFIDKEYLCD/WorkSpace/Test.c"
int seee;

void main() {
Initialization();
I2C1_Init(100000);
for(;;){
CheckCard();
checkPassword();
}
}
