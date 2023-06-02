// This example assumes you clipped a nail to the A1 pad on the CPX board
/*
  Pinouts:
A0 (a.k.a D12) - This is a special pin that can do true analog output so it's great for playing audio clips. In can be digital I/O, or analog I/O, but if you do that it will interfere with the built-in speaker. This is the one pin that cannot be used for capacitive touch.
A1 / D6 - This pin can be digital I/O, or analog Input. This pin has PWM output and can be capacitive touch sensor
A2 / D9 - This pin can be digital I/O, or analog Input. This pin has PWM output and can be capacitive touch sensor
A3 / D10 - This pin can be digital I/O, or analog Input. This pin has PWM output and can be capacitive touch sensor
A4 / D3 - This pin can be digital I/O, or analog Input. This pin is also the I2C SCL pin, and can be capacitive touch sensor
A5 / D2 - This pin can be digital I/O, or analog Input. This pin is also the I2C SDA pin, and can be capacitive touch sensor
A6 / D0 - This pin can be digital I/O, or analog Input. This pin has PWM output, Serial Receive, and can be capacitive touch sensor
A7 / D1 - This pin can be digital I/O, or analog Input. This pin has PWM output, Serial Transmit, and can be capacitive touch sensor
*/

#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

AsyncDelay delay_3s;

int capacitence = 0;

float tempF = 0;

int light = 0;

int capMap = 0;

const byte switchPin = 7;

volatile bool switchState = 0;

volatile bool switchFlag = 0;




void setup() {

Serial.begin(9600);

CircuitPlayground.begin();

delay_3s.start(3000, AsyncDelay::MILLIS);

pinMode(switchPin, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(switchPin), switchChanged, CHANGE);

}

void loop() {

  if (switchFlag)
  {
    delay(5);
    switchState = digitalRead(7);
    switchFlag = 0;
  }


 capacitence = CircuitPlayground.readCap(6);
 tempF = CircuitPlayground.temperatureF();
 light = CircuitPlayground.lightSensor();


  LEDsGreen();

  if (delay_3s.isExpired()) {
    
    displayCapacitence();
    displayTempF();
    displayLight(); 
    Serial.println(switchState);
    delay_3s.repeat(); // Count from when the delay expired, not now
  }  
    
  //Capacitance based function + threshold  
  if (capacitence < 700 ){
    for (int i = 0; i < 10; i++){
      CircuitPlayground.setPixelColor(i, 150, 0, 0);
      delay(100); 
    }
  }
 
  if (light < 50)
  {
    //This function indicates the plant needs sun
    sparkle();
  }  



  if(switchState)
  {
    //This function is on or off depending on the switch state for night mide
    //This is because temperature will drop during night 
    if (tempF > 87)
    {
     CircuitPlayground.playTone(500, 2000); //plays a 500 hz tone for 2000 ms (2 sec)
    }    
  }
}


void displayCapacitence(){
  Serial.print("Capacitence: ");
  Serial.println(capacitence);  
}

void displayTempF(){
  Serial.print("TempF: ");
  Serial.println(tempF);  
}

void displayLight(){
  Serial.print("Light: ");
  Serial.println(light);  
}

void switchChanged()
{
  switchFlag = 1;
}

void LEDsGreen()
{
  //Could also map to a brightness level. This way as cap drops (soil moisture decreaes) the lights will get dimmer 
  //If the light then drops below the threshold defined earlier, it will do the red display listed earlier
  capMap = map(capacitence, 220, 1017, 0, 255);
  for (int i = 0; i < 10; i++){
   CircuitPlayground.setPixelColor(i, 0, 150, 0); 
  }
}

void sparkle(){
  CircuitPlayground.clearPixels();
  for(int i = 0; i < 11; i++){
    int ranLED = random(9);
    CircuitPlayground.setPixelColor(ranLED, 255, 255, 0);
    delay(250);
    CircuitPlayground.clearPixels();
    delay(250);
  }
}

