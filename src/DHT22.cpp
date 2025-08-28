#include "DHT22.h"

/**
 * Constructor for DHT22 sensor
 * @param pin: GPIO pin connected to DHT22 data line
 * Initializes humidity and temperature to 0, clears data buffer.
 */
DHT22::DHT22(uint8_t pin) : pin(pin), humidity(0.0), temperature(0.0) {
    memset(data, 0, sizeof(data));
}

/**
 * readDHT22
 * Reads 40-bit data from DHT22 sensor and updates internal temperature and humidity.
 * @return true if reading was successful, false if checksum fails or no response from sensor
 *
 * Steps:
 * 1. Send start signal from MCU to sensor
 * 2. Wait for sensor response (80µs LOW, 80µs HIGH)
 * 3. Read 40 bits (humidity MSB→LSB, temperature MSB→LSB, checksum)
 * 4. Verify checksum and update humidity/temperature
 */
bool DHT22::readDHT22() {
    memset(data, 0, sizeof(data));

    // Step 1: MCU sends start signal
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(1);   // 18 ms LOW
    digitalWrite(pin, HIGH);
    delayMicroseconds(20);
    pinMode(pin, INPUT);

    // Step 2: Wait for sensor response
    unsigned long t = micros();
    while (digitalRead(pin) == HIGH) {
        if (micros() - t > 90) {
            Serial.println("No response LOW");
            return false;
        }
    }
    while (digitalRead(pin) == LOW);   // 80 µs LOW
    while (digitalRead(pin) == HIGH);  // 80 µs HIGH

    // Step 3: Read 40 bits (HUMID_MSB[8] ---> HUMID_LSB[8] ---> TEMP_MSB[8] ---> TEMP_LSB[8] ---> CHK_SUM[8])
    for (int i = 0; i < 40; i++) {
        while (digitalRead(pin) == LOW);  // wait for start of bit

        unsigned long start = micros();
        while (digitalRead(pin) == HIGH);
        unsigned long duration = micros() - start;

        int byteIndex = i / 8;
        data[byteIndex] <<= 1;  // Shift Left 1 step
        if (duration > 28) { // >28 µs = logic 1
            data[byteIndex] |= 1;
        }
    }

    // Step 4: Checksum Verification
    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) != data[4]) {
        Serial.println("Checksum error");
        return false;
    }

    // Update internal humidity and temperature variables
    humidity = float(data[0] << 8 | data[1]) * 0.10;
    temperature = float(data[2] << 8 | data[3]) * 0.10;

    return true;
}


/** Returns last read humidity value */
float DHT22::getHumidity() const{
    return humidity;
}

/** Returns last read temperature value */
float DHT22::getTemperature() const{
    return temperature;
}