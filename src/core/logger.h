#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <time.h>
#include <string.h>

#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define RESET "\033[0m"

#define loggerLog(format, ...) \
 	do { \
        char prefix[80]; \
        sprintf(prefix, GREEN "[%s:%d]", \
               __FILE__, __LINE__); \
        int prefix_length = strlen(prefix); \
        printf("%s", prefix); \
        printf("%*s", (int)(40 - prefix_length - strlen(RESET)), ""); \
        printf(format, ##__VA_ARGS__); \
        printf(RESET "\n"); \
    } while (0)

#define loggerErr(format, ...) \
 	do { \
        char prefix[80]; \
        sprintf(prefix, RED "[%s:%d]", \
               __FILE__, __LINE__); \
        int prefix_length = strlen(prefix); \
        printf("%s", prefix); \
        printf("%*s", (int)(40 - prefix_length - strlen(RESET)), ""); \
        printf(format, ##__VA_ARGS__); \
        printf(RESET "\n"); \
    } while (0)

#define loggerWarn(format, ...) \
 	do { \
        char prefix[80]; \
        sprintf(prefix, YELLOW "[%s:%d]", \
               __FILE__, __LINE__); \
        int prefix_length = strlen(prefix); \
        printf("%s", prefix); \
        printf("%*s", (int)(40 - prefix_length - strlen(RESET)), ""); \
        printf(format, ##__VA_ARGS__); \
        printf(RESET "\n"); \
    } while (0)
#endif