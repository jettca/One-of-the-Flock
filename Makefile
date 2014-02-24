CC = g++
CFLAGS = -g -Wall
TARGET = bin/main
OBJDIR = obj
SRCDIR = src
FRAMEWORKS = -framework OpenGL -framework GLUT
LIBRARIES = -I /usr/X11R6/include/ -L /usr/X11R6/lib64/ -lglut -lGL -lGLU -lX11 -lXmu -lXi -lm

# Link for target
$(TARGET): $(OBJDIR)/main.o $(OBJDIR)/quaternion.o $(OBJDIR)/bird.o $(OBJDIR)/point.o
	mkdir -p bin
	$(CC) $(CFLAGS) $(OBJDIR)/* -o $(TARGET) $(LIBRARIES)

# Compile objects
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(SRCDIR)/quaternion.h $(SRCDIR)/bird.h $(SRCDIR)/point.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/quaternion.o: $(SRCDIR)/quaternion.cpp $(SRCDIR)/quaternion.h $(SRCDIR)/point.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/quaternion.cpp -o $(OBJDIR)/quaternion.o

$(OBJDIR)/bird.o: $(SRCDIR)/bird.cpp $(SRCDIR)/bird.h $(SRCDIR)/quaternion.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/bird.cpp -o $(OBJDIR)/bird.o

$(OBJDIR)/point.o: $(SRCDIR)/point.cpp $(SRCDIR)/point.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/point.cpp -o $(OBJDIR)/point.o

.PHONY: clean
clean:
	rm -rf bin/* obj/*

.PHONY: all
all: clean $(TARGET)
