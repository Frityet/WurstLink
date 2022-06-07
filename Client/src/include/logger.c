#include "logger.h"

#include <stdarg.h>

#include "types.h"

FILE *Logger_output_file;

#undef constructor
#undef destructor

__attribute__((constructor(INT32_MIN))) 
static void Logger_initialise(void)
{
	Logger_output_file = fopen(LOGGER_OUTPUT_FILENAME, "w+");
	if (Logger_output_file != NULL)
		fseek(Logger_output_file, 0, SEEK_SET);
}

__attribute__((destructor(INT32_MAX))) 
static void Logger_deinitialise(void)
{
	fclose(Logger_output_file);
}

void Logger_log_base(FILE *out, const char *style, const char *file, const char *func, int line, const char *fmt, ...)
{
	enum { BUFFER_SIZE = 512 };
	char buf[BUFFER_SIZE], fbuf[BUFFER_SIZE];
	//We need the offset later, so save the result of writing to the buffer
	int offset = snprintf(buf, BUFFER_SIZE, "%s[%s:%d - %s]: ", style, file, line, func),
		foffset = snprintf(fbuf, BUFFER_SIZE, "[%s:%d - %s]: ", file, line, func);

	//Because we are writing identical info to each buffer, just copy the args
	va_list l, l2;
	va_start(l, fmt);
	va_copy(l2, l);

	//Normal vsnprintf will overwrite the buffer, so we must offset it
	vsnprintf(buf + offset, BUFFER_SIZE - offset, fmt, l);
	vsnprintf(fbuf + foffset, BUFFER_SIZE - foffset, fmt, l2);

	va_end(l);

	fputs(buf, out);
	//Clear all colours, etc
	fputs("\x1b[0m\n", out);
	fputs(fbuf, Logger_output_file);
	fputc('\n', Logger_output_file);
}
