#include "InCap_encoder.h"

void InCap_encoder::init() {
    uint32_t af_setting = 0;

    if (_pinA == PA_0 && _pinB == PA_1) {
        _htim.Instance = TIM2;
        af_setting = GPIO_AF1_TIM2;
        __HAL_RCC_TIM2_CLK_ENABLE();
    } else if (_pinA == PA_6 && _pinB == PA_7) {
        _htim.Instance = TIM3;
        af_setting = GPIO_AF2_TIM3;
        __HAL_RCC_TIM3_CLK_ENABLE();
    }//  else if (_pinA == PB_6 && _pinB == PB_7) {
    //     _htim.Instance = TIM4;
    //     af_setting = GPIO_AF2_TIM4;
    //     __HAL_RCC_TIM4_CLK_ENABLE();
    // }

    enable_gpio_clock(_pinA);
    setup_gpio_pin(_pinA, af_setting);
    setup_gpio_pin(_pinB, af_setting);

    _htim.Init.Prescaler = 0;
    _htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    _htim.Init.Period = 0xFFFFFFFF; 
    // if (_htim.Instance != TIM2 && _htim.Instance != TIM5) {
    //     _htim.Init.Period = 0xFFFF;
    // }
    _htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    TIM_Encoder_InitTypeDef sConfig = {0};
    sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = 10;
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Filter = 10;

    HAL_TIM_Encoder_Init(&_htim, &sConfig);
    HAL_TIM_Encoder_Start(&_htim, TIM_CHANNEL_ALL);
    
    _timer.start();
    _last_time = _timer.elapsed_time();
}

void InCap_encoder::update() {
    microseconds now = _timer.elapsed_time();
    
    float dt = duration<float>(now - _last_time).count();
    
    if (dt <= 0.0f) return;

    int32_t current_count = (int32_t)__HAL_TIM_GET_COUNTER(&_htim);
    int32_t delta_count = current_count - _last_angle;
    
    if (_htim.Init.Period == 0xFFFF) {
        if (delta_count > 32767) delta_count -= 65536;
        else if (delta_count < -32768) delta_count += 65536;
    }

    float speed_pps = (float)delta_count / dt;
    float current_rpm = (speed_pps * 60.0f) / (_ppr * 4.0f);
    float current_rpmps = (current_rpm - _last_rpm) / dt;

    _state.angle += delta_count;
    _state.rpm = current_rpm;
    _state.rpmps = current_rpmps;

    _last_angle = current_count;
    _last_rpm = current_rpm;
    _last_time = now;
}

int64_t InCap_encoder::get_angle(){
    return _state.angle;
}

float InCap_encoder::get_rpm(){
    return _state.rpm;
}

float InCap_encoder::get_rpmps(){
    return _state.rpmps;
}

void InCap_encoder::reset_angle(){
    _state.angle = 0;
}

void InCap_encoder::reset_rpm(){
    _state.rpm = 0.0;
}

void InCap_encoder::reset_rpmps(){
    _state.rpmps = 0.0;
}

void InCap_encoder::reset_all(){
    _state.angle = 0;
    _state.rpm = 0.0;
    _state.rpmps = 0.0;
}

void InCap_encoder::enable_gpio_clock(PinName pin) {
    if ((pin >> 4) == 0) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if ((pin >> 4) == 1) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if ((pin >> 4) == 2) __HAL_RCC_GPIOC_CLK_ENABLE();
}

void InCap_encoder::setup_gpio_pin(PinName pin, uint32_t af) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = (1 << (pin & 0x0F));
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = af;
    
    GPIO_TypeDef* port;
    switch (pin >> 4) {
        case 0: port = GPIOA; break;
        case 1: port = GPIOB; break;
        case 2: port = GPIOC; break;
        default: return;
    }
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}
