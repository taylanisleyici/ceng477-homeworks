CC = g++
CFLAGS = -g
LIBS = `pkg-config --cflags --libs freetype2` -lglfw -lGLU -lGL -lGLEW
TARGET = main

all: $(TARGET)

$(TARGET): main.cpp
	$(CC) $(CFLAGS) main.cpp -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
