#pragma once

#include <stdio.h>
#include "common.h"

#ifndef LOGGER_OUTPUT_FILENAME
#	define LOGGER_OUTPUT_FILENAME "wurstlink.log"
#endif

#define LOG_DEBUG(msg, ...) 	({\
									extern void Logger_log_base(FILE *out, const char *style, const char *file, const char *func, int line, const char *fmt, ...);\
									Logger_log_base(stdout, "\x1b[2m", __FILE__, __PRETTY_FUNCTION__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__);\
								})

#define LOG_INFO(msg, ...)  	({\
									extern void Logger_log_base(FILE *out, const char *style, const char *file, const char *func, int line, const char *fmt, ...);\
									Logger_log_base(stdout, "\x1b[0m", __FILE__, __PRETTY_FUNCTION__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__);\
								})

#define LOG_WARNING(msg, ...) 	({\
									extern void Logger_log_base(FILE *out, const char *style, const char *file, const char *func, int line, const char *fmt, ...);\
									Logger_log_base(stdout, "\x1b[33m", __FILE__, __PRETTY_FUNCTION__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__);\
								})

#define LOG_ERROR(msg, ...) 	({\
									extern void Logger_log_base(FILE *out, const char *style, const char *file, const char *func, int line, const char *fmt, ...);\
									Logger_log_base(stdout, "\x1b[31m", __FILE__, __PRETTY_FUNCTION__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__);\
								})

#define LOG_FATAL(msg, ...) 	({\
									extern void Logger_log_base(FILE *out, const char *style, const char *file, const char *func, int line, const char *fmt, ...);\
									Logger_log_base(stdout, "\x1b[1;5;31m", __FILE__, __PRETTY_FUNCTION__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__);\
								})
