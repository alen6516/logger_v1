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
// [main][WARN] This is an example.


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

void mylog(unsigned, const char *, mode_e, const char*, ...);

int init_logger(struct logger_t*, const char*, unsigned char);


#define INFO(format, ...)  mylog(__LINE__, __FILE__, INFO_MODE, format, __VA_ARGS__)
#define DEBUG(format, ...) mylog(__LINE__, __FILE__, DEBUG_MODE, format, __VA_ARGS__)
#define CHECK(format, ...) mylog(__LINE__, __FILE__, CHECK_MODE, format, __VA_ARGS__)
#define WARN(format, ...)  mylog(__LINE__, __FILE__, WARN_MODE, format, __VA_ARGS__)
#define ERROR(format, ...) mylog(__LINE__, __FILE__, ERROR_MODE, format, __VA_ARGS__)


/*
int main () {
    g_mode = info_mode;
    INFO("123 %c %d %p\n", 'v', 87, NULL);
    
    const char *s = "12321";
    printf("%s\n", s);
}
*/
#endif
