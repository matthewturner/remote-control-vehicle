#ifndef DebugS_h
#define DebugS_h

// Allows serial output for a single file
// Just change the import to DebugS.h

#define DBG(...) Serial.println(__VA_ARGS__)
#define DBG_PRNT(...) Serial.print(__VA_ARGS__)
#define DBG_WRT(...) debugMode == DEBUG_ON ? Serial.write(__VA_ARGS__)

#define DBGP(...) _stream->println(__VA_ARGS__)
#define DBGP_PRNT(...) _stream->print(__VA_ARGS__)
#define DBGP_WRT(...) _stream->write(__VA_ARGS__)

#endif