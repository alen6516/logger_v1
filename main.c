#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#include "util.h"
#include "logger.h"


#define LOG_FILE "./log.txt"

/*
#define LOG(format, ...) _LOG(__FILE__, __LINE__, format, ##__VA_ARGS__)

void _log(const char* file, unsigned int line, const char* format, va_list arg) 
{
    char buf[100];
    int len = 0;
    len = snprintf(buf, 20, "%s:%u ", file, line);
    vsnprintf(buf+len, 100-1, format, arg);
    printf("%s", buf);
}

void _LOG(const char *file, unsigned int line, const char* format, ...) 
{
    va_list arg;
    va_start(arg, format);
    va_end(arg);
    _log(file, line, format, arg);
}
*/

struct logger_t logger;


int main () 
{
    init_logger(&logger, LOG_FILE);
    //CHECK("hello world %d\n", 1);
    INFO("hello %d\n", 1);
}
