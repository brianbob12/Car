#ifndef TOPHAT_H
#define TOPHAT_H

#define TOPHAT_TRANSMISSION_FREQUENCY 2 //Hz

void setup_TopHat();
void loop_TopHat();

/**
 * Callback function for used packets
 * @param packets: number of packets used
 * since the last call to this function
 */
void on_used_packets(int packets);

#endif