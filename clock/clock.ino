#include <Adafruit_NeoPixel.h>
#include "RTClib.h"

#define PIN        6 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 450 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
RTC_DS1307 rtc;

uint8_t rtc_minute = 0;
uint8_t rtc_hour = 0; 

uint16_t mins[12][2] = {
{0,0},
{78,88},
{29,37},
{57,77},
{38,56},
{38,56},
{13,27},
{38,56},
{38,56},
{57,77},
{29,37},
{78,88}
};

uint16_t hrs[12][2] = {
{137,147},
{161,170},
{120,135},
{173,186},
{188,202},
{236,245},
{219,235},
{203,218},
{246,257},
{258,268},
{269,286},
{308,327}
};

uint16_t helpers[5][2] = {
{0,11},
{113,119},
{149,160},
{287,305},
{78,88}
};

void print_time() {
	int i;
  	pixels.clear(); // Set all pixel colors to 'off'

	//print "it is"
	for (i = helpers[0][0]; i <= helpers[0][1]; i++) {
			pixels.setPixelColor(i, 0xFF);
	}
	//print "past"
	if ((rtc_minute <= 6) && (rtc_minute != 0)) {
		for (i = helpers[2][0]; i <= helpers[2][1]; i++) {
			pixels.setPixelColor(i, 0xFF);
		}
	}
	//print "to"
	if ((rtc_minute > 6) && (rtc_minute != 0)) {
		for (i = helpers[1][0]; i <= helpers[1][1]; i++) {
			pixels.setPixelColor(i, 0xFF);
		}
	}
	//print "oclock"
	if (rtc_minute ==  0) {
		for (i = helpers[3][0]; i <= helpers[3][1]; i++) {
			pixels.setPixelColor(i, 0xFF);
		}
	}
	//print "five"
	if ((rtc_minute == 5) || (rtc_minute == 7)) {
		for (i = helpers[4][0]; i <= helpers[4][1]; i++) {
			pixels.setPixelColor(i, 0xFF);
		}
	}
	//print "hour"
	for (int i = hrs[rtc_hour][0]; i <= hrs[rtc_hour][1]; i++) {
			pixels.setPixelColor(i, 0xFF);
	}
	//print "minute"
	for (int i = mins[rtc_minute][0]; i <= mins[rtc_minute][1]; i++) {
			pixels.setPixelColor(i, 0xFF);
	}
}

void print_clock() {
	int blue;
	for (int i = 0; i <= 327; i++) { 
		if (pixels.getPixelColor(i) == 0xFF) {
			//blue = map(i, 0, 327, 0, 100);
			if (i%2) 
				pixels.setPixelColor(i, pixels.Color(255, 0, 0));
			else 
				pixels.setPixelColor(i, pixels.Color(0, 0, 255));

		}
		else {
			//pixels.setPixelColor(i, pixels.Color(0, 128, (100- blue) ));
			pixels.setPixelColor(i, pixels.Color(0,0,0));
		}
	
	}
	pixels.show();
}

void setup() {
	Serial.begin(57600);
	delay(500);
	if (! rtc.begin()) {
    	Serial.println("Couldn't find RTC");
    while (1);
  	}
  	if (! rtc.isrunning()) {
    	Serial.println("RTC is NOT running!");
  	}
 
   	rtc.adjust(DateTime(2014, 1, 21, 4, 0, 0));

  	pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  	pixels.clear(); // Set all pixel colors to 'off'
	pixels.show();
}

void loop() {
    DateTime now = rtc.now();
	int counter = now.second();
	rtc_hour = now.hour() - 1;
	if ((counter%5) == 0) {
		Serial.println(counter, DEC);
		rtc_minute = counter/5;
		print_time();
		print_clock();
		delay(1000);
	}
}


