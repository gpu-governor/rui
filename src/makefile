CC = gcc
FILE = main.c
EXE = main
RAY = -lraylib -lGL -lpthread -ldl -lrt 
STD = -lm -w #ignores warnings


all: $(EXE)

$(EXE): $(FILE)
		$(CC) $(FILE) -o $(EXE) $(RAY) $(STD)

clean:
	rm -f $(EXE)
