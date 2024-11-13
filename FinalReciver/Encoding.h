#ifndef ENCODING_H
#define ENCODING_H

// Maximum frequency that can be encoded (3 Hz maps to 255)
#define FREQUENCY_MAX 3

// Function declarations
int encodeFrequency(float frequency);
float decodeFrequency(int encodedFrequency);
bool decodeLeftDirection(int directions);
bool decodeRightDirection(int directions);
int encodeDirections(bool leftDirection, bool rightDirection);

#endif