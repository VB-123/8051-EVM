#include<reg51.h>
#define msec 50
#define msec_d 10
#define lcd_data_str_pin P2 
sbit rs = P3^0;  //Register select (RS) pin
sbit rw = P3^1;  //Read write(RW) pin
sbit en = P3^2;  //Enable(EN) pin
sbit start_pin = P1^0; // Start voting pin
sbit stop_pin = P1^5; // Stop voting pin

sbit party_1=P1^1;  //Candidate1
sbit party_2=P1^2;  //Candidate2
sbit party_3=P1^3;  //Candidate3
sbit party_4=P1^4;  //Candidate4
int max = 0;
int carry = 0;
int arr[4];

int vote_amt[3],j;
unsigned int vote_1,vote_2,vote_3,vote_4,winner;

// Helper functions
void delay(int delay_time);
void lcd_cmd(unsigned char cmd);
void lcd_str(char str[50]);
void lcd_data_int(unsigned int vote);
void lcd_display();
void vote_count();
void results();

void main(){
	/*MAIN FUNCTION:
	Description:  Sets the initial values of all the pins. 
								The voting starts once the start pin is set to 0.
								The votes are incremented as the bits are set to 1.
								The counting stops when the stop pin is set to zero.
	Also this function interfaces with the LCD to create the home display. */
	start_pin = stop_pin = 1;
	vote_1 = vote_2 = vote_3 = vote_4 = 0;
	party_1 = party_2 = party_3 = party_4 = 1;
	
	lcd_cmd(0x38); delay(msec);
	lcd_cmd(0x0E); delay(msec);
	lcd_cmd(0x01); delay(msec);
	lcd_cmd(0x80); delay(msec);
	lcd_str( "    WELCOME!    " ); delay(msec);
	lcd_cmd(0x01); lcd_cmd(0x80); delay(msec);
	lcd_str(" Press START "); delay(msec);
	lcd_cmd(0xC0); delay(msec);
	lcd_str(" To Vote "); delay(msec);
	delay(100);
	while(1){
		while(start_pin != 0){
		 if (stop_pin == 0){
			break;
			}
		}
		
		if (stop_pin == 0){
			break;
		}
		lcd_display();
		}
		while(1){
		results();
		}
}

void delay(int delay_time){  // Time delay function
	int j,k;
	for(j=0;j<=delay_time;j++){
		for(k=0;k<=1000;k++);}
}

void lcd_cmd(unsigned char cmd){
	/*LCD_COMMAND: To write a command to the LCD.
	@PARAMS: cmd: The command to be written to the LCD.
	DESC: The enable pin is set to 1. Then the command register is selected.
	Then the command is written to the register.*/
	lcd_data_str_pin = cmd;
	en = 1;
	rs = 0;
	rw = 0;
	delay(1);
	en = 0;
	return;
}

void lcd_str(char str[50]){
	/*LCD_STRING: To write a String to the LCD.
	@PARAMS: str: The string to be written to the LCD, 50 bytes long atmost.
	DESC: The enable pin is set to 1. The read/write pin is set to 0, to write.
	Then the data register is selected.*/
	int p;
	for (p=0;str[p]!='\0';p++){
		lcd_data_str_pin = str[p];
		rw = 0;
		rs = 1;
		en = 1;
		delay(1);
		en = 0;
	}
	return;
}

void lcd_data_int(unsigned int vote){
	/*LCD_data_Integer: To write a numeric value to the LCD.
	@PARAMS: vote: The no. of votes to be written to the LCD.
	DESC: Each digit is extracted from the integer and converted to it's ASCII value.
	The enable pin is set to 1. The read/write pin is set to 0, to write.
	Then the data register is selected.*/
	char dig_ctrl_var;
	int p,j;
	for (j=2;j>=0;j--){
		vote_amt[j]=vote%10;
		vote=vote/10;
	}

	for (p=0;p<=2;p++){
		dig_ctrl_var = vote_amt[p]+48;
		lcd_data_str_pin = dig_ctrl_var;
		rw = 0;
		rs = 1;
		en = 1;
		delay(1);
		en = 0;
	}
return;
} 

void vote_count(){  
	/* Function to count votes 
	DESC: On high to low transition increment the vote counts are incremented.*/
	while (party_1==1 && party_2==1 && party_3==1 && party_4==1 && stop_pin != 0);
	if (party_1 == 0){
    delay(msec_d);          // debounce delay
    if(party_1 == 0){
		while (party_1 == 0); // Wait for the switch to be released
			vote_1++;
    }
	}
	else if (party_2==0){
		delay(msec_d);
    if(party_2 == 0){
		while (party_2 == 0); 
			vote_2++;
    }
	}
	else if (party_3==0){
		delay(msec_d);
    if(party_3 == 0){
		while (party_3 == 0); 
			vote_3++;
    }
	}
	else if (party_4==0){
		delay(msec_d);
    if(party_4 == 0){
		while (party_4 == 0); 
			vote_4++;
    }
	}
}

void lcd_display(){
	/*Funtion to display all the parties contesting in the election.*/
    lcd_cmd(0x38); delay(msec);
		lcd_cmd(0x0E);delay(msec);
		lcd_cmd(0x01);delay(msec);
		lcd_cmd(0x80);delay(msec);
		lcd_str("ABC"); delay(msec);
		lcd_cmd(0x84); delay(msec);
		lcd_str("PQR"); delay(msec);
		lcd_cmd(0x88); delay(msec);
		lcd_str("XYZ"); delay(msec);
		lcd_cmd(0x8C); delay(msec);
		lcd_str("OTH");delay(msec);

		vote_count();
		lcd_cmd(0x01); delay(msec);
		lcd_cmd(0x82); delay(msec);
    lcd_str("THANK"); delay(msec);
		lcd_cmd(0x14); delay(msec);
		lcd_str("YOU!!"); delay(100);
}

void results(){
	/*Function to declare the results.
	DESC: First the function displays the no. of votes for each party.
								Then the maximum number of votes is calculated and the winner is declared.
								It also checks if there are any ties in the results.*/
	int i;
	int carry = 0;
	lcd_cmd(0x01);delay(msec);
	lcd_cmd(0x80); delay(msec);
	lcd_str("Results"); delay(msec);
	lcd_cmd(0x14); delay(msec);
	lcd_str("Are"); delay(msec);
	lcd_cmd(0x14); delay(msec);
	lcd_str("Out!");delay(msec);
	// Display the party names
	lcd_cmd(0x01);delay(msec);
	lcd_cmd(0x80);delay(msec);
	lcd_str("ABC");delay(msec);
	lcd_cmd(0x84);delay(msec);
	lcd_str("PQR");delay(msec);
	lcd_cmd(0x88); delay(msec);
	lcd_str("XYZ");delay(msec);
	lcd_cmd(0x8C);delay(msec);
	lcd_str("OTH");delay(msec);
	// Display the vote count
	lcd_cmd(0xC0);delay(100);
	lcd_data_int(vote_1);delay(msec);

	lcd_cmd(0xC4);delay(msec);
	lcd_data_int(vote_2);delay(msec);

	lcd_cmd(0xC8);delay(msec);
	lcd_data_int(vote_3);delay(msec);

	lcd_cmd(0xCC);delay(msec);
	lcd_data_int(vote_4);delay(300);

	arr[0] = vote_1;
	arr[1] = vote_2;
	arr[2] = vote_3;
	arr[3] = vote_4;
	// Find maximum number of votes
	for( i=0; i<4; i++){
		if(arr[i]>=max)
			max = arr[i];
	}
	// Declare the winner
	if ( (vote_1 == max) && ( vote_2 != max) && (vote_3 != max)&& (vote_4 != max) ){
		carry = 1;
		lcd_cmd(0x01);delay(msec);
		lcd_cmd(0x82);delay(msec);
		lcd_str("ABC");delay(msec);
		lcd_cmd(0x86);delay(msec);
		lcd_str("WINS!");delay(msec);
		winner = 1;
	}

	if ( (vote_2 == max) && ( vote_1 != max) && (vote_3 != max)&& (vote_4 != max) ){
		carry = 1;
		lcd_cmd(0x01);delay(msec);
		lcd_cmd(0x82);delay(msec);
		lcd_str("PQR");delay(msec);
		lcd_cmd(0x86);delay(msec);
		lcd_str("WINS!");delay(msec);
		winner =2;
	}

	if ( (vote_3 == max) && ( vote_2 != max) && (vote_1 != max)&& (vote_4 != max) ){
		carry = 1;
		lcd_cmd(0x01);delay(msec);
		lcd_cmd(0x82);delay(msec);
		lcd_str("XYZ");delay(msec);
		lcd_cmd(0x86);delay(msec);
		lcd_str("WINS!");delay(msec);
		winner = 3;
	}

	if ( (vote_4 == max) && ( vote_2 != max) && (vote_3 != max)&& (vote_1 != max) ){
		carry = 1;
		lcd_cmd(0x01);delay(msec);
		lcd_cmd(0x82);delay(msec);
		lcd_str("OTH");delay(msec);
		lcd_cmd(0x86);delay(msec);
		lcd_str("WINS!");delay(msec);
		winner = 4;
	}
	// Tie 
	if (carry==0){
		lcd_cmd(0x01);delay(msec);
		lcd_cmd(0x82);delay(msec);
		lcd_str("CLASH");delay(50);
		lcd_cmd(0x14);delay(msec);
		lcd_str("BETWEEN!!!");delay(50);
		if(vote_1 == max){
		 lcd_cmd(0xC0);
		 lcd_str("ABC");
		 delay(50);
		}
		if(vote_2 == max){
		 lcd_cmd(0xC5);
		 lcd_str("PQR");
		 delay(50);
		}
		if(vote_3 == max){
		 lcd_cmd(0xC9);
		 lcd_str("XYZ");
		 delay(50);
		}
		if(vote_4 == max){
		 lcd_cmd(0xCD);
		 lcd_str("OTH");
		 delay(50);
		}
	}
}

