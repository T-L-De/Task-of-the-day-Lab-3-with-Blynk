/*************************************************************

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  NOTE:
  BlynkTimer provides SimpleTimer functionality:
    http://playground.arduino.cc/Code/SimpleTimer

  App project setup:
    Value Display widget attached to Virtual Pin V5
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
//#define BLYNK_TEMPLATE_ID           "TMPLxxxxxx"
//#define BLYNK_DEVICE_NAME           "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//Output Pin
#define LED_R 16
#define LED_Y 17
#define LED_G 5

//Input Pin
#define LDR_Pin 34

#define VIN 3.3 // V power voltage, 3.3v in case of NodeMCU
#define R 10000 // Voltage devider resistor value

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Esp32";
char pass[] = "projectfyp999";
char auth[] = "YBnzHn4b1YSt_qpcDi4pmfwd-hGXrrbg";

WidgetLED led0(V0);
WidgetLED led1(V1);
WidgetLED led2(V2);
BlynkTimer timer;

int get_lux(void)
{ float LDR_Vout = (analogRead(LDR_Pin)* VIN / float(4095));// Conversion analog to voltage
  float RLDR = (R * LDR_Vout)/(VIN - LDR_Vout); // Conversion voltage to resistance in Ohm
  int lux = 170/(RLDR/1000); // Conversion resitance to lumen, RLDR/1000 - convert Ohm to KOhm
  return lux;
}


// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  int lux_value = get_lux();
  //float LM35_value = get_LM35();
  
  Blynk.virtualWrite(V5, lux_value);
  
  if( lux_value > 150 ){
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_Y, LOW);
    digitalWrite(LED_R, LOW);
    led2.on();
    led0.off();
    led1.off();
    } 
  else if(( lux_value > 20 )&&( lux_value < 150)){
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_Y, HIGH);
    digitalWrite(LED_R, LOW);
    led1.on();
    led0.off();
    led2.off();
    }
 else {
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_Y, LOW);
    digitalWrite(LED_R, HIGH);
    led0.on();
    led1.off();
    led2.off();
    }
   
  //Blynk.virtualWrite(V8, PB_state);
  
  Serial.println(String(lux_value)+" LUX");

  Serial.println();
}

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 3
BLYNK_WRITE(V3)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V3 to a variable
  digitalWrite(LED_BUILTIN, pinValue);
  Serial.print("LED on ESP32 value is: ");
  Serial.println(pinValue);
}


void setup()
{
  // Debug console
  Serial.begin(115200);
  
  pinMode(LED_R, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(PB_Pin, INPUT_PULLUP);
  
  Blynk.begin(auth, ssid, pass);
  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
