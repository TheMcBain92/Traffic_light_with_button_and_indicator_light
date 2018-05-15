/*
  Traffic Lights

  Runs a UK traffic light sequence. See Constants for light pin numbers.

  The circuit:
  - RED LED attached from pin 9 to ground
  - Green LED attached from pin 10 to ground
  - RED LED attached from pin 11 to ground
  - RED LED attached from pin 3 to ground
  - Yellow LED attached from pin 4 to ground
  - Green LED attached from pin 5 to ground
    
  - pushbutton attached to pin 2 from +3.3V
  - 10K resistor attached to pin 2 from ground

  created 2018
  by Stephen McBain <http://www.0j0.org>
*/

// include librarys:
#include <SPI.h>
#include <SD.h>
#include <DS3231.h>
// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);

File dataFile;

// constants won't change. They're used here to set pin numbers:
const int crossing = 2;     // the number of the pushbutton pin
const int red =  3;         // the number of the LED pin
const int yellow =  4;      // the number of the LED pin
const int green =  5;       // the number of the LED pin
const int crossred = 6;     // Crossing light Red
const int crossgreen = 7;   //Crossing Light Green
const int buttonlight = 8;  // Button Pressed Light
const int buzzer = 9;       // Buzzer

// variables will change:
int crossingbutton = 0;        // Is crossing button pressed. 1 = Yes
int crossingState = 0;         // variable for reading the pushbutton status

void setup() {
  //Set pinouts and initial values
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(crossred, OUTPUT);
  pinMode(crossgreen, OUTPUT);
  pinMode(buttonlight, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(13, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(crossing, INPUT);
  digitalWrite(green, HIGH);
  digitalWrite(crossred, HIGH);
  
  //Start Serial
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Traffic Lights V1");
  Serial.println("Written By Stephen McBain");
  
    // Initialize the rtc object
  rtc.begin();
  
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(16, 57, 30);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(5, 2, 2018);   // Set the date to January 1st, 2014

  //SetupSD Card
  Serial.print("\nInitializing SD card...");
  //Change 10 below to Set CS Pin
  if (!SD.begin(10)) {
    Serial.println("SD inisalisation Failed");
    return;
  } else {
    dataFile = SD.open("startup.txt", FILE_WRITE);
    dataFile.print("Startup logged.....");
    dataFile.print(rtc.getDateStr());
    dataFile.print(", ");
    dataFile.println(rtc.getTimeStr());
    dataFile.close();
    SD.remove("traflog.txt");
    Serial.println("SD Ready");
    dataFile = SD.open("traflog.txt", FILE_WRITE);
    dataFile.println("Traffic Lights V1");
    dataFile.println("Written By Stephen McBain");
    dataFile.println();
    dataFile.println("Data");
    dataFile.println();
    dataFile.close();
  }
 }


void loop() {
  // read the state of the pushbutton value:
  int  crossingbutton = digitalRead(crossing);
    if (crossingbutton == HIGH) {
      dataFile = SD.open("traflog.txt", FILE_WRITE);
      int crossingState = 1;
      Serial.print("Crossing button Pressed");
      Serial.print(", ");
      Serial.println(rtc.getTimeStr());
      dataFile.print("Crossing button Pressed");
      dataFile.print(", ");
      dataFile.println(rtc.getTimeStr());
      digitalWrite(buttonlight, HIGH);
      delay(5000);
        digitalWrite(green, LOW);
        digitalWrite(yellow, HIGH);
        delay(1000);
        digitalWrite(yellow, LOW);
        digitalWrite(red, HIGH);
        delay(1000);
        digitalWrite(crossred, LOW);
        digitalWrite(crossgreen, HIGH);
        tone(buzzer, 1000);
        digitalWrite(buttonlight,LOW);
        Serial.print("Cross Now");
        Serial.print(", ");
        Serial.println(rtc.getTimeStr());
        dataFile.print("Cross Now");
        dataFile.print(", ");
        dataFile.println(rtc.getTimeStr());
        delay(10000);
        Serial.print("Stop Crossing");
        Serial.print(", ");
        Serial.println(rtc.getTimeStr());
        dataFile.print("Stop Crossing");
        dataFile.print(", ");
        dataFile.println(rtc.getTimeStr());
        noTone(buzzer);
        digitalWrite(crossgreen, LOW);
        digitalWrite(crossred, HIGH);
        delay(1000);
        digitalWrite(yellow, HIGH);
        delay(1000);
        digitalWrite(red, LOW);
        digitalWrite(yellow, LOW);
        digitalWrite(green, HIGH);
        Serial.print("Sequence Complete");
        Serial.print(", ");
        Serial.println(rtc.getTimeStr());
        dataFile.print("Sequence Complete");
        dataFile.print(", ");
        dataFile.println(rtc.getTimeStr());
        dataFile.close();
        delay(2000);
        int crossingstate = 0;
      } else {
      delay(2);
    } 
}
