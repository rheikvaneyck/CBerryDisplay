#!/bin/bash
# Author: Marcus Nasarek
# License: GPL
# Version: 1.0
# Usage: embed_in_template.sh jpg
# Result: 320x240 pixel x 24 bit Bitmap with jpg included
# jpg is scaled to a 135 Pixel width and 160 Pixel height 
# at position 165 Pixel from the left side and 60 Pixel from top
# de
# Aufruf: embed_in_template.sh jpg
# Ergebnis: 320x240 Pixel x 24 bit Bitmap mit eingefügtem jpg
# Das jpg wird auf 135 Pixel Breite und 160 Pixel Höhe skaliert
# und an Position 165  Pixel von links und 60 Pixel von oben gesetzt

if [[ -z $(which convert) ]]; then
  echo "Ist ImageMagick nicht installiert?"
  exit 1
fi

if [[ -f $1 ]]; then
  SCRIPTPATH=$(dirname "$0")
  TEMPLATE=${SCRIPTPATH}/CurrentTitleTemplate.svg
  JPGFILE=$1
  BMPFILE=${JPGFILE%.*}.bmp
  OPTIONS="-geometry 135x160+165+60 -composite"
  convert "${TEMPLATE}" "${JPGFILE}" ${OPTIONS} "${BMPFILE}"
else
  echo "Kann die JPG-Datei nicht lesen"
fi

RESULT=$(identify ${BMPFILE} | cut -d ' ' -f 2)

if [[ ${RESULT}='BMP' ]]; then
  echo "Konvertierung erfolgreich: ${BMPFILE}"
else 
  echo "Irgendetwas ging schief: ${RESULT}"
fi
