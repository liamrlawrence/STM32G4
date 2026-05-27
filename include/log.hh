#pragma once

#include "SEGGER_RTT.h"  // IWYU pragma: keep


/* ------------------------------------------------------------------ */
/* Log levels. Lower number = higher severity.                        */
/* Set LOG_LEVEL to one of these to control how much gets compiled in.*/
/* ------------------------------------------------------------------ */
#define LOG_LEVEL_NONE 0  /* logging disabled entirely         */
#define LOG_LEVEL_ERROR 1 /* unrecoverable / serious problems  */
#define LOG_LEVEL_WARN 2  /* something unexpected, but running */
#define LOG_LEVEL_INFO 3  /* normal operational messages       */
#define LOG_LEVEL_DEBUG 4 /* detailed diagnostic output        */
#define LOG_LEVEL_TRACE 5 /* very verbose, per-step tracing    */

/* ------------------------------------------------------------------ */
/* Active level. Override at build time with e.g. -DLOG_LEVEL=4, or   */
/* #define it before including this header. Defaults to INFO.         */
/* ------------------------------------------------------------------ */
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_INFO
#endif

/* ------------------------------------------------------------------ */
/* Output backend. Everything funnels through LOG_OUTPUT, so logging  */
/* can be repointed at printf, a UART, etc. by overriding this one    */
/* macro before including. Defaults to SEGGER RTT up-channel 0.       */
/* ------------------------------------------------------------------ */
#ifndef LOG_OUTPUT
#define LOG_OUTPUT(...) SEGGER_RTT_printf(0, __VA_ARGS__)
#endif

/* ------------------------------------------------------------------ */
/* Colors for different log levels using ANSI escape codes            */
/* ------------------------------------------------------------------ */
#ifndef LOG_USE_COLOR
#define LOG_USE_COLOR 1
#endif

#if LOG_USE_COLOR
#define LOG_COL_ERROR "\x1B[1;31m" /* bright red      */
#define LOG_COL_WARN "\x1B[1;33m"  /* bright yellow   */
#define LOG_COL_INFO "\x1B[1;32m"  /* bright green    */
#define LOG_COL_DEBUG "\x1B[1;36m" /* bright cyan     */
#define LOG_COL_TRACE "\x1B[2;37m" /* dim white       */
#define LOG_COL_RESET "\x1B[0m"    /* back to default */
#else
#define LOG_COL_ERROR ""
#define LOG_COL_WARN ""
#define LOG_COL_INFO ""
#define LOG_COL_DEBUG ""
#define LOG_COL_TRACE ""
#define LOG_COL_RESET ""
#endif


/* ------------------------------------------------------------------ */
/* Level macros. Each expands to a real call only if the active level */
/* is high enough; otherwise it expands to a no-op that evaluates     */
/* none of its arguments and generates zero code.                     */
/*                                                                    */
/* NOTE: the color codes, tag and trailing "\n" are concatenated onto */
/* fmt as adjacent string literals, so fmt MUST be a string literal   */
/* (the normal case for logging), not a char* variable.               */
/* ------------------------------------------------------------------ */
#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(fmt, ...) LOG_OUTPUT(LOG_COL_ERROR "[ERROR] " fmt LOG_COL_RESET "\n", ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...) ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
#define LOG_WARN(fmt, ...) LOG_OUTPUT(LOG_COL_WARN "[WARN]  " fmt LOG_COL_RESET "\n", ##__VA_ARGS__)
#else
#define LOG_WARN(fmt, ...) ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(fmt, ...) LOG_OUTPUT(LOG_COL_INFO "[INFO]  " fmt LOG_COL_RESET "\n", ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...) ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(fmt, ...) LOG_OUTPUT(LOG_COL_DEBUG "[DEBUG] " fmt LOG_COL_RESET "\n", ##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...) ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_TRACE
#define LOG_TRACE(fmt, ...) LOG_OUTPUT(LOG_COL_TRACE "[TRACE] " fmt LOG_COL_RESET "\n", ##__VA_ARGS__)
#else
#define LOG_TRACE(fmt, ...) ((void)0)
#endif

