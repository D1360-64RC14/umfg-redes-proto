CC_FLAGS = -Wall -Wextra -O2 -static
CC = gcc $(CC_FLAGS)

LIB_SOCKCLIENT_A = lib/libsockclient/libsockclient.a

sockclient: bin/sockclient

run: sockclient
	./bin/sockclient

valgrind: sockclient
	valgrind --leak-check=full --track-origins=yes -s ./bin/sockclient

test:
	make -C lib/libsockclient test

clean:
	rm -rf bin
	make -C lib/libsockclient clean

# -----------------------------------------------------------------

bin/sockclient: src/main.c $(LIB_SOCKCLIENT_A)
	mkdir -p bin
	$(CC) -o $@ -Ilib $^
	@echo "Successfully built into $@!"

$(LIB_SOCKCLIENT_A):
	make -C lib/libsockclient libsockclient.a