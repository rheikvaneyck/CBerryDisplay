#!/bin/bash
# Author: Marcus Nasarek
# License: GPL
# Version: 1.0
# Usage: convert_to_bmp.sh jpg
# Result: 320x240 pixel x 24 bit Bitmap 

if [[ -z $(which convert) ]]; then
  echo "Ist ImageMagick nicht installiert?"
  exit 1
fi

if [[ -f $1 ]]; then
  JPGFILE=$1
  BMPFILE=${JPGFILE%.*}.bmp
  OPTIONS="-resize 320x240 -background black -compose Copy -gravity center -extent 320x240 -depth 24 -alpha remove -alpha off -compress none BMP3:"
  convert ${JPGFILE} ${OPTIONS}${BMPFILE}
else
  echo "Kann die JPG-Datei nicht lesen"
fi

RESULT=$(identify ${BMPFILE} | cut -d ' ' -f 2)

if [[ ${RESULT}='BMP' ]]; then
  echo "Konvertierung erfolgreich: ${BMPFILE}"
else 
  echo "Irgendetwas ging schief: ${RESULT}"
fi
