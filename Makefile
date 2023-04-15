#Pirleci Noela-Elena, 315CA

CC=gcc
CFLAGS=-Wall-Wextra -std=c99

build:
	gcc image_editor.c -o image_editor
clean:
	rm -f image_editor
