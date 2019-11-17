
#include <tm4c123gh6pm.h>

void UART_Init(void);
unsigned char UART_InChar(void);
void UART_OutChar(unsigned char data);
void display(char *string);
void displaynash(void);
unsigned char myChar;
void waitforanswer(void);
void PORTF_init(void);
int isbuttonpressed = 0;
int inputneeded = 1;
int answer =0;
void light(void);
uint8_t Data=0x01;

int main(){
	
	UART_Init();  // initialization
	PORTF_init();//
	 while(1){
		   
		   waitforanswer(); // check if there is input from putty
		   light(); // if there is input flash the light according that input 
		   Data = GPIO_PORTF_DATA_R & 0x10; // check if the button is presed 
		   if(Data == 0x00) // if it is, display the string;
				 displaynash(); // function to dispay the string 
		 
}
	
}
unsigned char UART_InChar(void){
	
  while((UART0_FR_R&0x0010) != 0)return '\0'; // wait until RXFE is 0
  return((unsigned char)(UART0_DR_R&0xFF));
}
// Wait for buffer to be not full, then output
void UART_OutChar(unsigned char data){
	
  while((UART0_FR_R&0x0020) != 0); // wait until TXFFis 0
	
  UART0_DR_R = data;
}
void display(char *string){
	while(*string){
		UART_OutChar(*(string++));
	}
}

void UART_Init(void){

	SYSCTL_RCGC1_R |= 0x0001;

	SYSCTL_RCGC2_R |= 0x0001;

	UART0_CTL_R &= ~0x0001;

	UART0_IBRD_R = 104;// IBRD=int(16000000/(16*9600)) = int(104.166666)

	UART0_FBRD_R = 11;// FBRD = int(0.166666 * 64 + 0.5) = 11

	UART0_LCRH_R = 0x0070;	// 8-bit length, enable FIFO

	UART0_CTL_R = 0x0301; 	// enable RXE, TXE and UART

	GPIO_PORTA_AFSEL_R |= 0x03; // alt funct on PA1-0

	GPIO_PORTA_DEN_R |= 0x03; // digital I/O on PA1-0

}


void PORTF_init(){
	
	SYSCTL_RCGCGPIO_R |= 0x20;
	
	GPIO_PORTF_DIR_R = 0x0E;
	
	GPIO_PORTF_PUR_R = 0x10;
	
	GPIO_PORTF_DEN_R = 0x1E;
	
}

void waitforanswer(){


	myChar = UART_InChar (); 

	if(myChar){
  
	answer = 1;
	}

}
void light(void){
	  
			
	    if(answer == 1){
		  UART_OutChar(myChar);
		 switch (myChar){
			case 'r' :
				GPIO_PORTF_DATA_R = 0x02;
			break;
			
			case 'b' :
				GPIO_PORTF_DATA_R = 0x04;
			break;
			
			case 'g' :
				GPIO_PORTF_DATA_R = 0x08;
			break;
			
			default :
				GPIO_PORTF_DATA_R = 0x00;
		}
	}
			answer = 0;
	    
	
		 
		
}
	
void displaynash(void){
	
  display("NASHVILLE TN");
	display("\n");
	
} 
