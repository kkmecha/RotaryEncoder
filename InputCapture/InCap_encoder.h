#ifndef INCAP_ENCODER_H
#define INCAP_ENCODER_H

#include "mbed.h"

class InCap_encoder {
public:
    InCap_encoder(PinName pinA, PinName pinB, float ppr);
    float get_rpm();

private:
    InterruptIn _phaseA;
    InterruptIn _phaseB;
    Timer _timer;
    float _ppr;
    
    volatile uint64_t _last_pulse_time;
    volatile uint64_t _pulse_interval;
    volatile int _direction;
    void encode_isr();
};

#endif