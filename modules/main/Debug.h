#ifndef Debug_h
#define Debug_h

#define DEBUG_ON 1
#define DEBUG_OFF 0
const byte debugMode = DEBUG_ON;
#define DBG(...) debugMode == DEBUG_ON ? Serial.println(__VA_ARGS__) : NULL
#define DBG_PRNT(...) debugMode == DEBUG_ON ? Serial.print(__VA_ARGS__) : NULL
#define DBG_WRT(...) debugMode == DEBUG_ON ? Serial.write(__VA_ARGS__) : NULL

#define DBGP(...) debugMode == DEBUG_ON ? _stream->println(__VA_ARGS__) : NULL
#define DBGP_PRNT(...) debugMode == DEBUG_ON ? _stream->print(__VA_ARGS__) : NULL
#define DBGP_WRT(...) debugMode == DEBUG_ON ? _stream->write(__VA_ARGS__) : NULL

#endif