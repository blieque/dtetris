CC = clang
OUTPUT = ./dtetris

build: $(OUTPUT)

$(OUTPUT):
	@echo "Building..."
	$(CC) \
	  -Wall \
	  -Wextra \
	  -pedantic \
	  -std=c11 \
	  -pthread \
	  src/functions.c \
	  src/getch.c \
	  src/thread_input.c \
	  src/thread_rendering.c \
	  src/dtetris.c \
	  -o $(OUTPUT)

install:
	@if [ -e $(OUTPUT) ]; then \
	  cp -i dtetris /usr/local/bin; \
	  echo "Copying executable to /usr/local/bin."; \
	else \
	  echo "Run \`make build\` first."; \
	fi

clean:
	@[ -e $(OUTPUT) ] && rm $(OUTPUT)

#.watch:
#	while true; do \
#		make \
#		inotifywait -qre close_write .; \
#	done
