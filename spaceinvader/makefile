TARGET=cannon

all: $(TARGET)

%.o:%.c 
	gcc -g -I.. -Wall -pedantic `sdl2-config --cflags` -c  $< -o $@

$(TARGET): $(TARGET).o ../primlib.o target.o player.o list.o level.o
	gcc -g -I.. -Wall -pedantic `sdl2-config --cflags` ../primlib.o target.o player.o list.o level.o $< -o $@ -lSDL2_gfx `sdl2-config --libs` -lm

level.o: level.c level.h

list.o: list.c list.h

player.o: player.c player.h

target.o: target.c target.h

../primlib.o: ../primlib.c ../primlib.h

$(TARGET).o: $(TARGET).c ../primlib.h target.h player.h list.h level.h

clean:
	-rm $(TARGET) $(TARGET).o ../primlib.o target.o player.o list.o level.o