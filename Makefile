CC = g++
CFLAGS = -g -Wall
TARGET = bin/main
OBJDIR = obj
SRCDIR = src
LIBRARIES = -I /usr/X11R6/include/ -L /usr/X11R6/lib64/ -lglut -lGL -lGLU -lX11 -lXmu -lXi -lm
UNAME = $(shell uname)

ifeq ($(UNAME), Darwin)
	LIBRARIES = -framework OpenGL -framework GLUT
endif

# Link for target
$(TARGET): $(OBJDIR)/main.o $(OBJDIR)/quaternion.o $(OBJDIR)/fish.o $(OBJDIR)/point.o $(OBJDIR)/bubble.o
	mkdir -p bin
	$(CC) $(CFLAGS) $(OBJDIR)/* -o $(TARGET) $(LIBRARIES)


# Compile objects
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(SRCDIR)/fish.h $(SRCDIR)/bubble.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/quaternion.o: $(SRCDIR)/quaternion.cpp $(SRCDIR)/quaternion.h $(SRCDIR)/point.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/quaternion.cpp -o $(OBJDIR)/quaternion.o

$(OBJDIR)/fish.o: $(SRCDIR)/fish.cpp $(SRCDIR)/fish.h $(SRCDIR)/quaternion.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/fish.cpp -o $(OBJDIR)/fish.o

$(OBJDIR)/point.o: $(SRCDIR)/point.cpp $(SRCDIR)/point.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/point.cpp -o $(OBJDIR)/point.o

$(OBJDIR)/bubble.o: $(SRCDIR)/bubble.cpp $(SRCDIR)/bubble.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/bubble.cpp -o $(OBJDIR)/bubble.o

.PHONY: clean
clean:
	rm -rf bin obj

.PHONY: all
all: clean $(TARGET)
