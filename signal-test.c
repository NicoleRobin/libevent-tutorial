/*
 * g++ -g -o signal signal.c -levent
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <event2/event.h>

int called = 0;

static void signal_cb(evutil_socket_t fd, short event, void *arg)
{
	struct event *signal = (struct event*)arg;

	printf("%s:%d:%s: got signal %d\n", __FILE__, __LINE__, __func__, event_get_signal(signal));

	if (called >= 2)
	{
		event_del(signal);
	}

	called++;
}

int main(void)
{
	struct event *signal_int;
	struct event_base *base;

	/* Initialize the event library */
	base = event_base_new();

	/* Initialize one event */
	signal_int = evsignal_new(base, SIGINT, signal_cb, event_self_cbarg());

	event_add(signal_int, NULL);

	event_base_dispatch(base);
	event_base_free(base);

	return 0;
}
