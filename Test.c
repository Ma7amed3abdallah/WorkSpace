#include "RTC_source.c"
int seee;
//char uart_read;
void main() {
int *t;
//Trisc=0x80;
Initialization();
I2C1_Init(100000);
for(;;){
CheckCard();
//checkPassword();
}
}