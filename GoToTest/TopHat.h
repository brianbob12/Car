#ifndef TOPHAT_H
#define TOPHAT_H

#define TOPHAT_TRANSMISSION_FREQUENCY 2 //Hz

#define I2C_SLAVE_ADDR 0x28
#define SDA_PIN 5
#define SCL_PIN 4

void setup_TopHat();
void loop_TopHat();

/**
 * Callback function for used packets
 * @param packets: number of packets used
 * since the last call to this function
 */
void on_used_packets(int packets);

/**
 * Get the health of the robot
 * @return health: health of the robot
 */
int get_health();

#endif