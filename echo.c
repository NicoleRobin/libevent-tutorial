/*
 * g++ -g -o echo echo.c -levent
 */
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

//#include <event2/event.h>
#include <event.h>

void accept_cb(int fd, short events, void *arg);
void read_cb(int fd, short events, void *arg);
void write_cb(int fd, short events, void *arg);

void accept_cb(int fd, short events, void *arg)
{
	struct event *ev = (struct event*)arg;

	struct sockaddr_in cli_addr;
	memset(&cli_addr, 0, sizeof(cli_addr));
	socklen_t cli_addr_len = sizeof(cli_addr);
	int client = accept(fd, (struct sockaddr*)&cli_addr, &cli_addr_len);

	printf("accept new connection [%s:%d]\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
	
	struct event *new_ev = (struct event*)malloc(sizeof(struct event));
	event_assign(new_ev, ev->ev_base, client, EV_READ | EV_PERSIST, read_cb, new_ev);
	event_add(new_ev, NULL);
}

void read_cb(int fd, short events, void *arg)
{
	struct event *ev = (struct event*)arg;

	char buf[1024];
	int iLen = recv(fd, buf, 1024, 0);
	if (iLen == 0)
	{
		printf("peer shutdown, close it\n");
		event_del(ev);
		free(ev);
		close(fd);
	}
	else if (iLen == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			printf("No data to read\n");
		}

		printf("Unknow error occur, errno[%d]\n", errno);
		event_del(ev);
		free(ev);
		close(fd);
	}
	else
	{
		printf("recv %d bytes data, content[%s]\n", iLen, buf);
		event_del(ev);
		event_assign(ev, ev->ev_base, fd, EV_WRITE | EV_PERSIST, write_cb, ev);
		event_add(ev, NULL);
	}
}

void write_cb(int fd, short events, void *arg)
{
	struct event *ev = (struct event*)arg;
	
	char buf[] = "recv data succ!\n";
	int iLen = send(fd, buf, strlen(buf), 0);

	printf("send %d bytes data, contents[%s]\n", iLen, buf);
	event_del(ev);
	event_assign(ev, ev->ev_base, fd, EV_READ | EV_PERSIST, read_cb, ev);
	event_add(ev, NULL);
}

int main(int argc, char **argv)
{
	int server = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(8080);

	bind(server, (struct sockaddr *)&addr, sizeof(addr));
	listen(server, 5);

	struct event_base *ev_base = event_base_new();
	
	struct event listen_event;
	struct event *ev = (struct event*)malloc(sizeof(struct event));
	event_assign(ev, ev_base, server, EV_READ | EV_PERSIST, accept_cb, ev);
	event_add(ev, NULL);

	event_base_dispatch(ev_base);

	delete ev;
	event_base_free(ev_base);

	return 0;
}
