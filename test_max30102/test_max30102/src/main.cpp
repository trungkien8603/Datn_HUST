#include <Wire.h>                 // I2C Library
#include "MAX30105.h"             // SparkFun MAX3010x library
#include "spo2_algorithm.h"       // SpO2 algorithm from SparkFun
#include "heartRate.h"
           // Heart rate algorithm from SparkFun'
MAX30105 particleSensor;

const byte RATE_SIZE = 4;         // Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE];            // Array of heart rates
byte rateSpot = 0;
long lastBeat = 0;                // Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;

int32_t spo2;                     // SpO2 value
int8_t validSPO2;                 // SpO2 valid flag
int32_t heartRate;                // Heart rate value
int8_t validHeartRate;            // Heart rate valid flag

void setup() {
  Wire.begin();                                       // Initialize I2C communication
  Wire.setClock(400000);                              // Set I2C speed to 400kHz

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {  // Initialize MAX30105
    Serial.println("MAX30105 was not found. Please check wiring/power.");
    while (1);
  }

  particleSensor.setup();                             // Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A);          // Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeIR(0x0A);           // Set IR LED to measure SpO2

  // Initialize serial communication
  Serial.begin(9600);                                 // Set the baud rate to your desired value
}

void loop() {
  long irValue = particleSensor.getIR();              // Reading the IR value
  long redValue = particleSensor.getRed();            // Reading the RED value for SpO2

  if (irValue > 50000)  // Finger is detected
  {
    if (checkForBeat(irValue) == true)                // If a heart beat is detected
    {
      long delta = millis() - lastBeat;               // Measure duration between two beats
      lastBeat = millis();
      beatsPerMinute = 60 / (delta / 1000.0);         // Calculating the BPM

      if (beatsPerMinute < 255 && beatsPerMinute > 20) // To calculate the average, store some values then average them
      {
        rates[rateSpot++] = (byte)beatsPerMinute;     // Store this reading in the array
        rateSpot %= RATE_SIZE;                        // Wrap variable

        beatAvg = 0;
        for (byte x = 0; x < RATE_SIZE; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;

        Serial.print("BPM: ");
        Serial.println(beatAvg);
      }

      // Calculate SpO2
      uint32_t irValueForSpO2 = particleSensor.getIR();
      uint32_t redValueForSpO2 = particleSensor.getRed();
      maxim_heart_rate_and_oxygen_saturation((int32_t*)&irValueForSpO2, (int32_t*)&redValueForSpO2, &spo2, &validSPO2, &heartRate, &validHeartRate);
      if (validSPO2 && validHeartRate)                // Check if valid readings obtained
      {
        Serial.print("SpO2: ");
        Serial.println(spo2);
      }
    }
  }
  else
  {
    Serial.println("No finger detected");
  }
}