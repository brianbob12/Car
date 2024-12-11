#include <Wire.h>

#define I2C_SLAVE_ADDR 0x28
#define SDA_PIN 5
#define SCL_PIN 4
int health = 100; // Default health value

void send_I2C_byte(uint8_t data) {
  // Send data to slave
  Wire.beginTransmission(I2C_SLAVE_ADDR);
  Wire.write(data);  // Send test data
  uint8_t error = Wire.endTransmission();

  if (error == 0) {
    Serial.println("Data sent successfully");
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

  // Check if health is zero
  if (health == 0) {
    stopRobot();
  }
}

void stopRobot() {
  Serial.println("Health is 0! Stopping all robot activities...");
  rgbLedWrite(2, 20, 0, 0);  // red LED to indicate stop
  while (1) {
    delay(1000); // Infinite loop to halt the robot
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

void loop_TopHat() {
  send_I2C_byte(0x55); // Example command (optional)
  receive_I2C_health(); // Read health value from slave

  delay(500); // Wait before next iteration
}
