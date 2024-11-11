#include <WiFi.h>
const char* ssid = "MotorControl";  // AP mode SSID, no password needed

// HTML content for web page with sliders for speed and buttons for direction
const char body[] PROGMEM = R"===( 
 <!DOCTYPE html>  
 <html><body>        
 <h1>Motor Control</h1>
 <p>Adjust the slider to set motor speed and use buttons to set direction.</p>
 <label for='speed'>Speed (0 to 100%): </label>
 <input type='range' id='speed' min='0' max='100' value='50' oninput='updateSpeed()'><span id='speedValue'>50</span>%<br><br>
 <button onclick="updateDirection(1)">Forward</button>
 <button onclick="updateDirection(0)">Reverse</button>
 <script>
   function updateSpeed() {
     let speed = document.getElementById('speed').value;
     document.getElementById('speedValue').innerText = speed + '%';
     fetch(`/update?speed=${speed}`);
   }
   function updateDirection(direction) {
     fetch(`/update?direction=${direction}`);
   }
 </script>
 </body></html>  
)===";

WiFiServer server(80);    // HTTP server on port 80

const int motorPin1 = 6;   // Connected to IN1 on SN754410
const int pwmPin = 0;      // Connected to EN1 on SN754410
const int interrupterPin = 7; // Pin connected to optical interrupter sensor

//const int motorPin1 = 4;   // Connected to IN1 on SN754410
//const int pwmPin = 1;      // Connected to EN1 on SN754410
//const int interrupterPin = 5; // Pin connected to optical interrupter sensor

const int resolution = 14; // Resolution for PWM duty cycle (14-bit)

int dutyCycle = 8191;      // Default duty cycle (50% of 16383)
bool motorDirection = true; // true = forward, false = reverse
int counts = 0;             // Count of rotations
unsigned long lastTime = 0; // Last time the speed was calculated
bool lastInterrupterState = HIGH;  // Last state of the interrupter sensor

void updateMotorDirection() {
  if (motorDirection) {
    digitalWrite(motorPin1, HIGH);
  } else {
    digitalWrite(motorPin1, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, "");  // Start WiFi in AP mode, no password required
  Serial.println(ssid);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();

  // Setup motor control pins and PWM
  pinMode(motorPin1, OUTPUT);
  ledcAttach(pwmPin, 6, resolution);        // Attach motor PWM pin to channel 0
  ledcWrite(pwmPin, dutyCycle);         // Set initial duty cycle
  // Setup interrupter for counting
  pinMode(interrupterPin, INPUT_PULLUP);
  updateMotorDirection();
  lastTime = millis();
}

void loop() {
  WiFiClient client = server.accept();  // Check for incoming client connection
  if (client) {
    String currentLine = "";               // String to hold incoming data
    while (client.connected()) {
      if (client.available()) {            // If there are bytes to read
        char c = client.read();            // Read a byte
        if (c == '\n') {
          // If the line is blank, end of client request
          if (currentLine.length() == 0) {
            client.print(body);            // Send HTML page to client
            break;
          } else {
            // Handle GET request to update speed or direction
            if (currentLine.startsWith("GET /update?")) {
              if (currentLine.indexOf("speed=") > -1) {
                // Parse speed from the request and map to 14-bit range
                int newSpeed = currentLine.substring(currentLine.indexOf("speed=") + 6).toInt();
                dutyCycle = map(newSpeed, 0, 100, 0, 16360);
                ledcWrite(pwmPin, dutyCycle);   // Update PWM duty cycle
                Serial.printf("Updated speed: %d%%, duty cycle: %d\n", newSpeed, dutyCycle);
              }
              if (currentLine.indexOf("direction=") > -1) {
                // Parse direction from the request
                int newDirection = currentLine.substring(currentLine.indexOf("direction=") + 10).toInt();
                motorDirection = newDirection == 1;
                updateMotorDirection();
                Serial.printf("Updated direction: %s\n", motorDirection ? "Forward" : "Reverse");
              }
            }
            currentLine = "";               // Clear the line for the next input
          }
        } else if (c != '\r') {
          currentLine += c;                // Add character to current line
        }
      }
    }
    client.stop();  // Close the connection with the client
  }

  // Read the interrupter pin and count rotations based on state change
  bool interrupterState = digitalRead(interrupterPin);
  if (lastInterrupterState == HIGH && interrupterState == LOW) {
    // Count a rotation when the interrupter changes from HIGH to LOW
    counts++;
  }
  lastInterrupterState = interrupterState;

  // Calculate and print speed every 1 second
  if (millis() - lastTime >= 1000) {
    int speed = counts; // counts per second as speed measure
    counts = 0; // Reset counts for the next interval
    lastTime = millis();
    Serial.printf("Speed: %d counts/sed, Direction: %s\n", speed, motorDirection ? "Forward" : "Reverse");
  }
}
