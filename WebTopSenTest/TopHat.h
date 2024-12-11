#ifndef TOPHAT_H
#define TOPHAT_H

void TopHat_setup();
void TopHat_loop();

/**
 * Callback function for used packets
 * @param packets: number of packets used
 * since the last call to this function
 */
void on_used_packets(int packets);

#endif