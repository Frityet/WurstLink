//
// Created by Frityet on 2022-03-17.
//

#pragma once

#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#define LOG_BUFSIZE 1024

//#define LOG_COLOUR_CODE_INFO    "\x1b[39m"
//#define LOG_COLOUR_CODE_SUCCESS "\x1b[32m"
//#define LOG_COLOUR_CODE_DEBUG   "\x1b[90m"
//#define LOG_COLOUR_CODE_WARNING "\x1b[33m"
//#define LOG_COLOUR_CODE_ERROR   "\x1b[31m"
//#define LOG_COLOUR_CODE_FATAL   "\x1b[41m"

//Emscripten JS throws a fit over \x1b
#define LOG_COLOUR_CODE_INFO    "{Info} - "
#define LOG_COLOUR_CODE_SUCCESS "{Success} - "
#define LOG_COLOUR_CODE_DEBUG   "{Debug} - "
#define LOG_COLOUR_CODE_WARNING "{Warning} - "
#define LOG_COLOUR_CODE_ERROR   "{Error} - "
#define LOG_COLOUR_CODE_FATAL   "{Fatal} - "

#define LOG_INFO(...)       ({ \
                                char _LOG_buf[LOG_BUFSIZE]; \
                                snprintf(_LOG_buf, LOG_BUFSIZE, __VA_ARGS__);     \
                                logbase(LOG_TYPE_INFO, _LOG_buf, __LINE__, __FILE__, __PRETTY_FUNCTION__);  \
                            })


#define LOG_SUCCESS(...)    ({ \
                                char _LOG_buf[LOG_BUFSIZE]; \
                                snprintf(_LOG_buf, LOG_BUFSIZE, __VA_ARGS__);     \
                                logbase(LOG_TYPE_SUCCESS, _LOG_buf, __LINE__, __FILE__, __PRETTY_FUNCTION__);\
                            })


#define LOG_DEBUG(...)      ({ \
                                char _LOG_buf[LOG_BUFSIZE]; \
                                snprintf(_LOG_buf, LOG_BUFSIZE, __VA_ARGS__);     \
                                logbase(LOG_TYPE_DEBUG, _LOG_buf, __LINE__, __FILE__, __PRETTY_FUNCTION__);\
                            })


#define LOG_WARNING(...)    ({ \
                                char _LOG_buf[LOG_BUFSIZE]; \
                                snprintf(_LOG_buf, LOG_BUFSIZE, __VA_ARGS__);     \
                                logbase(LOG_TYPE_WARNING, _LOG_buf, __LINE__, __FILE__, __PRETTY_FUNCTION__);\
                            })


#define LOG_ERROR(...)      ({ \
                                char _LOG_buf[LOG_BUFSIZE]; \
                                snprintf(_LOG_buf, LOG_BUFSIZE, __VA_ARGS__);     \
                                logbase(LOG_TYPE_ERROR, _LOG_buf, __LINE__, __FILE__, __PRETTY_FUNCTION__);\
                            })


#define LOG_FATAL(...)      ({ \
                                char _LOG_buf[LOG_BUFSIZE]; \
                                snprintf(_LOG_buf, LOG_BUFSIZE, __VA_ARGS__);     \
                                logbase(LOG_TYPE_FATAL, _LOG_buf, __LINE__, __FILE__, __PRETTY_FUNCTION__);\
                            })


enum LogType {
    LOG_TYPE_INFO,
    LOG_TYPE_SUCCESS,
    LOG_TYPE_DEBUG,
    LOG_TYPE_WARNING,
    LOG_TYPE_ERROR,
    LOG_TYPE_FATAL
};

__attribute__((used))
static void logbase(enum LogType type, const char *msg, int line, const char *file, const char *func)
{
    switch (type) {
        default:
        case LOG_TYPE_INFO: {
            fprintf(stdout, LOG_COLOUR_CODE_INFO    "[%s:%d - %s] %s\n", file, line, func, msg);
            break;
        }
        case LOG_TYPE_SUCCESS: {
            fprintf(stdout, LOG_COLOUR_CODE_SUCCESS "[%s:%d - %s] %s\n", file, line, func, msg);
            break;
        }
        case LOG_TYPE_DEBUG: {
            fprintf(stdout, LOG_COLOUR_CODE_DEBUG   "[%s:%d - %s] %s\n", file, line, func, msg);
            break;
        }
        case LOG_TYPE_WARNING: {
            fprintf(stderr, LOG_COLOUR_CODE_WARNING "[%s:%d - %s] %s\n", file, line, func, msg);
            break;
        }
        case LOG_TYPE_ERROR: {
            fprintf(stderr, LOG_COLOUR_CODE_ERROR "[%s:%d - %s] %s\n", file, line, func, msg);
            break;
        }
        case LOG_TYPE_FATAL: {
            fprintf(stderr, LOG_COLOUR_CODE_FATAL "[%s:%d - %s] %s\n", file, line, func, msg);
            break;
        }
    }
}


