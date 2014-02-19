CC = g++
CFLAGS = -g -Wall
TARGET = bin/main
OBJDIR = obj
SRCDIR = src
FRAMEWORKS = -framework OpenGL -framework GLUT

# Link for target
$(TARGET): $(OBJDIR)/main.o $(OBJDIR)/trackball.o $(OBJDIR)/GLScreenCapturer.o $(OBJDIR)/quaternion.o
	$(CC) $(CFLAGS) $(OBJDIR)/* -o $(TARGET) $(FRAMEWORKS)

# Compile objects
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(SRCDIR)/trackball.h $(SRCDIR)/GLScreenCapturer.h $(SRCDIR)/quaternion.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/quaternion.o: $(SRCDIR)/quaternion.cpp $(SRCDIR)/quaternion.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/quaternion.cpp -o $(OBJDIR)/quaternion.o

$(OBJDIR)/trackball.o: $(SRCDIR)/trackball.cpp $(SRCDIR)/trackball.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/trackball.cpp -o $(OBJDIR)/trackball.o

$(OBJDIR)/GLScreenCapturer.o: $(SRCDIR)/GLScreenCapturer.cpp $(SRCDIR)/GLScreenCapturer.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/GLScreenCapturer.cpp -o $(OBJDIR)/GLScreenCapturer.o

.PHONY: clean
clean:
	rm -rf bin/* obj/*

.PHONY: all
all: clean $(TARGET)