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

// Motor 1 pins
const int motor1DirectionPin = 6;
const int motor1PWMpin = 0;
const int motor1EncoderInputPin = 5;

// // Motor 2 pins
// const int motor2Pin1 = 4;
// const int pwmPin2 = 1;

const int resolution = 8; // Resolution for PWM duty cycle (8-bit) max: 2^8 - 1 = 255
int maxDutyCycle = pow(2, resolution) - 1;
ledc_channel_t motor1Channel = LEDC_CHANNEL_0;
ledc_timer_t motor1Timer = LEDC_TIMER_0;
ledc_channel_t motor2Channel = LEDC_CHANNEL_1;
ledc_timer_t motor2Timer = LEDC_TIMER_1;

// Add these global variables near the top with other globals
volatile unsigned long lastPulseTime = 0;
volatile unsigned long pulseCount = 0;
volatile float frequency = 0;
const unsigned long FREQUENCY_UPDATE_INTERVAL = 100; // Update frequency every 100ms

// Add this ISR function before setup()
void IRAM_ATTR encoderISR() {
  unsigned long currentTime = micros();
  if (lastPulseTime != 0) {
    // Calculate instantaneous frequency (in Hz)
    // 1000000 converts micros to seconds
    frequency = 1000000.0 / (currentTime - lastPulseTime);
  }
  lastPulseTime = currentTime;
  pulseCount++;
}

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

// void configureMotor2Pin(){
//    ledc_timer_config_t motor2TimerConfig = {
//     .speed_mode = LEDC_LOW_SPEED_MODE,
//     .duty_resolution = LEDC_TIMER_8_BIT,
//     .timer_num = motor2Timer,
//     .freq_hz = PWM_FREQ,
//     .clk_cfg = LEDC_AUTO_CLK
//   };
//   ledc_timer_config(&motor2TimerConfig);

//   ledc_channel_config_t motor2ChannelConfig = {
//     .gpio_num = motor1Pin1,
//     .speed_mode = LEDC_LOW_SPEED_MODE,
//     .channel = motor2Channel,
//     .timer_sel = motor2Timer,
//     .duty = 0,
//     .hpoint = 0
//   };
//   ledc_channel_config(&motor2ChannelConfig);
// }


// int dutyCycle2 = 8191;     // Default duty cycle for motor 2 (50%)
bool motorDirection1 = true; // Motor 1 direction
// bool motorDirection2 = true; // Motor 2 direction

void updateMotor1Direction() {
  digitalWrite(motor1DirectionPin, motorDirection1 ? HIGH : LOW);
}

//valid speeds are 0 to 1
void updateMotor1Speed(float speed) {
  int dutyCycle = speed * maxDutyCycle;
  Serial.printf("Motor 1 raw speed: %f, duty cycle: %d\n", speed, dutyCycle);
  ledc_set_duty(LEDC_LOW_SPEED_MODE, motor1Channel, dutyCycle);
  ledc_update_duty(LEDC_LOW_SPEED_MODE, motor1Channel);
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
  // configureMotor2Pin();

  // Set initial motor directions
  updateMotor1Direction();

  // Add this after the pinMode for encoder
  pinMode(motor1EncoderInputPin, INPUT_PULLUP);  // Enable internal pullup
  attachInterrupt(motor1EncoderInputPin, encoderISR, RISING);  // Trigger on rising edge
}

void loop() {
  // Add this at the start of loop() to print frequency

  Serial.printf("Encoder Frequency: %.2f Hz, Pulse Count: %lu\n", frequency, pulseCount);
  updateMotor1Speed(0);
  delay(5000);

  Serial.printf("Encoder Frequency: %.2f Hz, Pulse Count: %lu\n", frequency, pulseCount);
  updateMotor1Speed(0.25);
  delay(5000);

  Serial.printf("Encoder Frequency: %.2f Hz, Pulse Count: %lu\n", frequency, pulseCount);
  updateMotor1Speed(0.5);
  delay(5000);

  Serial.printf("Encoder Frequency: %.2f Hz, Pulse Count: %lu\n", frequency, pulseCount);
  updateMotor1Speed(0.75);
  delay(5000);

  Serial.printf("Encoder Frequency: %.2f Hz, Pulse Count: %lu\n", frequency, pulseCount);
  updateMotor1Speed(1);
  delay(5000);

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