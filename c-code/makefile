all: loadbmp

loadbmp: loadbmp.o tft.o RAIO8870.o bmp.o 
	gcc loadbmp.o tft.o RAIO8870.o bmp.o -lbcm2835 -lrt -lm -o loadbmp

writetext: writetext.o tft.o RAIO8870.o
	gcc writetext.o tft.o RAIO8870.o -lbcm2835 -lrt -lm -o writetext

clearscreen: clearscreen.o tft.o RAIO8870.o
	gcc clearscreen.o tft.o RAIO8870.o -lbcm2835 -lrt -lm -o clearscreen

tft.o: tft.c tft.h
	gcc -Os -c tft.c

loadbmp.o: loadbmp.c loadbmp.h
	gcc -Os -c loadbmp.c

writetext.o: writetext.c writetext.h
	gcc -Os -c writetext.c

clearscreen.o: clearscreen.c clearscreen.h
	gcc -Os -c clearscreen.c
	
RAIO8870.o: RAIO8870.c RAIO8870.h
	gcc -Os -c RAIO8870.c

bmp.o: bmp.c bmp.h RAIO8870.h
	gcc -Os -c bmp.c

clean:
	rm -rf *o loadbmp writetext clearscreen

