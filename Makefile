all: clean build

build:
	clang -pthread dtetris.c thread_input.c thread_rendering.c -o dtetris -Wall

install:
	cp -i dtetris /usr/local/bin

clean:
	[[ -f dtetris ]] && rm dtetris

#watch:
#	while true; do \
#		make \
#		inotifywait -qre close_write .; \
#	done
