#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "textColors.h"

int infof(const char *format, ...) {
    va_list text;
    va_start(text, format);
    textcolor(BRIGHT, GREEN, BLACK);
    int status_code = vfprintf(stdout, format, text);
    textcolor(RESET, WHITE, BLACK);
    va_end(text);
    return status_code;
}

int warnf(const char *format, ...) {
    va_list text;
    va_start(text, format);
    textcolor(BRIGHT, YELLOW, BLACK);
    int status_code = vfprintf(stdout, format, text);
    textcolor(RESET, WHITE, BLACK);
    va_end(text);
    return status_code;
}

int errorf(const char *format, ...) {
    va_list text;
    va_start(text, format);
    textcolor(BRIGHT, CYAN, BLACK);
    int status_code = vfprintf(stdout, format, text);
    textcolor(RESET, WHITE, BLACK);
    va_end(text);
    return status_code;
}

int panicf(const char *format, ...) {
    va_list text;
    va_start(text, format);
    textcolor(BRIGHT, RED, BLACK);
    int ok = vfprintf(stdout, format, text);
    textcolor(RESET, WHITE, BLACK);
    va_end(text);
    return ok;
}
