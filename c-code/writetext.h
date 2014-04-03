#ifndef WRITETEXT_H
#define WRITETEXT_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include "RAIO8870.h"

// delete screen and write text
// ---------------------------------------------
void WriteText( uint16_t pos_y, unsigned char *text, uint8_t fg_color);

#endif
