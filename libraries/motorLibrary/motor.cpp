#include "motor.h"
#include "Arduino.h"
#include <avr/interrupt.h>

//static member initial definition
volatile boolean motor::AIN1 = false;
volatile boolean motor::AIN2 = false;
volatile boolean motor::BIN1 = false;
volatile boolean motor::BIN2 = false;

volatile int motor::MOTORACTION = MOTORSTOP;
	 
volatile boolean motor::RUNFOREVER = false;
	 
volatile int motor::MOTORDUTYLEFT = 75;      //motor PWM duty
volatile int motor::MOTORDUTYRIGHT = 75;     //motor PWM duty
	
	 
volatile int motor::MOTORONDURATIONSET = 0;  //motor PWM on duration
volatile int motor::MOTORONDURATION = 0;  //motor PWM on duration

motor::motor()	//constructor
{
    noInterrupts();  
	
	// AIN1 = false;
	// AIN2 = false;
	// BIN1 = false;
	// BIN2 = false;
	
	MOTORACTION = MOTORSTOP;
  
    pinMode(AIN1PIN, OUTPUT);
    pinMode(AIN2PIN, OUTPUT);
    pinMode(BIN1PIN, OUTPUT);
    pinMode(BIN2PIN, OUTPUT);
    
    // digitalWrite(AIN1PIN, 0);
    // digitalWrite(AIN2PIN, 0);
    // digitalWrite(BIN1PIN, 0);
    // digitalWrite(BIN2PIN, 0);
    
    TCCR2A = 0;
    TCCR2B = 0;
    
    OCR2A = 110;            //compare registers
    OCR2B = 100;
  
    TCNT2 = 0;            // preload timer
	
    TCCR2B = 0x02;        // 8 prescaler 3.906 kHz

    // TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt
    // TIMSK2 |= (1 << OCIE2B);  // enable timer compare interrupt
	// TIMSK2 |= (1 << TOIE2);   // enable timer overflow interrupt
    
	//enable specific interrupts from Sensor Class
	
    interrupts();             // enable all interrupts
}

void motor::motorStartRotate(boolean left, int modulation)
{
    motorDutySet(modulation, modulation);
      
      if(left == true)
      {			
			MOTORACTION = MOTORPIVOTLEFT;
      }      
      else
      {
			MOTORACTION = MOTORPIVOTRIGHT;
      } 
}

/* int motor::motorDirDist(int dir, int dist, int modulation)
{
      //initialize duration for going straight; exact formula yet to be determined
      int durationForward = dist * CALIBRATE_MOTOR_STRAIGHT / modulation * 100 ; 

      //set modulation 0-100
      if (modulation < 1 || modulation > 100)
      {
          return -1;
      }
      
      //dir 0: no turn, -180: 180 degrees to left, +180: 180 degrees to right
      if(dir < -180 || dir > 180)
      {
          return -1;
      }
     
      //distance to go straight forward or backward, exact range yet to be determined
      if(dist < -10000 || dist > 10000)
      {    
          return -1;
      }   

      motorDir(dir, modulation);  //turn the robot 
      
      int dur = 0;
      do
      {
        // ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        // {
          int j = 0;
          dur = MOTORONDURATION;
        // }
      }while(dur > 0); 
     
      motorDutySet(modulation, modulation);
      
      if(durationForward > 0)          //move robot straight
      {
          // ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
          // {
            RUNFOREVER = false;
            
            AIN1 = true;      //pin 10  -> AIN1 on (left motor)
            AIN2 = false;     //pin 11  -> AIN2 off (left motor)
            
            BIN1 = true;      //pin 12  -> BIN1 on (right motor)
            BIN2 = false;     //pin 13  -> BIN2 off (right motor)
  
            MOTORONDURATIONSET = durationForward;
            MOTORONDURATION = durationForward;    
          // }   
      }
      else if(durationForward < 0)     //move backward
      {   
          // ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
          // {
            RUNFOREVER = false;
            
            AIN1 = false;     //pin 10  -> AIN1 off (left motor)
            AIN2 = true;      //pin 11  -> AIN2 on (left motor)
            
            BIN1 = false;     //pin 12  -> BIN1 off (right motor)
            BIN2 = true;      //pin 13  -> BIN2 on (right motor)
            
            MOTORONDURATIONSET = -1* durationForward;
            MOTORONDURATION = -1* durationForward; 
          // }
      }
      
      do
      {
        // ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        // {
          int g = 0;
          dur = MOTORONDURATION;
        // }
      }while(dur > 0);
     
      return 0;
}

int motor::motorDistNoBlock(int dist, int modulation)
{
	 if(MOTORONDURATION == 0)
	 {
		 //initialize duration for going straight; exact formula yet to be determined
		  int durationForward = dist * CALIBRATE_MOTOR_STRAIGHT / modulation * 100 ; 

		  //set modulation 0-100
		  if (modulation < 1 || modulation > 100)
		  {
			  return -1;
		  }
		 
		  //distance to go straight forward or backward, exact range yet to be determined
		  if(dist < -10000 || dist > 10000)
		  {    
			  return -1;
		  }
		 
		  motorDutySet(modulation, modulation);
		  
		  if(durationForward > 0)          //move robot straight
		  {
			  // ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			  // {
				RUNFOREVER = false;
				
				AIN1 = true;      //pin 10  -> AIN1 on (left motor)
				AIN2 = false;     //pin 11  -> AIN2 off (left motor)
				
				BIN1 = true;      //pin 12  -> BIN1 on (right motor)
				BIN2 = false;     //pin 13  -> BIN2 off (right motor)
	  
				MOTORONDURATIONSET = durationForward;
				MOTORONDURATION = durationForward;    
			  // }   
		  }
		  else if(durationForward < 0)     //move backward
		  {   
			  // ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			  // {
				RUNFOREVER = false;
				
				AIN1 = false;     //pin 10  -> AIN1 off (left motor)
				AIN2 = true;      //pin 11  -> AIN2 on (left motor)
				
				BIN1 = false;     //pin 12  -> BIN1 off (right motor)
				BIN2 = true;      //pin 13  -> BIN2 on (right motor)
				
				MOTORONDURATIONSET = -1* durationForward;
				MOTORONDURATION = -1* durationForward; 
			  // }
		  }
     }
      return 0;
} */

int motor::motorStart(boolean forward,int modulationLeft, int modulationRight)
{
    
      motorDutySet(modulationLeft, modulationRight);
      
      //move robot straight indefinitely
      if(forward)
      {		
			MOTORACTION = MOTORFORWARD;
      }
      else
      {    
			MOTORACTION = MOTORBACKWARD;
      }
      
      return 0;
}

void motor::motorDutySet(int left, int right)
{
	MOTORDUTYLEFT = left;
	MOTORDUTYRIGHT = right;
}

void motor::motorStop()
{
		MOTORACTION = MOTORSTOP;       
}

/* void motor::motorDir(int dir, int modulation)
{
      // initialize turning duration according to direction and modulation; yet to be determined
      int durationTurn = dir * CALIBRATE_MOTOR_ROTATE / modulation *100; 
      
      motorDutySet(modulation, modulation);
      
      if(durationTurn < 0)
      {
          //left turn
          ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
          {
            RUNFOREVER = false;
            
            AIN1 = false;    //pin 10  -> AIN1 off (left motor)
            AIN2 = true;     //pin 11  -> AIN2 on (left motor)
            
            BIN1 = true;     //pin 12  -> BIN1 on (right motor)
            BIN2 = false;    //pin 13  -> BIN2 off (right motor)
            
            MOTORONDURATIONSET = -1 * durationTurn;
            MOTORONDURATION = -1 * durationTurn; 
          }         
      }      
      else if(durationTurn > 0)
      {
          //right turn
          ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
          {
            RUNFOREVER = false;
            
            AIN1 = true;     //pin 10  -> AIN1 on (left motor)
            AIN2 = false;    //pin 11  -> AIN2 off (left motor)
          
            BIN1 = false;    //pin 12  -> BIN1 off (right motor)
            BIN2 = true;     //pin 13  -> BIN2 on (right motor)
        
            MOTORONDURATIONSET = durationTurn; 
            MOTORONDURATION = durationTurn;
          }
     
      } 
}
 */
void motor::compAInterrupt()
{
	//set motor output AIN1, AIN2 to low
  
     // digitalWrite(AIN1PIN,1);
     // digitalWrite(AIN2PIN,1);

	 // PORTB &= B11110011;
	 PORTB |= B00001100;
	 
	
}

void motor::compBInterrupt()
{
	//set motor output BIN1, BIN2 to low
      
    // digitalWrite(BIN1PIN, 1);
    // digitalWrite(BIN2PIN, 1);
	
	 PORTB |= B00110000; 
}

void motor::overFlowInterrupt()
{
	switch(MOTORACTION)
	{
		case MOTORSTOP:
			PORTB |= B00111100;
		break;
		case MOTORFORWARD:
			PORTB &= B11000011;
			PORTB |= B00010100;
		break;
		case MOTORPIVOTLEFT:
			PORTB &= B11000011;
			PORTB |= B00011000;
		break;
		case MOTORPIVOTRIGHT:
			PORTB &= B11000011;
			PORTB |= B00100100;
		break;
		case MOTORBACKWARD:
			PORTB &= B11000011;
			PORTB |= B00101000;
		break;
		default:
			PORTB &= B11000011;
	}
	
	OCR2B = motor::MOTORDUTYRIGHT*255/100;            // set compare registers to duty
	OCR2A = motor::MOTORDUTYLEFT*255/100;            // set compare registers to duty
}


