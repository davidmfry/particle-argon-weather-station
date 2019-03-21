/*
 * Project weather-station
 * Description:
 * Author:
 * Date:
 */

#include "Adafruit_MPL3115A2.h"
#include "ThingSpeak.h"

Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

// Thing Speak config
TCPClient client;
int sendDelay = 10000;

unsigned int myChannelNumber = 731870;
const char * myWriteAPIKey = "233HUX99399K7CAH";

char buffer[100];
char buffer2[100];
char buffer3[100];

void setup() {
  // Put initialization like pinMode and begin functions here.
  pinMode(A5, OUTPUT);
  pinMode(D7, OUTPUT);

  // Serial.begin(115200);
  // delay(5000);

  ThingSpeak.begin(client);
  Particle.publish("PowerON", "Weather Station", 60, PRIVATE);
  
  // Serial.println("Adafruit_MPL3115A2 test!");
  
  while(! baro.begin()) {
    Serial.println("Couldnt find sensor");
    delay(1000);
  }
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

  
   float pascals = baro.getPressure();
  // Our weather page presents pressure in Inches (Hg)
  // Use http://www.onlineconversion.com/pressure.htm for other units
  Serial.print(pascals/3377); Serial.println(" Inches (Hg)");
  snprintf(buffer, sizeof(buffer), "%f Inches (Hg)", pascals);
  Particle.publish("pressure", buffer);

  float altm = baro.getAltitude();
  snprintf(buffer, sizeof(buffer), "%f meters", altm);
  Particle.publish("altm", buffer);

  float tempC = baro.getTemperature();
  snprintf(buffer, sizeof(buffer), "Temp is %f*C", tempC);
  Particle.publish("temp", buffer);

  float tempToF = (tempC * 1.8000) + 32;
  float metersToFeet = altm * 3.2808;
  ThingSpeak.setField(1, tempToF);
  ThingSpeak.setField(2, metersToFeet);
  ThingSpeak.setField(3, pascals );
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  digitalWrite(A5, HIGH);
  digitalWrite(D7, HIGH);
  
  delay(sendDelay);
  
  digitalWrite(A5, LOW);
  digitalWrite(D7, LOW);
  
  
  // delay(1000);
  
  
  // delay(1000);
}