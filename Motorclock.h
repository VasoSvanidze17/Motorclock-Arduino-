#ifndef Motorclock_H
#define Motorclock_H
#include "Arduino.h"

class Motorclock
{
public:
	//Declaration Constructor
	Motorclock(int hour, int minutes, uint8_t servoP, uint8_t stepperN1P, 
				uint8_t stepperN2P, uint8_t stepperN3P, uint8_t stepperN4P, 
				uint8_t buzzerP, uint8_t button1P, uint8_t button2P, uint8_t rgbRP, 
				uint8_t rgbGP, uint8_t rgbBP);

	//Declaration public member functions
	String showTime();
	void setTimer(double minutes);
	void setTimerByHand();
	void setAlarm(int hour, int minutes);
	void setAlarmByHand();
	void checkAlarm();
	double setStopWatch();
	double setStopWatchByHand();

private:
	struct Alarm
	{
		uint8_t hours;
		uint8_t minutes;
	};

	Alarm _alarm;
	bool _is_Set_Alarm;

	//Declaration constanst private variables
	
	//constants for servo motor
	const uint8_t _MAX_ANGLE_SERVO = 180;
	const uint8_t _SERVO_DELAY = 15;

	//constants for stepper motor
	const uint8_t _STEPS_PER_REVOLUTION = 200;
	const uint8_t _STEPPER_MOTOR_SPEED = 80;
	const uint8_t _STEPPER_MOTOR_PARAMETER = 21;
	const uint8_t _STEPPER_ONE_STEP = 100;

	//constants for clock
	const int _HOURS_ON_WATCH = 12;
	const int _MINUTES_IN_HOUR = 60;
	const int _SECONDS_IN_MINUTE = 60;
	const int _MILLISECONDS_IN_SECOND = 1000;

	//Declaration primitive private variables
	long _time;
	int _hour;
	int _minutes;
	int _seconds;
	int _servo_P;
	uint8_t _stepper_N1_P;
	uint8_t _stepper_N2_P;
	uint8_t _stepper_N3_P;
	uint8_t _stepper_N4_P;
	uint8_t _buzzer_P;
	uint8_t _button_1P;
	uint8_t _button_2P;
	uint8_t _rgb_R_P;
	uint8_t _rgb_G_P;
	uint8_t _rgb_B_P; 

	//Declaration of private functions
	void initPins();
	void setHourServo(int hour);
	void setMinutesStepper(int minutes);
	void buzzerSound(int minutes);
	int countButton1Clicked();
	void updateTime();
	void updateSeconds(long differenceTime);
	void updateMinutes(long differenceTime);
	void updateHours(long differenceTime);
};
#endif