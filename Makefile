CC = g++
Target = test-echo time-test hello-world http-server

all: $(Target) 

test-echo:test-echo.c
	gcc -g -o test-echo test-echo.c -levent_core

time-test:time-test.c
	gcc -g -o time-test time-test.c -levent_core

hello-world:hello-world.c
	gcc -g -o hello-world hello-world.c -levent_core

http-server:http-server.c
	gcc -g -o http-server http-server.c -levent

.PHONY:clean

clean:
	rm -rf *.o
	rm -rf $(Target)
