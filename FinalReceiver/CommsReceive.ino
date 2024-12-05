#include "CommsReceive.h"

#define COMMS_BAUD_PERIOD 10000    //us
#define DIGITAL_LOW_WIDTH 3000     //us
#define DIGITAL_HIGH_WIDTH 6000    //us
#define DELTA_TIME_TOLERANCE 1500  //us

#define TIME_BETWEEN_MESSAGES 3 * COMMS_BAUD_PERIOD
#define TIME_BETWEEN_MESSAGES_TOLERANCE 100  //us
#define EFFECTIVE_TIME_BETWEEN_MESSAGES TIME_BETWEEN_MESSAGES - TIME_BETWEEN_MESSAGES_TOLERANCE

#define MESSAGE_INT_LENGTH 3
#define MESSAGE_BIT_LENGTH MESSAGE_INT_LENGTH * 8

#define DIGITAL_NOISE_FILTER_THRESHOLD 300  //us

const int commsPin = 10;

int message[MESSAGE_INT_LENGTH] = { 0, 0, 0 };
int messageIndex = 0;

int commsPinLastState = LOW;
int commsPinRisingEdgeTime = 0;   //in us
int commsPinFallingEdgeTime = 0;  //in us

int setBit(int originalBit, bool newBit, int index) {
  if (newBit) {
    return originalBit | 1 << index;
  } else {
    return originalBit & ~(1 << index);
  }
}

#define NOISE_FILTER_SAMPLE_COUNT 20

int samples[NOISE_FILTER_SAMPLE_COUNT];
int sampleIndex = 0;

void clearSamples() {
  for (int i = 0; i < NOISE_FILTER_SAMPLE_COUNT; i++) {
    samples[i] = 0;
  }
}

//returns the majority vote of the last n samples
bool noiseFilterReadCommsPin() {
  bool currentState = digitalRead(commsPin);

  samples[sampleIndex] = currentState;
  sampleIndex++;
  sampleIndex %= NOISE_FILTER_SAMPLE_COUNT;

  // Count HIGH samples and return true if majority are HIGH
  int trueCount = 0;
  for (int i = 0; i < NOISE_FILTER_SAMPLE_COUNT; i++) {
    if (samples[i]) {
      trueCount++;
    }
  }
  return trueCount > NOISE_FILTER_SAMPLE_COUNT / 2;
}

void setup_commsReceive() {
  clearSamples();
  pinMode(commsPin, INPUT);
}


void onMessageReceived() {
  onMessageReceivedExternal(message);
}

// Processes each received bit and assembles it into the message array
void onBitReceived(bool bit) {
  message[messageIndex / 8] = setBit(message[messageIndex / 8], bit, messageIndex % 8);
  messageIndex++;
  if (messageIndex == MESSAGE_BIT_LENGTH) {  // Message complete
    messageIndex = 0;
    onMessageReceived();
  }
}

// Statistics for debugging communication issues
int invalidPulseWidthCount = 0;
int lastInvalidPulseWidth = 0;
int sumOfInvalidPulseWidths = 0;

// Main communication reading function - detects edges and decodes bits
void readCommsPin() {
  int commsPinState = noiseFilterReadCommsPin();
  if (commsPinState != commsPinLastState) {  // Edge detected
    if (commsPinState == HIGH) {  // Rising edge
      //Rising edge
      commsPinRisingEdgeTime = micros();
      int lowWidth = commsPinRisingEdgeTime - commsPinFallingEdgeTime;

      if (lowWidth > EFFECTIVE_TIME_BETWEEN_MESSAGES) {
        //New message
        messageIndex = 0;
      }
    } else {  // Falling edge
      // Decode bit based on pulse width
      commsPinFallingEdgeTime = micros();
      int pulseWidth = micros() - commsPinRisingEdgeTime;

      // Check if pulse width matches expected durations for 1 or 0
      if (
        pulseWidth > DIGITAL_HIGH_WIDTH - DELTA_TIME_TOLERANCE && pulseWidth < DIGITAL_HIGH_WIDTH + DELTA_TIME_TOLERANCE) {
        onBitReceived(true);  // Long pulse = 1
      } else if (
        pulseWidth > DIGITAL_LOW_WIDTH - DELTA_TIME_TOLERANCE && pulseWidth < DIGITAL_LOW_WIDTH + DELTA_TIME_TOLERANCE) {
        onBitReceived(false);  // Short pulse = 0
      } else {
        // Track invalid pulses for debugging
        invalidPulseWidthCount++;
        lastInvalidPulseWidth = pulseWidth;
        sumOfInvalidPulseWidths += pulseWidth;
      }
    }
    commsPinLastState = commsPinState;
  }
}

void loop_commsReceive() {
  readCommsPin();
}

