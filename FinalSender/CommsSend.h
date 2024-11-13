#ifndef COMMS_SEND_H
#define COMMS_SEND_H

#define MESSAGE_INT_LENGTH 3

// Function declarations
void setup_commsSend();
void loop_commsSend();

void setNextMessage(int messageToSend[MESSAGE_INT_LENGTH]);


//There must be a global function called onMessageCompleteExternal(messageJustSent[MESSAGE_INT_LENGTH])

#endif