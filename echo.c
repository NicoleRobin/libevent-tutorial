#include <stdio.h>

#include "event2/event.h"

void accept_cb(int fd, short events, void *arg)
{
	struct event_base *ev_base = (struct event_base*)arg;

	struct sockaddr_in cli_addr;
	memset(&cli_addr, 0, sizeof(cli_addr));
	socklen_t cli_addr_len = sizeof(cli_addr);
	int client = accept(fd, (struct sockaddr*)&cli_addr, &cli_addr_len);

	printf("accept new connection [%s:%d]\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
	
	struct event *ev = event_new(ev_base, client, EV_READ, read_cb, ev_base);
	event_add(ev_base);
}

void read_cb(int fd, short events, void *arg)
{
	struct event_base *ev_base = (struct event_base*)arg;

	char buf[1024];
	int iLen = recv(fd, buf, 1024, 0);
	if (iLen == 0)
	{
		printf("peer shutdown, close it\n");
		// 怎么删除event呢？
		close(fd);
	}
	else if (iLen == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{

		}
	}
	
	printf("recv %d bytes data, content[%s]\n", iLen, buf);
	// 这里应该删除EV_READ，添加EV_WRITE
	//
}

void write_cb(int fd, short events, void *arg)
{
	struct event_base ev_base = (struct event_base)arg;

	int iLen = send(fd, buf, strlen(buf), 0);

	printf("send %d bytes data, contents[%s]\n", );
	// 这里应该删除EV_WRITE，添加EV_READ
}

int main(int argc, char **argv)
{
	int server = socket(AF_INET, SOCK_STREAM, 0);

	struct sockadd_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADD_ANY;
	addr.sin_port = htons(8080);

	bind(server, (struct sockaddr *)&addr, sizeof(addr));
	listen(server, 5);

	struct event_base *ev_base = event_base_new();
	
	struct event *ev = event_new(ev_base, server, EV_READ, accept_cb, ev_base);
	event_add(ev, NULL);

	event_base_dispatch();

	event_free(ev);
	event_base_free(ev_base);

	return 0;
}
