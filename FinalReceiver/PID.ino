#include "driver/ledc.h"
#include "PID.h"


//setup_PID

//loop_PID

//internal

const int PWM_FREQ = 50;

// Smoothing using an exponential moving average
//smoothing factor of 0 is not smoothing and 1 is max smoothing(constant value)
const float SMOOTHING_FACTOR = 0.5;

// Motor 2 (left) pins
const int motor2DirectionPin = 6;
const int motor2PWMpin = 0;
const int motor2EncoderInputPin = 7;
unsigned long motor2LastEncoderTime = millis();
unsigned long motor2LastEncoderMillis = 0;
bool motor2LastEncoderState = HIGH;
float motor2Frequency = 0;
float motor2FrequencySmooth = 0;

// Motor 1 (right) pins
const int motor1DirectionPin = 4;
const int motor1PWMpin = 1;
const int motor1EncoderInputPin = 5;
unsigned long motor1LastEncoderTime = millis();
unsigned long motor1LastEncoderMillis = 0;
bool motor1LastEncoderState = HIGH;
float motor1Frequency = 0;
float motor1FrequencySmooth = 0;



const int resolution = 14; // Resolution for PWM duty cycle (14-bit) max: 2^14 - 1 = 16383
int maxDutyCycle = pow(2, resolution) - 1;
ledc_channel_t motor1Channel = LEDC_CHANNEL_0;
ledc_timer_t motor1Timer = LEDC_TIMER_0;
ledc_channel_t motor2Channel = LEDC_CHANNEL_1;
ledc_timer_t motor2Timer = LEDC_TIMER_1;

void configureMotor1Pin(){
  ledcAttach(motor1PWMpin, PWM_FREQ, resolution);
}

void configureMotor2Pin(){
  ledcAttach(motor2PWMpin, PWM_FREQ, resolution);
}


// int dutyCycle2 = 8191;     // Default duty cycle for motor 2 (50%)
bool motor1Direction = true; // Motor 1 direction
bool motor2Direction = true; // Motor 2 direction

float motor1TargetFrequency = 1;  // Target frequency in Hz
float motor2TargetFrequency = 1;  // Target frequency in Hz

void setMotor1Direction(bool direction){
  motor1Direction = direction;
  updateMotor1Direction();
}

void setMotor2Direction(bool direction){
  motor2Direction = direction;
  updateMotor2Direction();
}

void setMotor1TargetFrequency(float frequency){
  motor1TargetFrequency = frequency;
}

void setMotor2TargetFrequency(float frequency){
  motor2TargetFrequency = frequency;
}

void updateMotor1Direction() {
  digitalWrite(motor1DirectionPin, motor1Direction ? HIGH : LOW);
}

void updateMotor2Direction() {
  digitalWrite(motor2DirectionPin, motor2Direction ? LOW : HIGH);
}

//valid speeds are 0 to 1
void updateMotor1Speed(float speed) {
  int dutyCycle = speed * maxDutyCycle;
  ledcWrite(motor1PWMpin, dutyCycle);
}

void updateMotor2Speed(float speed) {
  int dutyCycle = speed * maxDutyCycle;
  ledcWrite(motor2PWMpin, dutyCycle);
}

void readMotor1Encoder(){
  bool encoderState = digitalRead(motor1EncoderInputPin);
  int currentMillis = millis();
  float timePeriod = (currentMillis - motor1LastEncoderMillis) / 1000.0;
  if (motor1LastEncoderState == HIGH && encoderState == LOW) {
    if(timePeriod > 0.005){
      //Serial.printf("Time period: %f\n", timePeriod);
      motor1Frequency = 1 / (timePeriod *20);
      //Serial.printf("Motor 1 Frequency: %f\n", motor1Frequency);
      motor1FrequencySmooth = (1 - SMOOTHING_FACTOR) * motor1FrequencySmooth + SMOOTHING_FACTOR * motor1Frequency;
      //Serial.printf("Motor 1 Frequency Smooth: %f\n", motor1FrequencySmooth);
    }
    motor1LastEncoderMillis = currentMillis;
  }
  else if(timePeriod > 1){
    //if it hasn't moved in 1 second set the frequency to 0
    motor1Frequency = 0;
    motor1FrequencySmooth = (1 - SMOOTHING_FACTOR) * motor1FrequencySmooth + SMOOTHING_FACTOR * motor1Frequency;
    motor1LastEncoderMillis = currentMillis;
  }
  motor1LastEncoderState = encoderState;
}

void readMotor2Encoder(){
  bool encoderState = digitalRead(motor2EncoderInputPin);
  int currentMillis = millis();
  float timePeriod = (currentMillis - motor2LastEncoderMillis) / 1000.0;
  if (motor2LastEncoderState == HIGH && encoderState == LOW) {
    if(timePeriod > 0.005){
      motor2Frequency = 1 / (timePeriod *20);
      motor2FrequencySmooth = (1 - SMOOTHING_FACTOR) * motor2FrequencySmooth + SMOOTHING_FACTOR * motor2Frequency;
    }
    motor2LastEncoderMillis = currentMillis;
  }
  else if(timePeriod > 1){
    motor2Frequency = 0;
    motor2FrequencySmooth = (1 - SMOOTHING_FACTOR) * motor2FrequencySmooth + SMOOTHING_FACTOR * motor2Frequency;
    motor2LastEncoderMillis = currentMillis;
  }
  motor2LastEncoderState = encoderState;
}

// PID Constants
const float KP = 0.3;   // Increased for faster response
const float KI = 0.1;   // Increased to reduce steady-state error
const float KD = 0.05;   // Adjusted for better damping

// PID Variables

float motor1LastError = 0.0;
float motor1Integral = 0.0;
unsigned long motor1LastPIDTime = 0;

// Add this function to calculate PID output
float calculateMotor1PID() {
    float error = motor1TargetFrequency - motor1FrequencySmooth;
    unsigned long currentTime = micros();
    float deltaTime = (currentTime - motor1LastPIDTime) / 1000000.0;// Convert to seconds
    
    // Only update if some time has passed
    if (deltaTime > 0) {
        motor1Integral += error * deltaTime;
        float derivative = (error - motor1LastError) / deltaTime;
        
        float output = (KP * error) + (KI * motor1Integral) + (KD * derivative);
        
        
        // Clamp output to valid range (0 to 1)
        output = constrain(output, 0.0, 1.0);
        
        motor1LastError = error;
        motor1LastPIDTime = currentTime;
        return output;
    }
    return 0;
}

float motor2LastError = 0.0;
float motor2Integral = 0.0;
unsigned long motor2LastPIDTime = 0;

float calculateMotor2PID() {
  float error = motor2TargetFrequency - motor2FrequencySmooth;
  unsigned long currentTime = micros();
  float deltaTime = (currentTime - motor2LastPIDTime) / 1000000.0;// Convert to seconds

  // Only update if some time has passed
  if (deltaTime > 0) {
    motor2Integral += error * deltaTime;
    float derivative = (error - motor2LastError) / deltaTime;


    float output = (KP * error) + (KI * motor2Integral) + (KD * derivative);

    // Clamp output to valid range (0 to 1)
    output = constrain(output, 0.0, 1.0);

    motor2LastError = error;
    motor2LastPIDTime = currentTime;
    return output;
  }
  return 0;
}

void setup_PID() {
  // Setup motor control pins and PWM
  pinMode(motor1DirectionPin, OUTPUT);
  pinMode(motor2DirectionPin, OUTPUT);
  configureMotor1Pin();
  configureMotor2Pin();

  // Set initial motor directions
  updateMotor1Direction();
  updateMotor2Direction();

  // Add this after the pinMode for encoder
  pinMode(motor1EncoderInputPin, INPUT_PULLUP);  // Enable internal pullup
  pinMode(motor2EncoderInputPin, INPUT_PULLUP);  // Enable internal pullup
}

void loop_PID() {
  // Add this at the start of loop() to print frequency

  // Replace direct speed setting with PID control
  float motor1PIDOutput = calculateMotor1PID();
  updateMotor1Speed(motor1PIDOutput);

  float motor2PIDOutput = calculateMotor2PID();
  updateMotor2Speed(motor2PIDOutput);

  readMotor1Encoder();
  readMotor2Encoder();

}