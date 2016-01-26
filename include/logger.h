#ifndef _LOGGER_H
#define _LOGGER_H

#define EARLY_DEBUG	1

#include "printf.h"
#include "types.h"

void logger_init(void);
void logger_putc(void *, char);
char logger_read(void);
unsigned int logger_available(void);

#endif
