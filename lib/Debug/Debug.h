#ifndef Debug_h
#define Debug_h

// #define DEBUG_ON

#ifdef DEBUG_ON
#define debugPrintln(...) _stream->println(__VA_ARGS__)
#define debugPrint(...) _stream->print(__VA_ARGS__)
#else
#define debugPrintln(...)
#define debugPrint(...)
#endif

#ifdef DEBUG_PRINT_ON
#define debugPrintf(...) printf(__VA_ARGS__)
#else
#define debugPrintf(...)
#endif

#endif