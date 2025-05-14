#include <Servo.h>


Servo myServo;


const int sensorPin = A0;
const int averageSampleSize = 10;
int sensorReadings[averageSampleSize];
int index = 0;
long total = 0;


long threshold = 0; // Will be set automatically
const int hysteresis = 5;  // Reduced hysteresis for sensitivity
const int thresholdOffset = 15; // Lower offset for more realistic triggering


bool isActivated = false;
int lastServoPosition = 0;
unsigned long lastToggleTime = 0;
const unsigned long debounceTime = 500; // Prevent rapid toggling (500ms)


void setup() {
  myServo.attach(9);
  Serial.begin(9600);
  myServo.write(0);
  delay(1000); // Give time for serial monitor to open
 
  // Initialize readings array
  for (int i = 0; i < averageSampleSize; i++) {
    sensorReadings[i] = 0;
  }


  // Pre-fill the readings buffer with actual values
  Serial.println("Pre-filling readings buffer...");
  for (int i = 0; i < averageSampleSize; i++) {
    int val = analogRead(sensorPin);
    sensorReadings[i] = val;
    total += val;
    delay(20);
  }
 
  // Calibration mode
  Serial.println("CALIBRATION: Relax your muscle for 5 seconds...");
  long sum = 0;
  int maxVal = 0;
  int minVal = 1023;
  const int calibrationSamples = 100;
 
  // Collect samples
  for (int i = 0; i < calibrationSamples; i++) {
    int val = analogRead(sensorPin);
    sum += val;
    maxVal = max(maxVal, val);
    minVal = min(minVal, val);
   
    // Print every 10th reading for monitoring
    if (i % 10 == 0) {
      Serial.print("Sample ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(val);
    }
    delay(50);
  }
 
  long baseline = sum / calibrationSamples;
  int variance = maxVal - minVal;
 
  // Set threshold based on baseline and variance
  threshold = baseline + thresholdOffset;
 
  Serial.println("\n--- Calibration Results ---");
  Serial.print("Min value: ");
  Serial.println(minVal);
  Serial.print("Max value: ");
  Serial.println(maxVal);
  Serial.print("Baseline: ");
  Serial.println(baseline);
  Serial.print("Variance: ");
  Serial.println(variance);
  Serial.print("Threshold set to: ");
  Serial.println(threshold);
  Serial.println("-------------------------");
  Serial.println("Now flex your muscle to test!");
}


void loop() {
  // Read new value
  int newValue = analogRead(sensorPin);


  // Update moving average
  total -= sensorReadings[index];
  sensorReadings[index] = newValue;
  total += sensorReadings[index];
  index = (index + 1) % averageSampleSize;


  int avgValue = total / averageSampleSize;


  // Print raw and average values for debugging
  Serial.print("Raw: ");
  Serial.print(newValue);
  Serial.print(" | Avg: ");
  Serial.print(avgValue);
  Serial.print(" | Threshold: ");
  Serial.print(threshold);
 
  // Check if sensor is activated
  unsigned long currentTime = millis();
  if (avgValue > threshold && !isActivated && (currentTime - lastToggleTime > debounceTime)) {
    toggleServo();
    isActivated = true;
    lastToggleTime = currentTime;
    Serial.print(" | ACTIVATED!");
  }
 
  if (avgValue < (threshold - hysteresis)) {
    isActivated = false;
  }
 
  Serial.println();
  delay(50);
}


void toggleServo() {
  int target = (lastServoPosition == 0) ? 180 : 0;
 
  // Move servo smoothly
  int step = (target > lastServoPosition) ? 5 : -5;
  for (int pos = lastServoPosition; pos != target; pos += step) {
    myServo.write(pos);
    delay(15);
  }
 
  myServo.write(target);
  lastServoPosition = target;
 
  Serial.print("Servo toggled to: ");
  Serial.println(target);
}

