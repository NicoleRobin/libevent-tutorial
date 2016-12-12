/*
 * gcc -g -o test-echo test-echo.c -levent_core
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include <event2/event.h>
#include <event2/bufferevent.h>

#define MAX_LINE 256
#define LISTEN_PORT 9999
#define LISTEN_BACKLOG 32

void do_accept(evutil_socket_t listener, short event, void *arg);
void read_cb(struct bufferevent *bev, void *arg);
void error_cb(struct bufferevent *bev, short event, void *arg);
void write_cb(struct bufferevent *bev, void *arg);

int main(int argc, char **argv)
{
	int ret;
	evutil_socket_t listener;
	listener = socket(AF_INET, SOCK_STREAM, 0);
	assert(listener > 0);
	evutil_make_listen_socket_reuseable(listener);

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(LISTEN_PORT);

	if (bind(listener, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("bind");
		return -1;
	}

	if (listen(listener, LISTEN_BACKLOG) < 0)
	{
		perror("listen");
		return -1;
	}

	printf("Listening on %d...\n", LISTEN_PORT);

	evutil_make_socket_nonblocking(listener);

	struct event_base *base = event_base_new();
	assert(base != NULL);
	struct event *listen_event;
	listen_event = event_new(base, listener, EV_READ | EV_PERSIST, do_accept, (void*)base);
	event_add(listen_event, NULL);
	event_base_dispatch(base);

	event_free(listen_event);
	event_base_free(base);

	printf("The end.");

	return 0;
}

void do_accept(evutil_socket_t listener, short event, void *arg)
{
	struct event_base *base = (struct event_base *)arg;
	evutil_socket_t fd;
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	socklen_t slen = sizeof(sin);
	fd = accept(listener, (struct sockaddr *)&sin, &slen);
	if (fd < 0)
	{
		fprintf(stderr, "accept, errno[%d], error:%s", errno, strerror(errno));
		return ;
	}
	
	if (fd > FD_SETSIZE)
	{
		perror("fd > FD_SETSIZE\n");
		return ;
	}

	printf("Accept: fd = %u, ip = %s, port = %d\n", fd, inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));

	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, read_cb, NULL, error_cb, arg);
	bufferevent_enable(bev, EV_READ | EV_WRITE |EV_PERSIST);
}

void read_cb(struct bufferevent *bev, void *arg)
{
	char line[MAX_LINE + 1];
	int n;
	evutil_socket_t fd = bufferevent_getfd(bev);

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	socklen_t slen = sizeof(sin);
	if (-1 == getpeername(fd, (struct sockaddr *)&sin, &slen))
	{
		fprintf(stderr, "getpeername, errno[%d], error:%s", errno, strerror(errno));
		return;
	}

	while(n = bufferevent_read(bev, line, MAX_LINE), n > 0)
	{
		line[n] = '\0';
		printf("Read: fd = %u, ip = %s, port = %d, line:%s", fd, inet_ntoa(sin.sin_addr), ntohs(sin.sin_port), line);

		bufferevent_write(bev, line, n);
	}
}

void write_cb(struct bufferevent *bev, void *arg)
{}

void error_cb(struct bufferevent *bev, short event, void *arg)
{
	evutil_socket_t fd = bufferevent_getfd(bev);

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	socklen_t slen = sizeof(sin);
	if (-1 == getpeername(fd, (struct sockaddr *)&sin, &slen))
	{
		fprintf(stderr, "getpeername, errno[%d], error:%s", errno, strerror(errno));
		return;
	}

	printf("fd = %u, ip = %s, port = %d, ", fd, inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
	if (event & BEV_EVENT_TIMEOUT)
	{
		printf("Timed out\n");
	}
	else if (event & BEV_EVENT_EOF)
	{
		printf("connection closed\n");
	}
	else if (event & BEV_EVENT_ERROR)
	{
		printf("some other error\n");
	}

	bufferevent_free(bev);
}
