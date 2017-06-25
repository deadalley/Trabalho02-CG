CC = g++
CFLAGS = -c -g
IFLAGS = -Iinclude
TFLAGS = -lGL -lGLU -lglut -std=c++11
SRCDIR = src
OBJDIR = obj
SRC = Point3D.cpp Object.cpp
OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))

all: main

main: $(OBJ) obj/main.o
	$(CC) -g $(IFLAGS) $^ -o $@ $(TFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(IFLAGS) $< -o $@

obj/main.o: main.cpp
	$(CC) $(CFLAGS) $(IFLAGS) main.cpp -o obj/main.o $(TFLAGS)

clean:
	rm -rf obj/*.o main

val:
	valgrind --track-origins=yes ./main

run:
	main
	@./main