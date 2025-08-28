#ifndef DHT22_H
#define DHT22_H

#include <Arduino.h>

class DHT22 {
    private:
        uint8_t data[5];
        uint8_t pin;
        float humidity;
        float temperature;
    public:
        DHT22(uint8_t pin);

        bool readDHT22();
        float getHumidity() const;
        float getTemperature() const;
};

#endif // DHT22_H
