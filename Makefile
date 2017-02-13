#
# Makefile
# Azinum 2017.02.13
#


CC = gcc

FLAGS = -Os -std=c99 -Wall

NAME = Tyos

FILES = *.c

all: default


default:
	$(CC) -o $(NAME) $(FILES) $(FLAGS)
	clear; ./$(NAME)
