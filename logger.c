#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

int init_logger(struct logger_t* logger, const char *log_file, unsigned char log_thread) {
    if (NULL == logger) {
        return -1;
    }
    logger->mode = CHECK_MODE;
    logger->log_file = fopen(log_file, "w+");
    logger->log_thread = log_thread;
    return 0;
}

