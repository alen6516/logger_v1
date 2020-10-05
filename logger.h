#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "util.h"

#define MODULE_SIZE 20    // "[main] "
#define MODE_SIZE 9       // "[WARN] "
#define MSG_SIZE 100      // msg body 
#define TOTAL_SIZE (MODULE_SIZE+MODE_SIZE+MSG_SIZE)


typedef enum {
    INFO_MODE = 0,
    DEBUG_MODE,
    CHECK_MODE,
    WARN_MODE,
    ERROR_MODE
} mode_e;

struct log_node_t {
    mode_e mode;
    char module[MODULE_SIZE];
    time_t time;
    char msg[MSG_SIZE];
    struct log_node_t* next;
};
#define LOGNODE_CALLOC() (struct log_node_t*) calloc(1, sizeof(struct log_node_t))

struct logger_t {
    mode_e mode;
    FILE* log_file;
    struct log_node_t root;
    unsigned char log_thread: 1,
                  spare:      7;
};
#define LOGGER_CALLOC() (struct logger_t*) calloc(1, sizeof(struct logger_t))
extern struct logger_t logger;


int init_logger(struct logger_t*, const char*, unsigned char);

#define _log(line_num, mode, format, ...) do {                     \
    char cwd[MODULE_SIZE];                                              \
    if (getcwd(cwd, MODULE_SIZE) != NULL) {                             \
        if (logger.log_thread) {                                        \
            struct log_node_t* log_node = LOGNODE_CALLOC();             \
        } else {                                                        \
            /*_mylog(cwd, line_num, CHECK_MODE, format, __VA_ARGS__);*/ \
            char buf[MODULE_SIZE + MODE_SIZE + MSG_SIZE];               \
            int len = 0;                                                \
            switch (mode) {                                             \
                case CHECK_MODE:                                        \
                    len = snprintf(buf, MODULE_SIZE+MODE_SIZE, "[%5s][%5s] ", cwd, "CHECK");    \
                    break;                                              \
                default:                                                \
                    break;                                              \
            }                                                           \
            len = snprintf(buf + MODULE_SIZE + MODE_SIZE - 1, MSG_SIZE-1, format, __VA_ARGS__);        \
            printf("%s", buf);                                          \
            }                                                           \
    } else {                                                            \
        /* can not get filename */                                      \
        printf("GG\n");                                                 \
    }                                                                   \
} while(0)


static inline void _log_get_module_name(char **filename)
{
    strtok(*filename, ".");
    strtok(*filename, "_");
}

static inline void log(unsigned line_num, mode_e mode, const char* format, ...)
{
    va_list arg;
    va_start(arg, format);
    va_end(arg);

    char cwd[MODULE_SIZE];
    if (getcwd(cwd, MODULE_SIZE) == NULL) {
        strncpy(cwd, "Unknown", MODULE_SIZE);
    } else {
        _log_get_module_name((char**) &cwd);
    }

    if (logger.log_thread) {
        struct log_node_t* log_node = LOGNODE_CALLOC();
    } else {
        char buf[TOTAL_SIZE];
        int len = 0;
        switch (mode) {
            case CHECK_MODE:
                len = snprintf(buf, MODULE_SIZE+MODE_SIZE, "[%5s][%5s] ", cwd, "CHECK");
                break;
            case INFO_MODE:
                len = snprintf(buf, MODULE_SIZE+MODE_SIZE, "[%5s][%5s] ", cwd, "INFO");
                break;
            case DEBUG_MODE:
                len = snprintf(buf, MODULE_SIZE+MODE_SIZE, "[%5s][%5s] ", cwd, "DEBUG");
                break;
            case WARN_MODE:
                len = snprintf(buf, MODULE_SIZE+MODE_SIZE, "[%5s][%5s] ", cwd, "WARN");
                break;
            case ERROR_MODE:
                len = snprintf(buf, MODULE_SIZE+MODE_SIZE, "[%5s][%5s] ", cwd, "ERROR");
                break;
            default:
                break;
        }
        len = vsnprintf(buf + MODULE_SIZE + MODE_SIZE - 1, MSG_SIZE-1, format, arg);
        if (NULL != logger.log_file) {
            fwrite(buf, TOTAL_SIZE, 1, logger.log_file);
        }
        printf("%s", buf);
    }
}


#define INFO(format, ...)  mylog(__LINE__, INFO_MODE, format, __VA_ARGS__)
#define DEBUG(format, ...) mylog(__LINE__, DEBUG_MODE, format, __VA_ARGS__)
#define CHECK(format, ...) mylog(__LINE__, CHECK_MODE, format, __VA_ARGS__)
#define WARN(format, ...)  mylog(__LINE__, WARN_MODE, format, __VA_ARGS__)
#define ERROR(format, ...) mylog(__LINE__, ERROR_MODE, format, __VA_ARGS__)

/*
int main () {
    g_mode = info_mode;
    INFO("123 %c %d %p\n", 'v', 87, NULL);
    
    const char *s = "12321";
    printf("%s\n", s);
}
*/
#endif
