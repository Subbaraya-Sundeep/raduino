#include <logger.h>

#ifdef EARLY_DEBUG
unsigned char log_buffer[1024] = {0};
unsigned int head = 0;
unsigned int tail = 0;

void logger_putc(void *arg, char data)
{
	*(log_buffer + head) = data;
	head++;
}

void logger_init(void)
{
	init_printf(NULL, logger_putc);
}


char logger_read(void)
{
	char data = *(log_buffer + tail);

	tail++;
	return data;
}

unsigned int logger_available(void)
{
	return head - tail;
}

#endif
