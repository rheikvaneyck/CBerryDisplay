all: loadbmp

loadbmp: loadbmp.o tft.o RAIO8870.o bmp.o 
	gcc loadbmp.o tft.o RAIO8870.o bmp.o -lbcm2835 -lrt -lm -o bmp_test

tft.o: tft.c tft.h
	gcc -Os -c tft.c

loadbmp.o: loadbmp.c 
	gcc -Os -c loadbmp.c
	
RAIO8870.o: RAIO8870.c RAIO8870.h
	gcc -Os -c RAIO8870.c

bmp.o: bmp.c bmp.h RAIO8870.h
	gcc -Os -c bmp.c

clean:
	rm -rf *o loadbmp

