sbit RFIDEnable at RC0_bit;
void Init();
void GrapIDs();
char addCard();
char removeCard();
void CheckCard();
void registeredCardAction();
void notRegisteredCardAction();
unsigned char buffer,cardExists=0,Row=0,j,Exist,Exist1,Exist2,uart_rd[20],id[16][14]={"","","","","","","","","","","","","","","",""};