#include <WiFi.h>
#include "driver/ledc.h"
const char* ssid = "MotorControl";  // AP mode SSID, no password needed

// HTML content for web page with sliders for speed and buttons for direction
const char body[] PROGMEM = R"===( 
<!DOCTYPE html>  
<html><body>        
<h1>Motor Control</h1>
<p>Adjust the sliders to set each motor's speed and use buttons to set direction.</p>

<!-- Motor 1 Controls -->
<h2>Motor 1</h2>
<label for='speed1'>Speed (0 to 100%): </label>
<input type='range' id='speed1' min='0' max='100' value='50' oninput='updateSpeed(1)'><span id='speedValue1'>50</span>%<br><br>
<button onclick="updateDirection(1, 1)">Forward</button>
<button onclick="updateDirection(1, 0)">Reverse</button>

<!-- Motor 2 Controls -->
<h2>Motor 2</h2>
<label for='speed2'>Speed (0 to 100%): </label>
<input type='range' id='speed2' min='0' max='100' value='50' oninput='updateSpeed(2)'><span id='speedValue2'>50</span>%<br><br>
<button onclick="updateDirection(2, 1)">Forward</button>
<button onclick="updateDirection(2, 0)">Reverse</button>

<script>
  function updateSpeed(motor) {
    let speed = document.getElementById('speed' + motor).value;
    document.getElementById('speedValue' + motor).innerText = speed + '%';
    fetch(`/update?motor=${motor}&speed=${speed}`);
  }
  function updateDirection(motor, direction) {
    fetch(`/update?motor=${motor}&direction=${direction}`);
  }
</script>
</body></html>  
)===";

WiFiServer server(80);    // HTTP server on port 80

const int PWM_FREQ = 500;

// Smoothing using an exponential moving average
//smoothing factor of 0 is not smoothing and 1 is max smoothing(constant value)
const float SMOOTHING_FACTOR = 0.5;

// Motor 1 pins
const int motor1DirectionPin = 6;
const int motor1PWMpin = 0;
const int motor1EncoderInputPin = 7;//sometimes 5
unsigned long motor1LastEncoderTime = millis();
unsigned long motor1LastEncoderMillis = 0;
bool motor1LastEncoderState = HIGH;
float motor1Frequency = 0;
float motor1FrequencySmooth = 0;

// Motor 2 pins
const int motor2DirectionPin = 4;
const int motor2PWMpin = 1;
const int motor2EncoderInputPin = 5;//sometimes 7
unsigned long motor2LastEncoderTime = millis();
unsigned long motor2LastEncoderMillis = 0;
bool motor2LastEncoderState = HIGH;
float motor2Frequency = 0;
float motor2FrequencySmooth = 0;



const int resolution = 8; // Resolution for PWM duty cycle (8-bit) max: 2^8 - 1 = 255
int maxDutyCycle = pow(2, resolution) - 1;
ledc_channel_t motor1Channel = LEDC_CHANNEL_0;
ledc_timer_t motor1Timer = LEDC_TIMER_0;
ledc_channel_t motor2Channel = LEDC_CHANNEL_1;
ledc_timer_t motor2Timer = LEDC_TIMER_1;

void configureMotor1Pin(){
  ledc_timer_config_t motor1TimerConfig = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_8_BIT,
    .timer_num = motor1Timer,
    .freq_hz = PWM_FREQ,
    .clk_cfg = LEDC_AUTO_CLK
  };
  ledc_timer_config(&motor1TimerConfig);

  ledc_channel_config_t motor1ChannelConfig = {
    .gpio_num = motor1PWMpin,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = motor1Channel,
    .timer_sel = motor1Timer,
    .duty = 0,
    .hpoint = 0
  };
  ledc_channel_config(&motor1ChannelConfig);
}

void configureMotor2Pin(){
   ledc_timer_config_t motor2TimerConfig = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_8_BIT,
    .timer_num = motor2Timer,
    .freq_hz = PWM_FREQ,
    .clk_cfg = LEDC_AUTO_CLK
  };
  ledc_timer_config(&motor2TimerConfig);

  ledc_channel_config_t motor2ChannelConfig = {
    .gpio_num = motor2PWMpin,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = motor2Channel,
    .timer_sel = motor2Timer,
    .duty = 0,
    .hpoint = 0
  };
  ledc_channel_config(&motor2ChannelConfig);
}


// int dutyCycle2 = 8191;     // Default duty cycle for motor 2 (50%)
bool motor1Direction = true; // Motor 1 direction
bool motor2Direction = true; // Motor 2 direction

void updateMotor1Direction() {
  digitalWrite(motor1DirectionPin, motor1Direction ? HIGH : LOW);
}

void updateMotor2Direction() {
  digitalWrite(motor2DirectionPin, motor2Direction ? HIGH : LOW);
}

//valid speeds are 0 to 1
void updateMotor1Speed(float speed) {
  int dutyCycle = speed * maxDutyCycle;
  ledc_set_duty(LEDC_LOW_SPEED_MODE, motor1Channel, dutyCycle);
  ledc_update_duty(LEDC_LOW_SPEED_MODE, motor1Channel);
}

void updateMotor2Speed(float speed) {
  int dutyCycle = speed * maxDutyCycle;
  ledc_set_duty(LEDC_LOW_SPEED_MODE, motor2Channel, dutyCycle);
  ledc_update_duty(LEDC_LOW_SPEED_MODE, motor2Channel);
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
const float KP = 0.6;   // Increased for faster response
const float KI = 0.15;   // Increased to reduce steady-state error
const float KD = 0.08;   // Adjusted for better damping

// PID Variables
float motor1TargetFrequency = 1;  // Target frequency in Hz
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

        if(timeToPrint()){
          Serial.printf("Error: %.2f, Integral: %.2f, Derivative: %.2f\n", error, motor1Integral, derivative);
        }
        
        float output = (KP * error) + (KI * motor1Integral) + (KD * derivative);
        
        if(timeToPrint()){
          Serial.printf("Raw Output: %.2f\n", output);
        }
        
        // Clamp output to valid range (0 to 1)
        output = constrain(output, 0.0, 1.0);
        
        motor1LastError = error;
        motor1LastPIDTime = currentTime;
        return output;
    }
    Serial.printf("No time passed\n");
    return 0;
}

float motor2TargetFrequency = 1;
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

    if(timeToPrint()){
      Serial.printf("Error: %.2f, Integral: %.2f, Derivative: %.2f\n", error, motor2Integral, derivative);
    }

    float output = (KP * error) + (KI * motor2Integral) + (KD * derivative);

    if(timeToPrint()){
      Serial.printf("Raw Output: %.2f\n", output);
    }

    // Clamp output to valid range (0 to 1)
    output = constrain(output, 0.0, 1.0);

    motor2LastError = error;
    motor2LastPIDTime = currentTime;
    return output;
  }
  Serial.printf("No time passed\n");
  return 0;
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, "");  // Start WiFi in AP mode, no password required
  Serial.println(ssid);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();

  // Setup motor control pins and PWM
  pinMode(motor1DirectionPin, OUTPUT);
  configureMotor1Pin();
  configureMotor2Pin();

  // Set initial motor directions
  updateMotor1Direction();
  updateMotor2Direction();
  // Add this after the pinMode for encoder
  pinMode(motor1EncoderInputPin, INPUT_PULLUP);  // Enable internal pullup
  pinMode(motor2EncoderInputPin, INPUT_PULLUP);  // Enable internal pullup
}

int lastPrintTime = millis();
bool timeToPrint(){
  return millis() - lastPrintTime > 1000;
}

void loop() {
  // Add this at the start of loop() to print frequency

  // Replace direct speed setting with PID control
  float motor1PIDOutput = calculateMotor1PID();
  updateMotor1Speed(motor1PIDOutput);

  float motor2PIDOutput = calculateMotor2PID();
  updateMotor2Speed(motor2PIDOutput);

  readMotor1Encoder();
  readMotor2Encoder();

  if(timeToPrint()){
    Serial.printf("Motor 1 Frequency: %.2f Hz\n", motor1Frequency);
    Serial.printf("Motor 1 Frequency Smooth: %.2f Hz\n", motor1FrequencySmooth);
    Serial.printf("Motor 1 PID Output: %.2f\n", motor1PIDOutput);

    Serial.printf("Motor 2 Frequency: %.2f Hz\n", motor2Frequency);
    Serial.printf("Motor 2 Frequency Smooth: %.2f Hz\n", motor2FrequencySmooth);
    Serial.printf("Motor 2 PID Output: %.2f\n", motor2PIDOutput);

    lastPrintTime = millis();
  }
  // WiFiClient client = server.accept();  // Check for incoming client connection
  // if (client) {
  //   String currentLine = "";               // String to hold incoming data
  //   while (client.connected()) {
  //     if (client.available()) {            // If there are bytes to read
  //       char c = client.read();            // Read a byte
  //       if (c == '\n') {
  //         if (currentLine.length() == 0) {
  //           client.print(body);            // Send HTML page to client
  //           break;
  //         } else {
  //           // Handle GET request to update speed or direction
  //           if (currentLine.startsWith("GET /update?")) {
  //             int motor = -1;
  //             if (currentLine.indexOf("motor=") > -1) {
  //               motor = currentLine.substring(currentLine.indexOf("motor=") + 6).toInt();
  //             }
              
  //             if (currentLine.indexOf("speed=") > -1) {
  //               int newSpeed = currentLine.substring(currentLine.indexOf("speed=") + 6).toInt();
  //               int newDutyCycle = map(newSpeed, 0, 100, 0, 16360);
                
  //               if (motor == 1) {
  //                 dutyCycle1 = newDutyCycle;
  //                 ledcWrite(pwmPin1, dutyCycle1);
  //                 Serial.printf("Motor 1 updated speed: %d%%, duty cycle: %d\n", newSpeed, dutyCycle1);
  //               } else if (motor == 2) {
  //                 dutyCycle2 = newDutyCycle;
  //                 ledcWrite(pwmPin2, dutyCycle2);
  //                 Serial.printf("Motor 2 updated speed: %d%%, duty cycle: %d\n", newSpeed, dutyCycle2);
  //               }
  //             }

  //             if (currentLine.indexOf("direction=") > -1) {
  //               int newDirection = currentLine.substring(currentLine.indexOf("direction=") + 10).toInt();
  //               if (motor == 1) {
  //                 motorDirection1 = newDirection == 1;
  //                 updateMotorDirection(1);
  //                 Serial.printf("Motor 1 updated direction: %s\n", motorDirection1 ? "Forward" : "Reverse");
  //               } else if (motor == 2) {
  //                 motorDirection2 = newDirection == 1;
  //                 updateMotorDirection(2);
  //                 Serial.printf("Motor 2 updated direction: %s\n", motorDirection2 ? "Forward" : "Reverse");
  //               }
  //             }
  //           }
  //           currentLine = "";               // Clear the line for the next input
  //         }
  //       } else if (c != '\r') {
  //         currentLine += c;                // Add character to current line
  //       }
  //     }
  //   }
  //   client.stop();  // Close the connection with the client
  // }
}