2025/07/19 不具合を修正しました
2025/08/14 get_angle関数の戻り値をint64_t(long int)に変更しました

# RotaryEncoder  
インクリメンタルエンコーダー専用ライブラリです
STM32_encoderとInterrupt_encoderの２つのクラスを用意しています  

**STM32_encoder**  
STM32マイコン標準搭載のエンコーダーモードを使用したモードです 内部でタイマーを分けて使用するため、タイマーの管理に注意が必要です  
精度はこちらの方がいいです  

サンプルコード  
``` cpp
#include "RotaryEncoder.h"
#include "mbed.h"

BufferedSerial pc(USBRX, USBTX, 9600);
STM32_encoder encoder1(PA_6, PA_7);

int64_t encoder1_angle; // get_angle()の返り値はint64_t型

int main(){
    encoder1.start(); // エンコーダーモードに設定
    encoder1.reset(); // データの初期化
    while(true){
        encoder1_angle = encoder1.get_angle();
        printf("angle1:%d\r\n", encoder1_angle);
        ThisThread::sleep_for(5ms);
    }
}
```

**Interruot_encoder**  
マイコンのGPIOピンに対しての割り込みを使用したモードです もっとも一般的で扱いやすいです  

サンプルコード  
```cpp
#include "mbed.h"
#include "RotaryEncoder.h"

BufferedSerial pc(USBRX, USBTX, 9600);
Interrupt_encoder encoder1(PA_6, PA_7);

int64_t encoder1_angle, encoder2_angle;

int main(){
     encoder1.start();
     encoder1.reset();
     while(true){
         encoder1_angle = encoder1.get_angle();
         printf("angle1:%d\r\n", encoder1_angle);
         ThisThread::sleep_for(5ms);
     }
}
```
