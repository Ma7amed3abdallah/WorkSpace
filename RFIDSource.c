#include "RFIDHeader.h"


void GrapIDs(){ //Grap IDs from EEProm
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
    //Display_Time();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1,"Done");
    Lcd_Out(2,1,time);
    UART1_Write_Text(uart_rd);
    UART1_Write(0x0D);
    UART1_Write(0x0A);
    UART1_Write_Text(time);
    UART1_Write(0x0D);
    UART1_Write(0x0A);
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