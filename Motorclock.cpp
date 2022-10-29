#include "Motorclock.h"
#include <Servo.h>
#include <Stepper.h>

/*
	Initialization constructor, 
	this function sets private variables and
	inits mode of pins of buzzer, buttons and rgb LED
*/
Motorclock::Motorclock(int hour, int minutes, uint8_t servoP, uint8_t stepperN1P, 
						uint8_t stepperN2P, uint8_t stepperN3P, uint8_t stepperN4P, 
						uint8_t buzzerP, uint8_t button1P, uint8_t button2P, uint8_t rgbRP, 
						uint8_t rgbGP, uint8_t rgbBP)
{	
	_is_Set_Alarm = false;
	_time = millis();
	_hour = hour;
	_seconds = 0;
	_minutes = minutes;
	_servo_P = servoP;
	_stepper_N1_P = stepperN1P;
	_stepper_N2_P = stepperN2P;
	_stepper_N3_P = stepperN3P;
	_stepper_N4_P = stepperN4P;
	_buzzer_P = buzzerP;
	_button_1P = button1P;
	_button_2P = button2P;
	_rgb_R_P = rgbRP;
	_rgb_G_P = rgbGP;
	_rgb_B_P = rgbBP;

	//init pins of buzzer, buttons and rgb LED
	initPins();
  analogWrite(_rgb_R_P, 0);
  analogWrite(_rgb_G_P, 0);
  analogWrite(_rgb_B_P, 0);
}

String Motorclock::showTime()
{	
	updateTime();
	setHourServo(_hour);
	setMinutesStepper(_minutes);
	String result = String(_hour) + ":" + String(_minutes);

	return result;
}

void Motorclock::setTimer(double minutes)
{
	buzzerSound(minutes);
}

void Motorclock::setTimerByHand()
{	
  analogWrite(_rgb_R_P, 255);
	int clicksNumber = countButton1Clicked();
	Serial.println(clicksNumber);
	buzzerSound(clicksNumber);
  analogWrite(_rgb_R_P, 0);
}

void Motorclock::setAlarm(int hour, int minutes)
{	
	_is_Set_Alarm = true;
	_alarm.hours = hour;
	_alarm.minutes = minutes;
}

void Motorclock::setAlarmByHand()
{
  analogWrite(_rgb_G_P, 255);
	Serial.println(_alarm.hours);
	Serial.println(_alarm.minutes);

	_is_Set_Alarm = true;
	int hour = countButton1Clicked() % _HOURS_ON_WATCH;
	delay(500);
	int minutes = (countButton1Clicked() % 6) * 10;

	_alarm.hours = hour;
	_alarm.minutes = minutes;
	delay(500);
}

void Motorclock::checkAlarm()
{	
	updateTime();

	if(_is_Set_Alarm && _hour >= _alarm.hours && _minutes >= _alarm.minutes)
	{
		buzzerSound(0);
		_is_Set_Alarm = false;
	}

  analogWrite(_rgb_G_P, 0);
}

double Motorclock::setStopWatch()
{	
	long startTime = millis();
	long countTime = 0;

	while(!digitalRead(_button_2P)){}

	countTime = millis() - startTime;
	double result = countTime;
	result = result / (double)_MILLISECONDS_IN_SECOND;
	result = result / (double)_SECONDS_IN_MINUTE;

  int revolutions = result / _MINUTES_IN_HOUR;
  for(int i = 0; i < revolutions; i++)
  {
    setMinutesStepper(_MINUTES_IN_HOUR);
  }
  setMinutesStepper(result - revolutions * _MINUTES_IN_HOUR);
  delay(1000);

  for(int i = 0; i < revolutions; i++)
  {
    setMinutesStepper(-_MINUTES_IN_HOUR);
  }
  setMinutesStepper(-(result - revolutions * _MINUTES_IN_HOUR));

	return result;
}

double Motorclock::setStopWatchByHand()
{
  analogWrite(_rgb_B_P, 255);
  while(!digitalRead(_button_1P)){}
  long startTime = millis();

  while(!digitalRead(_button_2P)){}
  long countTime = millis() - startTime;
  double result = countTime;
  result = result / (double)_MILLISECONDS_IN_SECOND;
  result = result / (double)_SECONDS_IN_MINUTE;

  result = 60;
  int revolutions = result / _MINUTES_IN_HOUR;
  for(int i = 0; i < revolutions; i++)
  {
    setMinutesStepper(_MINUTES_IN_HOUR);
  }
  setMinutesStepper(result - revolutions * _MINUTES_IN_HOUR);
  analogWrite(_rgb_B_P, 0);

	return result;
}





//Initialization of private functions


/*
	this function inits mode of pins
*/
void Motorclock::initPins()
{
	pinMode(_buzzer_P, OUTPUT);
	pinMode(_button_1P, INPUT);
	pinMode(_button_2P, INPUT);
	pinMode(_rgb_R_P, OUTPUT);
	pinMode(_rgb_G_P, OUTPUT);
	pinMode(_rgb_B_P, OUTPUT);
}

/*
	this function sets servo motor on accurate angle
*/
void Motorclock::setHourServo(int hour)
{
	Servo servo;	
	servo.attach(_servo_P);
	int angle = map(hour % _HOURS_ON_WATCH, 0, _HOURS_ON_WATCH - 1, 0, _MAX_ANGLE_SERVO);
	servo.write(angle);
	delay(_SERVO_DELAY);
}

/*
	this function sets stepper motor on accurate angle
*/
void Motorclock::setMinutesStepper(int minutes)
{
	Stepper stepper(_STEPS_PER_REVOLUTION, _stepper_N1_P, _stepper_N2_P,
		_stepper_N3_P, _stepper_N4_P);

	stepper.setSpeed(_STEPPER_MOTOR_SPEED);

	int nTimes;

  if(minutes >= 0)
  {
    nTimes = map(minutes, 0, _MINUTES_IN_HOUR, 0, _STEPPER_MOTOR_PARAMETER);
    for(int i = 0; i < nTimes; i++)
    {
      stepper.step(_STEPPER_ONE_STEP);
    }
  }
  else
  {
    nTimes = map(-minutes, 0, _MINUTES_IN_HOUR, 0, _STEPPER_MOTOR_PARAMETER);
    for(int i = 0; i < nTimes; i++)
    {
      stepper.step(-_STEPPER_ONE_STEP);
    }    
  }
}

/*
	after (int minutes) this functions turns on buzzer
	an buzzer will be on before user doesn't press button2
*/
void Motorclock::buzzerSound(int minutes)
{
	//delay(minutes * 1000);
	long millisTime = millis();
	int milliSecondsInMinute = _MILLISECONDS_IN_SECOND * _SECONDS_IN_MINUTE;
	long delayTime = minutes * milliSecondsInMinute;

	while(millis() - millisTime < delayTime){}
	digitalWrite(_buzzer_P, HIGH);
	while(!digitalRead(_button_2P)){}
	digitalWrite(_buzzer_P, LOW);
}

/*
	this function counts clicks on 
	button 1 until user doesn't click
	on button 2
*/
int Motorclock::countButton1Clicked()
{	
	int clicksNumber = 0;
	bool readingButton2 = LOW;
	
	bool readingButton1 = LOW;
	bool previousStateButton1 = LOW;

	while(!readingButton2)
	{
		readingButton2 = digitalRead(_button_2P);
		readingButton1 = digitalRead(_button_1P);

		if(readingButton1 == HIGH && readingButton1 != previousStateButton1)
		{
			clicksNumber++;
			previousStateButton1 = readingButton1;
		}

		if(readingButton1 == LOW)
		{
			previousStateButton1 = LOW;
		}
	}

	return clicksNumber;
}

/*
	this function updates time
*/
void Motorclock::updateTime()
{
	long differenceTime = millis() - _time;
	_time = millis();

	differenceTime = differenceTime / _MILLISECONDS_IN_SECOND;
	updateSeconds(differenceTime);

	differenceTime = differenceTime / _SECONDS_IN_MINUTE;
	updateMinutes(differenceTime);

	differenceTime = differenceTime / _MINUTES_IN_HOUR;
	updateHours(differenceTime);
}

void Motorclock::updateSeconds(long differenceTime)
{
	_seconds += differenceTime % _SECONDS_IN_MINUTE;
	if(_seconds / _SECONDS_IN_MINUTE > 0)
	{
		_seconds = _seconds % _SECONDS_IN_MINUTE;
		_minutes++;
	}
}

void Motorclock::updateMinutes(long differenceTime)
{
	_minutes += differenceTime % _MINUTES_IN_HOUR;
	if(_minutes / _MINUTES_IN_HOUR > 0)
	{
		_minutes = _minutes % _MINUTES_IN_HOUR;
		_hour++;
	}
}

void Motorclock::updateHours(long differenceTime)
{
	_hour += differenceTime % _MINUTES_IN_HOUR;
	if(_hour / _HOURS_ON_WATCH > 0)
	{
		_hour = _hour % _HOURS_ON_WATCH;
	}
}