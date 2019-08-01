/* DHT library

  MIT license
  written by Adafruit Industries
*/
#ifndef DHT_H
#define DHT_H

/*
  -----------  Hàm arduino > 100 là sao ? --------------
*/

//***
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


// Comment để cho phép in ra các thông báo gỡ lỗi tiệp
//#define DHT_DEBUG

// Xác định các lỗi sẽ được print ra !
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef DHT_DEBUG
#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
#define DEBUG_PRINT(...) {}
#define DEBUG_PRINTLN(...) {}
#endif

// Define types of sensors.
#define DHT11 11
#define DHT22 22
#define DHT21 21
#define AM2301 21


class Vietduino_DHT {
  public:
    ***   Vietduino_DHT(uint8_t pin, uint8_t type, uint8_t count = 6);    // Khai báo pin , kiểu dữ liệu , biến count ???
    void begin(void);                                        // Bắt đầu read DHT
    float readTemperature(bool S = false, bool force = false); // Read Temp , --> S=true --> C , S=flase --> F
    /*
       Convert C -> F : F = C*1.8 - 32;
       Convert F -> C : C = (F-32)/1.8;
    */
    float convertCtoF(float);
    float convertFtoC(float);
    /*
       Tính toán chỉ số nhiệt độ ? Cái này lấy dựa theo trang https://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml
       Đọc độ C nếu isFahrenheit = false
       Mặc định là đọc độ F isFahrenheit = true
    */
    float computeHeatIndex(float temperature, float percentHumidity, bool isFahrenheit = true);
    float readHumidity(bool force = false); // Read Humi
    //***
    boolean read(bool force = false); // ????


    //**********************
  private:
    uint8_t data[5]; // --> 0 - 255;
    uint8_t _pin, _type;
#ifdef __AVR
    /*
        Truy cập GPIO trực tiếp trên AVR 8 bit để theo dõi cổng và bitmask;
        Sử dụng chân digital để kết nối với DHT . Các nền tảng khác sử dụng digitalRead
    */
    uint8_t _bit, _port;
#endif
    uint32_t _lastreadtime, _maxcycles;
    bool _lastresult;
    uint32_t expectPulse(bool level);
};
//*********************

/*********************
  Cái này hiểu tạm InterupLock lock
  khi khai báo là lock thì nó sẽ tự chay noInterrup();
  còn phủ định ~lock thì nó sẽ chạy interups;
*/
class InterruptLock {
  public:
    InterruptLock() {
      noInterrupts();
    }
    ~InterruptLock() {
      interrupts();
    }
};




*********************
#endif
