CC_FLAGS = -Wall -Wextra -O2
CC = gcc $(CC_FLAGS)

libsockclient.a: client.o proto.o
	ar cr $@ $^

test: test/proto_test
	./test/proto_test

clean:
	rm -f libsockclient.a client.o proto.o
	rm -f test/proto_test

# -----------------------------------------------------------------

client.o: client.c
	$(CC) -c -o $@ -I. $^

proto.o: proto.c
	$(CC) -c -o $@ -I. $^

test/proto_test: test/proto_test.c libsockclient.a
	$(CC) -o $@ -I. $^