#include <Wire.h>



int health = 100; // Default health value

/**
 * Get the health of the robot
 * @return health: health of the robot
 */
int get_health(){
  return health;
}

void send_I2C_byte(uint8_t data) {
  // Send data to slave
  Wire.beginTransmission(I2C_SLAVE_ADDR);
  Wire.write(data);  // Send test data
  uint8_t error = Wire.endTransmission();

  if (error == 0) {
    //Serial.println("Data sent successfully");
    rgbLedWrite(2, 0, 20, 0);  // green
  } else {
    Serial.printf("Error sending data: %d\n", error);
    rgbLedWrite(2, 20, 0, 0);  // red
  }
}

void receive_I2C_health() {
  // Request health value from the slave
  uint8_t bytesReceived = Wire.requestFrom(I2C_SLAVE_ADDR, 1);
  if (bytesReceived > 0) {
    health = Wire.read(); // Read the health value
    Serial.printf("Health received: %d\n", health);
  } else {
    Serial.println("Failed to receive health value");
  }

  
}

//stops the robot until the health is not 0
void stopRobot() {
  Serial.println("Health is 0! Stopping all robot activities...");
  rgbLedWrite(2, 20, 0, 0);  // red LED to indicate stop
  while (health == 0) {
    delay(500); // Infinite loop to halt the robot
    receive_I2C_health();
  }
}

int used_packets = 0;

void on_used_packets(int packets) {
  used_packets += packets;
}

void setup_TopHat() {
  Serial.begin(115200);

  // Initialize I2C master
  Wire.begin(SDA_PIN, SCL_PIN, 40000);
  Serial.println("ESP32-C3 I2C Master initialized");
  Serial.printf("SDA: %d, SCL: %d\n", SDA_PIN, SCL_PIN);
}

int time_of_last_transmission_millis = 0;

void send_data_to_tophat() {
  send_I2C_byte(used_packets);
  receive_I2C_health();
  // Check if health is zero
  if (health == 0) {
    stopRobot();
  }
  used_packets = 0;
  time_of_last_transmission_millis = millis();
}

void loop_TopHat() {
  if (millis() - time_of_last_transmission_millis > 1000 / TOPHAT_TRANSMISSION_FREQUENCY) {
    send_data_to_tophat();
  }
}

