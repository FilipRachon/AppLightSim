CC=g++
OPT=-O3
LINKER_FLAGS=-lSDL2main -lSDL2
CFLAGS=-Wall -Wextra $(LINKER_FLAGS) $(OPT)

CFILES=src/main.cpp src/app.cpp src/canvas.cpp src/shapes.cpp src/gui.cpp src/renderer.cpp src/window.cpp
OBJS=obj/main.o obj/app.o obj/canvas.o obj/shapes.o obj/gui.o obj/renderer.o obj/window.o

EXE=app

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

obj/main.o: src/main.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

obj/app.o: src/app.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

obj/canvas.o: src/canvas.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

obj/shapes.o: src/shapes.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

obj/gui.o: src/gui.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

obj/renderer.o: src/renderer.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

obj/window.o: src/window.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

clean:
	rm -f $(EXE)
	rm -f obj/*.o