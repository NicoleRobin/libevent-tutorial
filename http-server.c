#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>

#include <netinet/in.h>
#include <arpa/inet.h>

char uri_root[512];

static const struct table_entry {
	const char *extension;
	const char *content_type;
} content_type_table[] = {
	{ "txt", "text/plain" }, 
	{ "c", "text/plain" }, 
	{ "h", "text/plain" }, 
	{ "html", "text/html" }, 

};

int main(int argc, char **argv)
{
	struct event_base *base;
	struct evhttp *http;
	struct evhttp_bound_socket *handl;
	int port = 0;

	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		printf("signal error, errno[%d], error[%s]", errno, strerror(errno));
		return -1;
	}

	base = event_base_new();


	return 0;
}