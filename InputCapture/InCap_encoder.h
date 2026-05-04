#include "mbed.h"
#include "stm32f3xx_hal.h" 
#include <chrono>

using namespace std::chrono;

class InCap_encoder {
public:
    struct MotionState {
        int64_t angle;
        float rpm;
        float rpmps;
    };

    InCap_encoder(PinName pinA, PinName pinB, int ppr = 200, int resolution = 4) 
        : _pinA(pinA), _pinB(pinB), _ppr(ppr), _resolution(resolution),_last_angle(0), _last_rpm(0) {
        _state = {0, 0.0f, 0.0f};
    }

    void init();
    void update();
    int64_t get_angle();
    float get_rpm();
    float get_rpmps();
    void reset_angle();
    void reset_rpm();
    void reset_rpmps();
    void reset_all();
private:
    PinName _pinA, _pinB;
    int _ppr ,_resolution;
    TIM_HandleTypeDef _htim;
    Timer _timer;
    MotionState _state;
    int32_t _last_angle;
    float _last_rpm;
    microseconds _last_time;

    void enable_gpio_clock(PinName pin);

    void setup_gpio_pin(PinName pin, uint32_t af);
};