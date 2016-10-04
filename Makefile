all: clean build

build:
	clang -pthread dtetris.c -o dtetris

install:
	cp -i dtetris /usr/local/bin

clean:
	[ -f dtetris ] && rm dtetris

#watch:
#	while true; do \
#		make \
#		inotifywait -qre close_write .; \
#	done
