const int plantPin = A0;      // Analog pin for the plant signal
const int buzzerPin = 9;      // Pin for the piezo buzzer
float samplingInterval = 0.0001; // Sampling interval in seconds (100 µs)
int threshold = 512;          // Threshold for zero crossing
unsigned long startTime;
int zeroCrossings = 0;        // Counter for zero crossings
int previousValue = 0;

// Voltage Conversion
const float referenceVoltage = 5.0; // Reference voltage (5V for Arduino Uno)
float voltage = 0.0;                // Variable to store voltage in volts
float voltage_mV = 0.0;             // Variable to store voltage in millivolts

void setup() {
  Serial.begin(9600);       // Initialize serial communication
  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as output
}

void loop() {
  zeroCrossings = 0;        // Reset zero crossing count
  startTime = millis();     // Start time for measurement

  while (millis() - startTime < 1000) {  // Measure for 1 second
    int rawValue = analogRead(plantPin);  // Read the analog value

    // Convert raw ADC value to millivolts
    voltage = (rawValue / 1023.0) * referenceVoltage; // Voltage in volts
    voltage_mV = voltage * 1000;                     // Convert to millivolts

    // Detect zero crossing
    if ((rawValue > threshold && previousValue <= threshold) ||
        (rawValue < threshold && previousValue >= threshold)) {
      zeroCrossings++;
    }

    previousValue = rawValue;  // Update the previous value
    // delayMicroseconds(100);    // Adjust the sampling interval (100 µs)
  }

  // Calculate frequency
  float frequency_Hz = zeroCrossings / 2.0;  // Divide by 2 (two crossings per cycle)

  // Map voltage to pitch (frequency in Hz)
  int noteFrequency = map(voltage_mV, 0, 5000, 200, 2000); // Map voltage to 200-2000 Hz range

  // Map frequency to duration (rhythm in ms)
  int noteDuration = map(frequency_Hz, 0, 10, 500, 100); // Map frequency to 500-100 ms

  // Debugging: Print values
  Serial.print("Voltage: ");
  Serial.print(voltage_mV);
  Serial.print(" mV, Frequency: ");
  Serial.print(frequency_Hz);
  Serial.print(" Hz, Note Frequency: ");
  Serial.print(noteFrequency);
  Serial.print(" Hz, Note Duration: ");
  Serial.print(noteDuration);
  Serial.println(" ms");

  // Play the note
  tone(buzzerPin, noteFrequency, noteDuration);

  // Wait for the note to finish
  // delay(noteDuration);
}
