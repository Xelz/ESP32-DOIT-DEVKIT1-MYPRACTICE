#include "PWM.h"

PWM::PWM(uint8_t pin, uint32_t frequency, uint8_t channel, uint8_t resolution)
    : pin(pin), frequency(frequency), channel(channel), resolution(resolution), dutyCycle(0) {}

void PWM::init() {
    ledcSetup(channel, frequency, resolution);
    ledcAttachPin(pin, channel);
    ledcWrite(channel, dutyCycle);
}

void PWM::updateDuty(uint32_t duty) {
    uint32_t maxDuty = (1 << resolution) - 1;
    dutyCycle = (duty > maxDuty) ? maxDuty : duty;
    ledcWrite(channel, dutyCycle);
}

void PWM::updateFrequency(uint32_t freq) {
    frequency = freq;
    ledcSetup(channel, frequency, resolution);
    ledcWrite(channel, dutyCycle);
}

void PWM::start() {
    ledcWrite(channel, dutyCycle);
}

void PWM::stop() {
    ledcWrite(channel, 0);
}
