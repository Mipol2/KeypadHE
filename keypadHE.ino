#include <Keyboard.h>

int hallSensorPin1 = A2; // The analog pin where the first Hall effect sensor is connected
int hallSensorPin2 = A3; // The analog pin where the second Hall effect sensor is connected
int hallSensorValue1 = 0; // Variable to store the first sensor reading
int hallSensorValue2 = 0; // Variable to store the second sensor reading
int mappedValue1 = 0; // Variable to store the mapped first sensor reading
int mappedValue2 = 0; // Variable to store the mapped second sensor reading
int threshold1 = 88; // Customize this value for your first actuation threshold (middle of the mapped range)
int threshold2 = 88; // Customize this value for your second actuation threshold (middle of the mapped range)
bool keyPressed1 = false; // Track whether the first key is currently pressed
bool keyPressed2 = false; // Track whether the second key is currently pressed

int minInput1 = 552; // Minimum expected value for the first sensor (considering some margin)
int maxInput1 = 840; // Maximum expected value for the first sensor (considering some margin)
int minInput2 = 552; // Minimum expected value for the second sensor (considering some margin)
int maxInput2 = 840; // Maximum expected value for the second sensor (considering some margin)

// Rapid trigger variables
unsigned long currentTime = 0;
unsigned long prevTime1 = 0;
unsigned long prevTime2 = 0;
unsigned long debounceTime = 30; // Debounce time

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 bits per second
  Keyboard.begin(); // Initialize the Keyboard library
}

void loop() {
  hallSensorValue1 = analogRead(hallSensorPin1);
  hallSensorValue2 = analogRead(hallSensorPin2); 

  // Map the values
  mappedValue1 = map(hallSensorValue1, minInput1, maxInput1, 0, 256);
  mappedValue2 = map(hallSensorValue2, minInput2, maxInput2, 0, 256);

  Serial.print("Original Value 1: ");
  Serial.print(hallSensorValue1);
  Serial.print("\tMapped Value 1: ");
  Serial.println(mappedValue1);
  Serial.print("Original Value 2: ");
  Serial.print(hallSensorValue2);
  Serial.print("\tMapped Value 2: ");
  Serial.println(mappedValue2);

  // Get the current time
  currentTime = millis();

  // Check if the first mapped value exceeds the threshold and debounce time has passed
  if (mappedValue1 > threshold1) {
    if (!keyPressed1) {
      Keyboard.press('.');
      keyPressed1 = true;
      prevTime1 = currentTime; // Update the last debounce time
    }
  } else {
    if (keyPressed1 && (currentTime - prevTime1 > debounceTime)) {
      Keyboard.release('.');
      keyPressed1 = false;
      prevTime1 = currentTime; // Update the last debounce time
    }
  }

  // Check if the second mapped value exceeds the threshold and debounce time has passed
  if (mappedValue2 > threshold2) {
    if (!keyPressed2) {
      Keyboard.press('/');
      keyPressed2 = true;
      prevTime2 = currentTime; // Update the last debounce time
    }
  } else {
    if (keyPressed2 && (currentTime - prevTime2 > debounceTime)) {
      Keyboard.release('/');
      keyPressed2 = false;
      prevTime2 = currentTime; // Update the last debounce time
    }
  }
}

void customizeThreshold(int newThreshold1, int newThreshold2) {
  threshold1 = newThreshold1; // Update the threshold value for the first key
  threshold2 = newThreshold2; // Update the threshold value for the second key
}
