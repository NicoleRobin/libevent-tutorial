#include <event2/event.h>
#include <stdio.h>

int main(void)
{
	struct event_base *base;
	base = event_base_new();

	int iRet = event_base_loop(base, EVLOOP_NO_EXIT_ON_EMPTY);
	printf("Ret=[%d]\n", iRet);

	return 0;
}
