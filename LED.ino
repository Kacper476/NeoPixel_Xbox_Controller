#include <XboxSeriesXControllerESP32_asukiaaa.hpp>
#include <Adafruit_NeoPixel.h>

#define PIN_NEO_PIXEL 2   // The ESP32 pin GPIO16 connected to NeoPixel
#define NUM_PIXELS 16     // The number of LEDs (pixels) on NeoPixel LED strip

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);



int mode=0;


XboxSeriesXControllerESP32_asukiaaa::Core xboxController;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting NimBLE Client");
  xboxController.begin();
   NeoPixel.setBrightness(255);
}

void loop() {
  xboxController.onLoop();

  if (xboxController.isConnected()) {
    buttons();
    Serial.println(mode);

    switch (mode) {
      case 0:
        connection();
        break;
      case 1:
        fate();
        break;
      case 2:
        rainbowCycle();
        break;
      case 3:
        changeColor();
        break;
      case 4:
        changeColorBasedOnAnalogInput();
        break;
      default:
        break;
    }
  } 
  else {
    handleDisconnectedController();
  }

  delay(500);
}

void handleDisconnectedController() {
  Serial.println("Controller not connected");
  connection_wait();

  if (xboxController.getCountFailedConnection() > 2) {
    ESP.restart();
  }}





void buttons()
{
  

  // Obsługa przycisku X
  if (xboxController.xboxNotif.btnX == 1) {
    mode = 1;  
  }

  // Obsługa przycisku Y
  if (xboxController.xboxNotif.btnY == 1) {
    mode = 2;   
  }

  // Obsługa przycisku B
  if (xboxController.xboxNotif.btnB == 1) {
    mode = 3;   
  }

  // Obsługa przycisku A
  if (xboxController.xboxNotif.btnA == 1) {
  mode = 4;
  }


}

void connection_wait()
{
 // Turn on all LEDs to red
  
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(255, 255, 255));
  }
  NeoPixel.show();   
  delay(1000);      

   for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(255, 0, 0));
  }
 
  NeoPixel.show();   
  delay(1000);      
 

}

void connection()
{
 for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(0, 255, 0));
  }
  NeoPixel.show();   

}

void fate()
{
   // Fading from red to blue
  for (int i = 0; i <= 255; i++) {
    for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(255 - i, 0, i));
       buttons();
      if(mode!=1)break;
    }
    if(mode!=1)break;
    NeoPixel.show();
    delay(1);  
  }

  delay(1); 



  // Fading from blue to red
  for (int i = 0; i <= 255; i++) {
    for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(i, 0, 255 - i));
  buttons();
      if(mode!=1)break;
    }
   if(mode!=1)break;
    NeoPixel.show();
    delay(1);  
  }

  delay(1);  
}

void rainbowCycle() {
  int cycleSpeed = 1000;  

  
  uint32_t colors[] = {
    NeoPixel.Color(255, 0, 0),      // Czerwony
    NeoPixel.Color(255, 165, 0),    // Pomarańczowy
    NeoPixel.Color(255, 255, 0),    // Żółty
    NeoPixel.Color(0, 255, 0),      // Zielony
    NeoPixel.Color(0, 0, 255),      // Niebieski
    NeoPixel.Color(128, 0, 128)     // Fioletowy
  };

  int numColors = sizeof(colors) / sizeof(colors[0]);

  for (int i = 0; i < numColors; i++) {
    for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
      NeoPixel.setPixelColor(pixel, colors[i]);
      buttons();
      if(mode!=2)break;
    }
    NeoPixel.show();
    delay(cycleSpeed);
  }
}


void changeColor() {
  int bul = 0;
  int i = 0;
  uint32_t colors[] = {
    NeoPixel.Color(255, 0, 0),      // Czerwony
    NeoPixel.Color(255, 165, 0),    // Pomarańczowy
    NeoPixel.Color(255, 255, 0),    // Żółty
    NeoPixel.Color(0, 255, 0),      // Zielony
    NeoPixel.Color(0, 0, 255),      // Niebieski
    NeoPixel.Color(128, 0, 128)     // Fioletowy
  };

while(true){
  
  int numColors = sizeof(colors) / sizeof(colors[0]);
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    NeoPixel.setPixelColor(pixel, colors[i]);
    buttons();
    if (mode != 3) return;  
  }
  NeoPixel.show();
  buttons();
  if (mode != 3) return;  
  
  while(true){
  buttons();
  if (mode != 3) return;
   if (xboxController.xboxNotif.btnRS == 1 && i != 5) 
   {
    i++;
    while(xboxController.xboxNotif.btnRS == 0);
    break;
   }


if (xboxController.xboxNotif.btnLS == 1 && i != 0) 
   {
    i--;
    while(xboxController.xboxNotif.btnLS == 0);
    break;
   }


 }
delay(100);  
  }


}



void changeColorBasedOnAnalogInput() {
  float LanalogX = xboxController.xboxNotif.joyLHori / 256.0;  
  float RanalogX = xboxController.xboxNotif.joyRHori / 256.0;  
  float trigLT = xboxController.xboxNotif.trigLT / 256.0;
  Serial.println(LanalogX);
  Serial.print(" ");
  Serial.print(RanalogX);
Serial.print(" ");
  Serial.print(trigLT);


  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(LanalogX, RanalogX, trigLT * 64));  // Scale to [0, 255]
    buttons();
    if (mode != 4) break;
  }

  NeoPixel.show();
}

