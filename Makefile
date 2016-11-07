CC = clang
OUTPUT = ./dtetris
all: clean build

build:
	$(CC) \
	  -Wall \
	  -Wextra \
	  -pedantic \
	  -std=c11 \
	  -pthread \
	  functions.c \
	  getch.c \
	  thread_input.c \
	  thread_rendering.c \
	  dtetris.c \
	  -o $(OUTPUT)

install:
	cp -i dtetris /usr/local/bin

clean:
ifneq ("$(wildcard ./dtetris)", "")
	rm ./dtetris
endif

#watch:
#	while true; do \
#		make \
#		inotifywait -qre close_write .; \
#	done
