#include <event2/event.h>
#include <stdio.h>

void cb_func(evutil_socket_t fd, short what, void *arg)
{
	printf("cb_func");
}

int main(void)
{
	struct event *ev1;
	struct event_base *base;

	base = event_base_new();

	ev1 = event_new(base, 0, EV_READ | EV_PERSIST, cb_func, (char*)"Read function");

	event_add(ev1, NULL);
	int iRet = event_base_dispatch(base);
	printf("Ret=[%d]\n", iRet);

	return 0;
}
