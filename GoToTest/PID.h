#ifndef PID_H
#define PID_H

// Function declarations
void setup_PID();
void loop_PID();

/*
Motor struct

Contains all the variables needed for PID control of a motor
*/
struct Motor {
    // Pin configuration
    int encoderPin;
    int directionPin;
    int inverseDirectionPin;
    int pwmPin;
    
    // State variables
    unsigned long lastEncoderTime;
    unsigned long lastEncoderMillis;
    bool lastEncoderState;
    float frequency;
    float frequencySmooth;
    bool direction;
    float targetFrequency;
    int encoderRunningCount;
    int encoderRunningCountStartMillis;
    
    // PID variables
    float lastError;
    float integral;
    unsigned long lastPIDTime;
    float pidOutput;
};

void setMotorDirection(Motor &motor, bool direction);
void setMotorTargetFrequency(Motor &motor, float frequency);

void resetPID(Motor &motor);

#define SMOOTHING_FACTOR 0.2
#define ENCODER_GUARD 0.00 //the ammount of time in seconds under which we ignore encoder signals
//Through experimentation, this should be zero

//This was supposed to be 11 (from docs), but we observed it's much higher
#define ENCODER_STEPS_PER_REVOLUTION 300

//PID Constants
#define KP 0.6
#define KI 0.06
#define KD 0.1


#define MOTOR1_ENCODER_INPUT_PIN 41
#define MOTOR1_DIRECTION_PIN 39
#define MOTOR1_INVERSE_DIRECTION_PIN 40
#define MOTOR1_PWM_PIN 38


// Define motor1 as a struct instance
Motor motor1 = {
    .encoderPin = MOTOR1_ENCODER_INPUT_PIN,
    .directionPin = MOTOR1_DIRECTION_PIN,
    .inverseDirectionPin = MOTOR1_INVERSE_DIRECTION_PIN,
    .pwmPin = MOTOR1_PWM_PIN,
    .lastEncoderTime = 0,
    .lastEncoderMillis = 0,
    .lastEncoderState = HIGH,
    .frequency = 0,
    .frequencySmooth = 0,
    .direction = true,
    .targetFrequency = 1,
    .encoderRunningCount = 0,
    .encoderRunningCountStartMillis = 0,
    .lastError = 0,
    .integral = 0,
    .lastPIDTime = 0,
    .pidOutput = 0
};

#define MOTOR2_ENCODER_INPUT_PIN 36
#define MOTOR2_DIRECTION_PIN 35
#define MOTOR2_INVERSE_DIRECTION_PIN 34
#define MOTOR2_PWM_PIN 33

Motor motor2 = {
    .encoderPin = MOTOR2_ENCODER_INPUT_PIN,
    .directionPin = MOTOR2_DIRECTION_PIN,
    .inverseDirectionPin = MOTOR2_INVERSE_DIRECTION_PIN,
    .pwmPin = MOTOR2_PWM_PIN,
    .lastEncoderTime = 0,
    .lastEncoderMillis = 0,
    .lastEncoderState = HIGH,
    .frequency = 0,
    .frequencySmooth = 0,
    .direction = true,
    .targetFrequency = 1,
    .encoderRunningCount = 0,
    .encoderRunningCountStartMillis = 0,
    .lastError = 0,
    .integral = 0,
    .lastPIDTime = 0,
    .pidOutput = 0
};

#define MOTOR3_ENCODER_INPUT_PIN 17
#define MOTOR3_DIRECTION_PIN 8
#define MOTOR3_INVERSE_DIRECTION_PIN 3
#define MOTOR3_PWM_PIN 9

Motor motor3 = {
    .encoderPin = MOTOR3_ENCODER_INPUT_PIN,
    .directionPin = MOTOR3_DIRECTION_PIN,
    .inverseDirectionPin = MOTOR3_INVERSE_DIRECTION_PIN,
    .pwmPin = MOTOR3_PWM_PIN,
    .lastEncoderTime = 0,
    .lastEncoderMillis = 0,
    .lastEncoderState = HIGH,
    .frequency = 0,
    .frequencySmooth = 0,
    .direction = true,
    .targetFrequency = 1,
    .encoderRunningCount = 0,
    .encoderRunningCountStartMillis = 0,
    .lastError = 0,
    .integral = 0,
    .lastPIDTime = 0,
    .pidOutput = 0
};

#define MOTOR4_ENCODER_INPUT_PIN 11
#define MOTOR4_DIRECTION_PIN 13
#define MOTOR4_INVERSE_DIRECTION_PIN 12
#define MOTOR4_PWM_PIN 14

Motor motor4 = {
    .encoderPin = MOTOR4_ENCODER_INPUT_PIN,
    .directionPin = MOTOR4_DIRECTION_PIN,
    .inverseDirectionPin = MOTOR4_INVERSE_DIRECTION_PIN,
    .pwmPin = MOTOR4_PWM_PIN,
    .lastEncoderTime = 0,
    .lastEncoderMillis = 0,
    .lastEncoderState = HIGH,
    .frequency = 0,
    .frequencySmooth = 0,
    .direction = true,
    .targetFrequency = 1,
    .encoderRunningCount = 0,
    .encoderRunningCountStartMillis = 0,
    .lastError = 0,
    .integral = 0,
    .lastPIDTime = 0,
    .pidOutput = 0
};


#endif