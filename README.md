2025/07/19 不具合を修正しました


クラスを2つ用意しています  


```python
# サンプルPythonコード
def hello_world():
    print("Hello, GitHub!")

hello_world()

  
**class STM32_encoder**  
 STM32の機能であるエンコーダー機能を使ったロータリーエンコーダーの処理  
 使える内部タイマーの数が限られ、マイコンによっても数が違うので用途は限定的になるかも  
  
**class Interrupt_encoder**  
 GPIOピンの割り込みを使用したロータリーエンコーダーの処理  
 一般的なのはこっち

