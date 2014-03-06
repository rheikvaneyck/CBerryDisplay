## CBerryDisplay

CBerryDisplay ist eine kleine Sammlung von Scripten und C-Code für das TFT-Display C-Berry von admatec. 

![C-Berry Display](http://admatec.de/sites/default/files/images/C_Berry.JPG)

Das Display hat einen integrierten Grafikprozessor und kann daher neben Textausgaben auch geometrische Figuren, Bitmap-Grafiken oder Funktionen darstellen.

### Voraussetzungen

* Raspberry Pi: [offizielle Webseite](http://www.raspberrypi.org/)
* Debian wheezy Image: [Download von Webseite](http://www.raspberrypi.org/downloads)
* bcm2835 lib: [Download von Webseite](http://www.airspayce.com/mikem/bcm2835/bcm2835-1.36.tar.gz)
* C-Berry Software: [Download von Webseite](http://admatec.de/sites/default/files/downloads/C-Berry.tar.gz)

### Installation

Das Repository kann als zip-Datei von der guitHub-Webseite oder mit git heruntergeladen werden:

`git clone https://github.com/rheikvaneyck/CBerryDisplay.git`

Danach werden die die C-Programme im Verzeichnis *c-code/* kompiliert und ins *bin/*-Verzeichnis kopiert:

```
cd c-code
make
make install 
```

