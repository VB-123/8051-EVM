#include <reg51.h>
#define msec 50

#define candidate_1 P1  //Candidate1
#define candidate_2 P3 //Candidate2
#define max_votes 0x0F

sbit start_pin = P2^0; // Start voting pin
sbit party_1 = P2^1;
sbit party_2 = P2^2;
sbit stop_pin = P2^3; // Stop voting pin

unsigned int vote_1,vote_2;
int winner,ov_flag;
int voting_in_progress = 1;

void delay(int delay_time);
void start_count();
void check();
void declare_winner();

void main(){
  delay(1);
  candidate_1 = 0x00;
  candidate_2 = 0x00;
  P2 = 0x0F;
  vote_1 = vote_2 = 0;
  while(voting_in_progress){
    if (start_pin == 0){
      delay(1);
      if (start_pin == 0){
        start_count();
      }
    }
    if (stop_pin == 0){
      delay(1);
      if (stop_pin == 0){
        voting_in_progress = 0;
        declare_winner();
      }
    }
  }
}

void delay(int delay_time){  // Time delay function
	int j,k;
	for(j = 0;j < delay_time; j++){
		for(k = 0;k < 1275; k++);}
}

void start_count(){
  while (stop_pin != 0){
    if (party_1 == 0){
      delay(msec);
      if(party_1 == 0){
        vote_1 = vote_1 + 1;
        check();
        if (ov_flag == 1){
          declare_winner();
        }
        else{
          while (party_1 == 0);
          P1 = vote_1;
        }
      }
    }
    if (party_2 == 0){
      delay(msec);
      if(party_2 == 0){
        vote_2 = vote_2 + 1;
        check();
        if (ov_flag == 1){
          declare_winner();
        }
        else{
          while(party_2 == 0);
          P3 = vote_2;
        }
      }
    }
  }
}


void check(){
  if(vote_1 == max_votes){
    winner = 1;
    ov_flag = 1;
  }
  else if(vote_2 == max_votes){
    winner = 2;
    ov_flag = 1;
  }
}

void declare_winner(){
  if (voting_in_progress = 0){
    candidate_1 = 0x00; // Turn OFF all LEDs
    delay(msec);
    candidate_2 = 0x00; // Turn OFF all LEDs
    delay(msec);
  }
  if (vote_1 > vote_2){
    winner = 1;
    candidate_1 = 0x0F; // Turn ON all LEDs at winner's port
    delay(3*msec);
    candidate_2 = 0x00; // Turn OFF all LEDs at loser's port
    delay(3*msec);
  }
  else if (vote_1 < vote_2){
    winner = 2;
    candidate_1 = 0x00; // Turn OFF all LEDs at loser's port
    delay(3*msec);
    candidate_2 = 0x0F; // Turn ON all LEDs at winner's port
    delay(3*msec);
  }
  else if (vote_1 == vote_2){
    winner = 0;
    candidate_1 = 0x0F; // In case of a tie, turn ON all LEDs
    delay(3*msec);
    candidate_2 = 0x0F; // In case of a tie, turn ON all LEDs
    delay(3*msec);
  } 
}
