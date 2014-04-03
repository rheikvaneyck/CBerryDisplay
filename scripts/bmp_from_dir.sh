#!/bin/bash
# Author: Marcus Nasarek
# License: GPL
# Version: 1.0
# Usage: bmp_from_dir [path]
# Result: 320x240 pixel x 24 bit Bitmaps with jpg found in path included

FS=$IFS
IFS=$(echo -en "\n\b")

if [[ -z "$1" ]]; then
	SEARCHDIR="$(pwd)"
else
	SEARCHDIR="$1"
fi

if [[ ! -d ${SEARCHDIR} ]]; then
	echo "Verzeichnis ${SEARCHDIR} existiert nicht"
	exit 1
fi

SUBDIRS="$(find ${SEARCHDIR} -type d)"

for SUBDIR in ${SUBDIRS}; do
	echo "----------"
	MP3FILES=$(ls "${SUBDIR}"/*.mp3)
	echo ${MP3FILES}[1]
done

IFS=$FS
