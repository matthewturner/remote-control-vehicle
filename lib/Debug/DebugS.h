#ifndef DebugS_h
#define DebugS_h

// Allows serial output for a single file
// Just change the import to DebugS.h

#define debugPrintln(...) _stream->println(__VA_ARGS__)
#define debugPrint(...) _stream->print(__VA_ARGS__)

#endif