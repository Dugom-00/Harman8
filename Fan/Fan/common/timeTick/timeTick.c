#include "timeTick.h"

uint32_t timeTick = 0;

void incTick()
{
	timeTick++;
}

uint32_t getTick()
{
	return timeTick;
}

void clearTick()
{
	timeTick = 0;
}