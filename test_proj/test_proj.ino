#include <Arduino.h> 
#include <EEPROM.h> 
#define USE_SERIAL Serial
#include <ESP8266WiFi.h> 
#include <ESP8266HTTPClient.h>
const char * ssid = "dlink";
const char * password = "Vijayv12";
HTTPClient http;
int count=0;
byte sensorInterrupt = 0; // 0 = digital pin 2

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;

volatile byte pulseCount;

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;

void setup() {
    Serial.begin(115200); // Start the Serial communication to send messages to the computer
    delay(10);
    Serial.println('\n');

    startWIFI();

    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    oldTime = 0;
}
void loop() {
   if ((millis() - oldTime) > 1000) // Only process counters once per second
    {
        detachInterrupt(sensorInterrupt);
        flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
        oldTime = millis();
        flowMilliLitres = (flowRate / 60) * 1000;
        totalMilliLitres += flowMilliLitres;
        unsigned int frac;

        // Print the flow rate for this second in litres / minute
        Serial.print("Flow rate: ");
        Serial.print(int(flowRate)); // Print the integer part of the variable
        Serial.print("."); // Print the decimal point
        // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
        frac = (flowRate - int(flowRate)) * 10;
        Serial.print(frac, DEC); // Print the fractional part of the variable
        Serial.print("L/min");
        Serial.print("  Output Liquid Quantity: "); // Output separator
        Serial.print(totalMilliLitres);
        Serial.println("mL");

        pulseCount = 0;

        // Enable the interrupt again now that we've finished sending output
        attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
           
           
           if (WiFi.status() == WL_CONNECTED && totalMilliLitres>=1000) {       
              senddata();
            }
}
}

void pulseCounter() {
    // Increment the pulse counter
    pulseCount++;
} 


void startWIFI(void) {
    
    
    WiFi.begin(ssid, password); // Connect to the network
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println(" ...");
  
    int i = 0;
  
    delay(100);         
    
      while (WiFi.status() != WL_CONNECTED) { 
        delay(2000);
        Serial.print(++i);
        Serial.print('.');
           
        delay(100);  
    }
    delay(2000);
    Serial.print('\n');
    Serial.print("Connection established!");
    Serial.print("IP address:\t");
    Serial.print(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer

}

void senddata(){
  
              HTTPClient http;
  
              http.begin("http://192.168.0.100/add_volume?value=vijaya");
              //http.addHeader("Content-Type", "application/json\r\n");
              
              int httpCode = http.GET();
              Serial.print(httpCode);

              if (httpCode ==HTTP_CODE_OK) {
                   totalMilliLitres =totalMilliLitres-1000;
                   flowMilliLitres=0;
                }
              else{Serial.printf("GET... failed");
                     
              }
        
             http.end();
  
  }

