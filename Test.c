#include "RTC_source.c"

void main() {
Initialization();
I2C1_Init(100000);
UART1_Write_Text("Start");
for(;;){
CheckCard();
//checkPassword();
}
}