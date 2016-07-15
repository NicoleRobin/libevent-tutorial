CC = g++
Target = block_http_client

all: block_http_client

block_http_client:block_http_client.c
	g++ -g -o $@ $<

.PHONY:clean

clean:
	rm -rf $Target
