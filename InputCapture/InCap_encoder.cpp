#include "InCap_encoder.h"

InCap_encoder::InCap_encoder(PinName pinA, PinName pinB, float ppr) 
    : _phaseA(pinA), _phaseB(pinB), _ppr(ppr * 4.0f) {
    
    _phaseA.rise(callback(this, &InCap_encoder::encode_isr));
    _phaseA.fall(callback(this, &InCap_encoder::encode_isr));
    _phaseB.rise(callback(this, &InCap_encoder::encode_isr));
    _phaseB.fall(callback(this, &InCap_encoder::encode_isr));

    _timer.start();
    _last_pulse_time = 0;
    _pulse_interval = 0;
    _direction = 1;
}

float InCap_encoder::get_rpm() {
    uint64_t now = _timer.read_high_resolution_us();

    if (now - _last_pulse_time > 100000) {
        return 0.0f;
    }

    if (_pulse_interval == 0) return 0.0f;

    float rpm = (1000000.0f / (float)_pulse_interval) * 60.0f / _ppr;
    
    return rpm * _direction;
}

void InCap_encoder::encode_isr() {
    uint64_t now = _timer.read_high_resolution_us();
    
    if (_last_pulse_time != 0) {
        _pulse_interval = now - _last_pulse_time;
    }
    _last_pulse_time = now;

    static int lastA = 0;
    static int lastB = 0;
    int curA = _phaseA.read();
    int curB = _phaseB.read();

    if (curA != lastA) {
        _direction = (curA ^ lastB) ? 1 : -1;
    } else {
        _direction = (curB ^ curA) ? 1 : -1;
    }

    lastA = curA;
    lastB = curB;
}
