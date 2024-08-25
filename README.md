# Multithread GPIO control on Raspberry Pi
## A. Goal
- 有二個 Threads，以下命名為 Thread-A 及 Thread-B，各自控制 LED-A 及 LED-B
- Thread-A 取 1~100 亂數，若亂數為 7 的倍數，且 Thread-B 之 LED 沒亮燈時，LED-A 亮一秒後滅掉
- Thread-B 取 1~100 亂數，若亂數為 9 的倍數，且 Thread-A 之 LED 沒亮燈時，LED-B 亮一秒後滅掉
- 上述動作，透過鍵盤輸入控制threads停止與再執行。
## B. Demo
[Demo video](https://youtu.be/pDDI-95khnU)
## C. Implement
### 1) Pi version
- Raspberry Pi 3 model B+
- SoC: BCM2837
- OS: Raspbian
### 2) GPIO setup
- LED 1: GPIO4 | wPi 7 | PHY pin 7
- LED 2: GPIO17 | wPi 0 | PHY pin 11

### 3) C++實作
- Idea
    - 採用Peterson's Algorithm的方式，將亂數生成和控制LED IO的地方lock住。
    - 當收到wait signal，led thread會busy waiting，待signal收到continue恢復運行。收到stop signal結束thread。
    - thread之間透過交換lock的方式，輪流產生亂數執行LED task
- Library
    - pthread
    - wiringPi
- Compile and run code
    - compile command
        > g++ -o test led.cpp -lwiringPi -lpthread
    - Run
        > ./test
- Keyboard command
  - 輸入1: Make threads wait
  - 輸入2: Threads continue
  - 輸入3: Stop and kill threads, finish process
### 4) Python 實作
- Idea
    - 跟C++版本實做的差別，在於wait狀態，這邊是將thread kill掉，回復時重新產生新thread跑task。
    - 透過GIL鎖住亂數生成與LED IO
- Run code
    - Run
        > python3 led.py
- Command
  - 輸入s: Stop and kill threads, return to main thread
  - 輸入c: Return to run() and re-create threads
