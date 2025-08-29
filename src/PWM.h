#ifndef PWM_H
#define PWM_H

#include <Arduino.h>

class PWM {
    private:
        uint8_t pin;
        uint32_t dutyCycle;
        uint32_t frequency;
        uint8_t resolution;
        uint8_t channel;
    public:
        PWM(uint8_t pin, uint32_t frequency, uint8_t channel, uint8_t resolution);
        
        void init();
        void updateDuty(uint32_t duty);
        void updateFrequency(uint32_t frequency);

        void start();
        void stop();
};

#endif