/*
 * gcc -o event_base_get_method ./event_base_get_method.c -levent_core
 */
#include <stdio.h>

#include <event2/event.h>

int main(int argc, char **argv)
{
	struct event_base *base = event_base_new();
	
	const char **pSupportMethods = event_get_supported_methods();
	printf("libevent supported methods:\n");
	while (*pSupportMethods != NULL)
	{
		printf("%s\n", *pSupportMethods);
		pSupportMethods++;
	}

	printf("*******************************************\n");
	const char *pMethod = event_base_get_method(base);
	printf("libevent used method is %s\n", pMethod);

	return 0;
}
