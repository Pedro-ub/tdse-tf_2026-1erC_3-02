#include "queue.h"
#include "config.h"
#include "main.h"


static Event_t buffer[QUEUE_SIZE];
static volatile uint8_t head  = 0;   /* proximo lugar donde escribir (push) */
static volatile uint8_t tail  = 0;   /* proximo lugar de donde leer (pop)   */
static volatile uint8_t count = 0;   /* cantidad de elementos actualmente en la cola */

void queueInit(void)
{
	head  = 0;
	tail  = 0;
	count = 0;
}

bool queueIsEmpty(void)
{
	return (count == 0);
}

bool queueIsFull(void)
{
	return (count == QUEUE_SIZE);
}

bool queuePush(Event_t event)
{
	bool ok = false;


	__disable_irq();

	if(!queueIsFull()){
		buffer[head] = event;
		head = (uint8_t)((head + 1) % QUEUE_SIZE);
		count++;
		ok = true;
	}
	/* Si esta llena, se descarta el evento nuevo (mejor perder un evento
	 * que corromper la cola o bloquear la ISR). */

	__enable_irq();

	return ok;
}

bool queuePop(Event_t *event)
{
	bool ok = false;

	if(event == NULL) return false;

	__disable_irq();

	if(!queueIsEmpty()){
		*event = buffer[tail];
		tail = (uint8_t)((tail + 1) % QUEUE_SIZE);
		count--;
		ok = true;
	}

	__enable_irq();

	return ok;
}
