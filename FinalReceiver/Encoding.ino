#include "Encoding.h"

int encodeFrequency(float frequency){
  return (int)(frequency * 255.0 / FREQUENCY_MAX);
}

float decodeFrequency(int encodedFrequency){
  return (float)(encodedFrequency * FREQUENCY_MAX / 255);
}

bool decodeLeftDirection(int directions){
  return directions & 1;
}

bool decodeRightDirection(int directions){
  return directions & 2;
}

int encodeDirections(bool leftDirection, bool rightDirection){
  return (leftDirection ? 1 : 0) | (rightDirection ? 2 : 0);
}