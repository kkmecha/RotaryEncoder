2025/07/19 不具合を修正しました

# RotaryEncoder  
インクリメンタルエンコーダー専用ライブラリです
STM32_encoderとInterrupt_encoderの２つのクラスを用意しています  

** STM32_encoder **  
STM32マイコン標準搭載のエンコーダーモードを使用したモードです 内部でタイマーを分けて使用するため、タイマーの管理に注意が必要です  
精度はこちらの方がいいです  

サンプルコード  
```markdoown
``` cpp
#include "RotaryEncoder.h"
#include "mbed.h"

BufferedSerial pc(USBRX, USBTX, 9600);
STM32_encoder encoder1(PA_6, PA_7);
STM32_encoder encoder2(PB_6, PB_7);

int32_t encoder1_angle, encoder2_angle; // get_count()の返り値はint32_t型

int main(){
    encoder1.start(); // エンコーダーモードに設定
    encoder2.start();
    encoder1.reset(); // データの初期化
    encoder2.reset();
    while(true){
    	  encoder1_angle = encoder1.get_count();
        encoder2_angle = encoder2.get_count();
        printf("angle1:%d, angle2:%d\r\n", encoder1_angle, encoder2_angle);
        ThisThread::sleep_for(5ms);
    }
}
```

** Interruot_encoder **  
マイコンのGPIOピンに対しての割り込みを使用したモードです もっとも一般的で扱いやすいです  

サンプルコード  
```markdown
```cpp
#include "mbed.h"
#include "RotaryEncoder.h"

BufferedSerial pc(USBRX, USBTX, 9600);
Interrupt_encoder encoder1(PA_6, PA_7);
Interrupt_encoder encoder2(PB_6, PB_7);

int encoder1_angle, encoder2_angle;

int main(){
	   while(true){
		     encoder1_angle = encoder1.get_count();
		     encoder2_angle = encoder2.get_count();
         printf("angle1:%d, angle2:%d\r\n", encoder1_angle, encoder2_angle);
         ThisThread::sleep_for(5ms);
	    }
}
```
