#include "DHT.h"
#include "secrets.h"
#include <WiFiNINA.h>
#include <ThingSpeak.h>


#define DHTPIN 2
#define DHTTYPE DHT22

char ssid[] = SECRET_SSI;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password

// this is the unique channel number on ThingSpeak where we will send our data
unsigned long myChannelNumber = 2626655;
// this WriteAPIKey helps to send data to our ThingSpeak channel
const char* myWriteAPIKey = "G59FN1R8LHBG4JGV";


DHT dht(DHTPIN, DHTTYPE);
// connect to Wi-Fi
WiFiClient client;

void setup() {
  Serial.begin(9600);
  dht.begin();
  // connecting to our Wi-Fi network
  connectWiFi();
  // here a connection is setting up to ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // next measurement will be taken after 16 seconds
  delay(16000);

  // it helps to read humidity data from sensor
  float humidity = dht.readHumidity();
  //it helps to read temperature data from sensor
  float temperature = dht.readTemperature();

  // here a "if condition" is applied, so that we can check that the reading taken from sensor is a number or not.     ^^^^^^^^^(isnan = Is Not A Number)^^^^^^^^^
  if (isnan(humidity) || isnan(temperature)) {
    // if the readings are not valid then this message will be printed
    Serial.println("Error: Could not read from DHT sensor!");
    // from here the loop will start again
    return;
  }

  //  here we are printing the readings of humidity and temperature on the serial monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  ThingSpeak.setField(1, temperature);  // this will set the feild 1 as temperature on the ThingSpeak
  ThingSpeak.setField(2, humidity);     // this will set the feild 2 as humidity on the ThingSpeak

  // here we are sending our data to ThingSpeak and checking if it worked
  int responseCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  // response code is a code which tells us if the data we sent to ThingSpeak is  received successfully or not. A code of "200" means the date is sent and any other number means data is n.ot sent and there is a problem.
  if (responseCode == 200) {
    // if successful then this message will be printed
    Serial.println("Data sent successfully to ThingSpeak.");
  } else {
    // is there is any problem then this message will be printed
    Serial.print("Failed to send data to ThingSpeak. Response code: ");
    Serial.println(responseCode);
  }
}

void connectWiFi() {
  Serial.print("Connecting to Wi-Fi...");
  // it will keep trying to connect until the wi-fi will connect
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // dot will be printed
    Serial.print(".");
    delay(1000);
  }
  // when connected this message will be printed on the serial monitor
  Serial.println("Wi-Fi connected.");
}
