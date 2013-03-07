#ifndef sensor_h
#define sensor_h

#include "Arduino.h"

#include <motor.h>

#define GRIDSENS_BACK_RIGHT  7
#define GRIDSENS_BACK_LEFT   6

#define GRIDSENS_FRONT_RIGHT_WIDE  4
#define GRIDSENS_FRONT_LEFT_WIDE   7

#define GRIDSENS_FRONT_RIGHT 5
#define GRIDSENS_FRONT_LEFT  6

#define TURNLEFT 0
#define TURNRIGHT 1
#define STAY 2
#define STRAIGHT 3
#define BACKUP 4
	
#define DISTANCESENS_FRONT	A0
#define DISTANCESENS_LEFT	A2
#define DISTANCESENS_RIGHT	A1
#define DISTANCESENS_BACK	A3
	
class sensor
{ 
  public:
    sensor();
	
	static void goStraightTile(int val); //user specified numver of tiles to move
	static void rotate90Left();
	static void rotate90Right();
	static void rotate180Right();
	static void rotate180Left();
	
	static int DEBOUNCE;
	volatile static int ACTION;				//state of the robot
	volatile static int GOGRIDCOUNT; 		//user specified tile count
	static void process();
	static boolean getFrontLeftWhite();
	static boolean getFrontRightWhite();
	static boolean getBackLeftWhite();
	static boolean getBackRightWhite();
	static boolean getFrontLeftWideWhite();
	static boolean getFrontRightWideWhite();\
	
	volatile static int distance_front;
	volatile static int distance_left;
	volatile static int distance_right;
	volatile static int distance_back;
	
	static void updateDistances();
	
  private:
 
};

#endif