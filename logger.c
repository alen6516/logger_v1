#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

/**
 * init logger
 */
int init_logger(struct logger_t* logger, const char *log_file, unsigned char log_thread) {
    if (NULL == logger) {
        return -1;
    }
    logger->mode = CHECK_MODE;
    logger->log_file = fopen(log_file, "w+");
    logger->log_thread = log_thread;
    return 0;
}


/**
 * the macro version of function log
 * use this macro if we don't want to call log()
 */
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


/**
 * get module name from current filename.
 * e.g. sflow_sender.c => sflow
 */
static void _log_get_module_name(char *filename)
{
    strtok(filename, ".");
    strtok(filename, "_");
}

/**
 * makeup the entire log
 * write to file and print to stdout
 */
void mylog(unsigned line_num, const char *filename, mode_e mode, const char* format, ...)
{
    va_list arg;
    va_start(arg, format);
    va_end(arg);

    
    char module[MODULE_SIZE] = {0};
    strncpy(module, filename, MODULE_SIZE-1);
    _log_get_module_name(module);

    if (logger.log_thread) {
        struct log_node_t* log_node = LOGNODE_CALLOC();
    } else {
        char buf[TOTAL_SIZE];
        int len = 0;
        switch (mode) {
            case CHECK_MODE:
                len = snprintf(buf, MODULE_SIZE+MODE_SIZE, "[%5s][%5s] ", module, "CHECK");
                break;
            case INFO_MODE:
                len = snprintf(buf, MODULE_SIZE+MODE_SIZE, "[%5s][%5s] ", module, "INFO");
                break;
            case DEBUG_MODE:
                len = snprintf(buf, MODULE_SIZE+MODE_SIZE, "[%5s][%5s] ", module, "DEBUG");
                break;
            case WARN_MODE:
                len = snprintf(buf, MODULE_SIZE+MODE_SIZE, "[%5s][%5s] ", module, "WARN");
                break;
            case ERROR_MODE:
                len = snprintf(buf, MODULE_SIZE+MODE_SIZE, "[%5s][%5s] ", module, "ERROR");
                break;
            default:
                break;
        }
        len = vsnprintf(buf + len, MSG_SIZE-1, format, arg);
        if (NULL != logger.log_file) {
            fwrite(buf, TOTAL_SIZE, 1, logger.log_file);
        }
        printf("%s", buf);
    }
}

