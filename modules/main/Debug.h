#ifndef Debug_h
#define Debug_h

#define DEBUG_ON 1
#define DEBUG_OFF 0
const byte debugMode = DEBUG_OFF;
#define DBG(...) debugMode == DEBUG_ON ? Serial.println(__VA_ARGS__) : NULL
#define DBG_PRNT(...) debugMode == DEBUG_ON ? Serial.print(__VA_ARGS__) : NULL
#define DBG_WRT(...) debugMode == DEBUG_ON ? Serial.write(__VA_ARGS__) : NULL

#endif