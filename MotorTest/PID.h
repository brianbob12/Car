#ifndef PID_H
#define PID_H

// Function declarations
void setup_PID();
void loop_PID();

void setMotorDirection(Motor &motor, bool direction);
void setMotorTargetFrequency(Motor &motor, float frequency);

#define SMOOTHING_FACTOR 0.5

#define ENCODER_STEPS_PER_REVOLUTION 11

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
    
    // PID variables
    float lastError;
    float integral;
    unsigned long lastPIDTime;
    float pidOutput;
};

#define MOTOR1_ENCODER_INPUT_PIN 4
#define MOTOR1_DIRECTION_PIN 5
#define MOTOR1_INVERSE_DIRECTION_PIN 6
#define MOTOR1_PWM_PIN 7

// Define motor1 as a struct instance
Motor motor1 = {
    MOTOR1_ENCODER_INPUT_PIN,
    MOTOR1_DIRECTION_PIN,
    MOTOR1_INVERSE_DIRECTION_PIN,
    MOTOR1_PWM_PIN,
    0, // lastEncoderTime
    0, // lastEncoderMillis
    HIGH, // lastEncoderState
    0, // frequency
    0, // frequencySmooth
    true, // direction
    1, // targetFrequency
    0, // lastError
    0, // integral
    0, // lastPIDTime
    0  // pidOutput
};

#define MOTOR2_ENCODER_INPUT_PIN 8
#define MOTOR2_DIRECTION_PIN 9
#define MOTOR2_INVERSE_DIRECTION_PIN 10
#define MOTOR2_PWM_PIN 11

Motor motor2 = {
    MOTOR2_ENCODER_INPUT_PIN,
    MOTOR2_DIRECTION_PIN,
    MOTOR2_INVERSE_DIRECTION_PIN,
    MOTOR2_PWM_PIN,
    0, // lastEncoderTime
    0, // lastEncoderMillis
    HIGH, // lastEncoderState
    0, // frequency
    0, // frequencySmooth
    true, // direction
    1, // targetFrequency
    0, // lastError
    0, // integral
    0, // lastPIDTime
    0  // pidOutput
};

#define MOTOR3_ENCODER_INPUT_PIN 12
#define MOTOR3_DIRECTION_PIN 13
#define MOTOR3_INVERSE_DIRECTION_PIN 14
#define MOTOR3_PWM_PIN 15

Motor motor3 = {
    MOTOR3_ENCODER_INPUT_PIN,
    MOTOR3_DIRECTION_PIN,
    MOTOR3_INVERSE_DIRECTION_PIN,
    MOTOR3_PWM_PIN,
    0, // lastEncoderTime
    0, // lastEncoderMillis
    HIGH, // lastEncoderState
    0, // frequency
    0, // frequencySmooth
    true, // direction
    1, // targetFrequency
    0, // lastError
    0, // integral
    0, // lastPIDTime
    0  // pidOutput
};

#define MOTOR4_ENCODER_INPUT_PIN 16
#define MOTOR4_DIRECTION_PIN 17
#define MOTOR4_INVERSE_DIRECTION_PIN 18
#define MOTOR4_PWM_PIN 19

Motor motor4 = {
    MOTOR4_ENCODER_INPUT_PIN,
    MOTOR4_DIRECTION_PIN,
    MOTOR4_INVERSE_DIRECTION_PIN,
    MOTOR4_PWM_PIN,
    0, // lastEncoderTime
    0, // lastEncoderMillis
    HIGH, // lastEncoderState
    0, // frequency
    0, // frequencySmooth
    true, // direction
    1, // targetFrequency
    0, // lastError
    0, // integral
    0, // lastPIDTime
    0  // pidOutput
};


#endif