#include<HardwareBLESerial.h>
#include <MQ135.h>
#include <Arduino_HS300x.h>

int redpin = 9; // select the pin for the red LED
int bluepin = 11; // select the pin for the blue LED
int greenpin = 10 ;// select the pin for the green LED

const int ANALOGPIN=0;
MQ135 gasSensor = MQ135(ANALOGPIN);

HardwareBLESerial &bleSerial = HardwareBLESerial::getInstance();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);

  if (!HS300x.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }

  pinMode (redpin, OUTPUT);
  pinMode (bluepin, OUTPUT);
  pinMode (greenpin, OUTPUT);
  pinMode(2, INPUT);

  Wire.begin();
  //setup the BLESerial
  while(!bleSerial.beginAndSetupBLE("IE5995-Daohua"));
  pinMode(3, OUTPUT);
}

void setRGB(int red, int green, int blue)
 {
    analogWrite(redpin, red);
  	analogWrite(greenpin, green);
  	analogWrite(bluepin, blue);
 }

void loop() {
  // Read the pressure and temperature values
  float humdity = HS300x.readHumidity();
  float temperature = HS300x.readTemperature();
  float VOC = gasSensor.getPPM();
  Serial.print("Humidity = ");
  Serial.print(humdity);
  Serial.println(" %");
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" C");
  Serial.print("VOC = ");
  Serial.print(VOC);
  Serial.println(" ppm");

  //BLESerial print the values on the cellphone
  bleSerial.poll();
  bleSerial.print("Hum: ");
  bleSerial.print((int64_t)humdity);
  bleSerial.println(" %");
  bleSerial.print("Temp: ");
  bleSerial.print((int64_t)temperature);
  bleSerial.println(" C");
  bleSerial.print("VOC = ");
  bleSerial.print((int64_t)VOC);
  bleSerial.println(" ppm");

  int x = VOC/(3000/255); //normolize the VOC value to RGB range
  setRGB(x, 255-x, 0); //set the LED light color changes with VOC
  delay(1000); // wait 1000ms for next reading
}
