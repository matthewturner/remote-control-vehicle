#ifndef Debug_h
#define Debug_h

#define DEBUG_ON 1
#define DEBUG_OFF 0

// set this
#define DEBUG_MODE DEBUG_OFF

#if DEBUG_MODE == DEBUG_ON
#define debugPrintln(...) _stream->println(__VA_ARGS__)
#define debugPrint(...) _stream->print(__VA_ARGS__)
#else
#define debugPrintln(...)
#define debugPrint(...)
#endif

#endif