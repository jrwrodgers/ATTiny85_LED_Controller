#include <Adafruit_NeoPixel.h> 
/*
 WS2811/Neopixel pattern switcher for ATtiny85 (and Arduino)
 Requires Adafruit NeoPixel Library
 WS2811 Signal, Digital Pin 4
 Button, Digital Pin 0
 GPL v3
*/

// Define Constants

#define NUM_LEDS 24
#define DATA_PIN 4
#define BTN_PIN 0
#define BTN_DELAY 250
#define NUM_PATTERNS 2
#define CTR_THRESH 16


// Define Variables
const int button = 3;
const int led = 13;
int bounceTime = 50;
int holdTime = 250;
int doubleTime = 500;
int lastReading = LOW;
int hold = 0;
int single = 0;
int LEDstate = 0;
long onTime = 0;
long lastSwitchTime = 0;


uint8_t j = 0;
uint8_t pattern=1;
uint8_t buttonState=0;
uint8_t lastPix=0; 
uint8_t myPix=0;
uint8_t direction=1;
uint8_t counter=0;
uint8_t colors[3];
uint32_t setColor=0;
unsigned long mark;

//Start Script
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

//Setup
void setup(){
    pinMode(BTN_PIN, INPUT);     
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    pinMode(button, INPUT);
}

//Loop
void loop() {
 
//Check button press
chkBtn(digitalRead(BTN_PIN));
 
//Battery check mode
 
//Change led sequence
 
//Change color
 
 
}


void loop() {

 int reading = digitalRead(button);

//first pressed
 if (reading == HIGH && lastReading == LOW) {
   onTime = millis();
 }

//held
 if (reading == HIGH && lastReading == HIGH) {
   if ((millis() - onTime) > holdTime) {
     invertLED();  
     hold = 1;
   }
 }

//released
 if (reading == LOW && lastReading == HIGH) {
   if (((millis() - onTime) > bounceTime) && hold != 1) {
     onRelease();
   }
   if (hold == 1) {
     Serial.println("held");
     digitalWrite(led, LEDstate);
     hold = 0;
   }  
 }
 lastReading = reading;

 if (single == 1 && (millis() - lastSwitchTime) > doubleTime) {
   Serial.println("single press");
   single = 0;
 }

}


void onRelease() {

 if ((millis() - lastSwitchTime) >= doubleTime) {
   single = 1;
   lastSwitchTime = millis();
   return;
 }  

 if ((millis() - lastSwitchTime) < doubleTime) {
   toggleLED();
   Serial.println("double press");
   single = 0;
   lastSwitchTime = millis();
 }  

}

void toggleLED() {
 if (LEDstate == 0) {
   LEDstate = 1;
 } else {
   LEDstate = 0;
 }
 digitalWrite(led, LEDstate);  
}
 
void invertLED() {
 if (LEDstate == 0) {
 digitalWrite(led, 1);
 } else {
 digitalWrite(led, 0);
 }
}




void loop() {
    // if button pressed, advance, set mark
    
   
    // if pattern greater than #pattern reset
    if (pattern > NUM_PATTERNS) { pattern = 1; }
    
    // choose a pattern
    pickPattern(pattern);

    // set direction
    if (direction == 1) { j++;  } else {  j--; }

    if (j > 254) { direction = 0; }
    if (j < 1) { direction = 1; }   
  
}

/* pick a pattern */
void pickPattern(uint8_t var) {
      switch (var) {
        case 1:
          // scanner, color and delay - RGB
          scanner(strip.Color(255,0,0),50);
          scanner(strip.Color(200,0,100),50);
          scanner(strip.Color(64,0,200),50);
        break;
//        case 2:
//          // color wipe random RGB
//          colorWipe(strip.Color(random(255), random(255), random(255)),50);
//        break;
//        case 3:
//          // color wave - Hue/Sat/Bright
//          // hue low (0-359), high (0-359),rate,extra delay
//          wavey(200,240,0.06,0);
//        break;
//        case 4:
//          // rainbow firefly, 1px at random
//          colorFirefly(60);
//          counter++;
//        break;
//        case 5:
//          // rainbow solid
//          rainbow(10);
//          counter++;
//        break;
//        case 6:
//           // bounce in and out
//           // tail len, counter, delay
//           bounceInOut(4,counter,20);
//           counter++;
//        break;
//        case 7:
//           // color wipe from center
//           colorWipeCenter(strip.Color(255,0,0),100);
//           colorWipeCenter(strip.Color(255,64,0),100);
//        break;
//        case 8:
//           // solid color
//           colorFast(strip.Color(255,255,255),0);
//        break;
//        case 9:
//          // fade even or odd
//          // 0-359 Hue value, even/odd, delay
//          fadeEveOdd(200,0,20);
//          fadeEveOdd(300,1,20);
//        break;
//        case 10:
//          // show rainbow
//          rainbowCycle(10);
//         break; 
//         case 11:
//          CylonBounce(0xff, 0, 0, 4, 10, 50);
//         break;
//         case12:
//          RunningLights(0xff,0xff,0x00, 50);
//         break;
         case2:
          meteorRain(0xff,0xff,0xff,10, 64, true, 30);
         break;
      }
}

/* check button state */
boolean chkBtn(int buttonState) {
   if (buttonState == HIGH && (millis() - mark) > BTN_DELAY) {
       j = 0;
       mark = millis();
       pattern++;
       return true;
    } 
    else { return false; }
}

void colorFirefly(int wait) {
        if(myPix != lastPix) {
          if(counter<CTR_THRESH) {
            float colorV = sin((6.28/30)*(float)(counter)) *255;
            HSVtoRGB((359/CTR_THRESH)*counter, 255, colorV, colors);
            strip.setPixelColor(myPix, colors[0], colors[1], colors[2]);
           strip.show();
           delay(wait);
          } else {
            lastPix=myPix;
            counter=0;
            colorFast(0,0);
          }
        } else {
          myPix=random(0,strip.numPixels());
        }
  
}

// Fill the dots one after the other with a color
// Modified from Neopixel sketch to break on button press

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      if(chkBtn(digitalRead(BTN_PIN))) { break; }
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// color wipe from center
void colorWipeCenter(uint32_t c, uint8_t wait) {
  uint8_t mid=strip.numPixels()/2;
  strip.setPixelColor(mid,c);
  for(uint16_t i=0; i<=strip.numPixels()/2; i++) {
      if(chkBtn(digitalRead(BTN_PIN))) { break; }
      strip.setPixelColor(mid+i, c);
      strip.setPixelColor(mid-i, c);
      strip.show();
      delay(wait);
  }
}

void showStrip() {
   strip.show();
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

// fast version 
void colorFast(uint32_t c, uint8_t wait) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
    }
    strip.show();
    delay(wait);
}

// Rainbow Cycle, modified from Neopixel sketch to break on button press
void rainbowCycle(uint8_t wait) {
    uint16_t i;

    //  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
    // }
}

void rainbow(uint8_t wait) {
    uint16_t i;

    //for(j=0; j<256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
    // }
}

// scanner

void scanner(uint32_t c,uint8_t wait) {
        for(int i=0; i< strip.numPixels(); i++) {
            if(chkBtn(digitalRead(BTN_PIN))) { break; }

            colorFast(0,0);
            strip.setPixelColor(i,c);
            strip.show();
            delay(wait);
        }
        for(int i=strip.numPixels(); i>0; i--) {
           if(chkBtn(digitalRead(BTN_PIN))) { break; }
            colorFast(0,0);
            strip.setPixelColor(i,c);
            strip.show();
            delay(wait);
        }    
}

// scanner to midpoint
void bounceInOut(uint8_t num, uint8_t start,uint8_t wait) {
  colorFast(0,0);
  uint8_t color=200;
  for(int q=0; q < num; q++) {
    if(strip.numPixels()-start >= 0 && start < NUM_LEDS) {
          strip.setPixelColor(start+q, strip.Color(0,color,0));
          strip.setPixelColor(strip.numPixels()-start-q, strip.Color(0,color,0));
      }   
    color=round(color/2.0);
    strip.show();
    delay(wait);
  }
  if(counter > strip.numPixels()) { counter=0; }
}

void fadeEveOdd(int c1,byte rem,uint8_t wait) {
              for(int j=0; j < CTR_THRESH; j++) {
                      for(int i=0; i< strip.numPixels(); i++) {
                        if(i % 2== rem) {
                           HSVtoRGB(c1,255,(255/CTR_THRESH)*j,colors);
                           strip.setPixelColor(i,colors[0],colors[1],colors[2]);
                         }
                      }           
                      if(chkBtn(digitalRead(BTN_PIN))) { break; }
                      strip.show();
                      delay(wait);
                }
                for(int j=CTR_THRESH; j >= 0; j--) {
                      for(int i=0; i< strip.numPixels(); i++) {
                        if(i % 2== rem) {
                           HSVtoRGB(c1,255,(255/CTR_THRESH)*j,colors);
                           strip.setPixelColor(i,colors[0],colors[1],colors[2]);
                         }
                      }             
                     if(chkBtn(digitalRead(BTN_PIN))) { break; }
                      strip.show();
                      delay(wait);
                } 
}

// twinkle random number of pixels
void twinkleRand(int num,uint32_t c,uint32_t bg,int wait) {
  // set background
   colorFast(bg,0);
   // for each num
   for (int i=0; i<num; i++) {
     strip.setPixelColor(random(strip.numPixels()),c);
   }
  strip.show();
  delay(wait);
}

// sine wave, low (0-359),high (0-359), rate of change, wait
void wavey(int low,int high,float rt,uint8_t wait) {
  float in,out;
  int diff=high-low;
  int step = diff/strip.numPixels();
  for (in = 0; in < 6.283; in = in + rt) {
       for(int i=0; i< strip.numPixels(); i++) {
           out=sin(in+i*(6.283/strip.numPixels())) * diff + low;
           HSVtoRGB(out,255,255,colors);
           strip.setPixelColor(i,colors[0],colors[1],colors[2]);
       }
           strip.show();
           delay(wait);
           if(chkBtn(digitalRead(BTN_PIN))) { break; }
  }
}

// sine wave, intensity
void waveIntensity(float rt,uint8_t wait) {
  float in,level;
  for (in = 0; in < 6.283; in = in + rt) {
       for(int i=0; i< strip.numPixels(); i++) {
         // sine wave, 3 offset waves make a rainbow!
        level = sin(i+in) * 127 + 128;
        // set color level 
        strip.setPixelColor(i,(int)level,0,0);
       }
           strip.show();
           delay(wait);
           if(chkBtn(digitalRead(BTN_PIN))) { break; }
  }
}

// helpers 

uint32_t Wheel(byte WheelPos) {
    if (WheelPos < 85) {
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    } else if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else {
        WheelPos -= 170;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}

void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
 
  delay(ReturnDelay);
}


void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
 
  for(int j=0; j<NUM_LEDS*2; j++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
     
      showStrip();
      delay(WaveDelay);
  }
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
 
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      }
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    strip.setPixelColor(ledNo, r,g,b);

}
// HSV to RGB colors
// hue: 0-359, sat: 0-255, val (lightness): 0-255
// adapted from http://funkboxing.com/wordpress/?p=1366
void HSVtoRGB(int hue, int sat, int val, uint8_t * colors) {
    int r, g, b, base;
    if (sat == 0) { // Achromatic color (gray).
        colors[0] = val;
        colors[1] = val;
        colors[2] = val;
    } else {
        base = ((255 - sat) * val) >> 8;
        switch (hue / 60) {
        case 0:
            colors[0] = val;
            colors[1] = (((val - base) * hue) / 60) + base;
            colors[2] = base;
            break;
        case 1:
            colors[0] = (((val - base) * (60 - (hue % 60))) / 60) + base;
            colors[1] = val;
            colors[2] = base;
            break;
        case 2:
            colors[0] = base;
            colors[1] = val;
            colors[2] = (((val - base) * (hue % 60)) / 60) + base;
            break;
        case 3:
            colors[0] = base;
            colors[1] = (((val - base) * (60 - (hue % 60))) / 60) + base;
            colors[2] = val;
            break;
        case 4:
            colors[0] = (((val - base) * (hue % 60)) / 60) + base;
            colors[1] = base;
            colors[2] = val;
            break;
        case 5:
            colors[0] = val;
            colors[1] = base;
            colors[2] = (((val - base) * (60 - (hue % 60))) / 60) + base;
            break;
        }

    }
}
