/* DHT library
  Cái thư viện này là của adafruit :)) Thằng này thì bá đạo rồi không có gì để nói :))
  Còn tui thì chỉ là cái thằng học cách viết thư viện và vietsub lại theo cách hiểu của tui
  Contact : buiquocbaodt@gmail.com
*/

#include "DHT.h"

#define MIN_INTERVAL 2000 // Thời gian tối thiểu

DHT::DHT(uint8_t pin, uint8_t type, uint8_t count) {
  _pin = pin;
  _type = type;
  #ifdef __AVR
    _bit = digitalPinToBitMask(pin);
    _port = digitalPinToPort(pin);
  #endif
  /*
   *  1 milisecond để đọc xung từ cảm biến DHT
   *  Lưu ý : Số lượng hiện tại bị bỏ qua khi đọc DHT tự điều chỉnh dựa trên tốc độ của bộ vi xử lý !
   *  Hàm _maxcycles này trả về kiểu 16.000 = 16Mhz của thạch anh . Cũng hơi khó hiểu ?   
  */
  _maxcycles = microsecondsToClockCycles(1000);  
}

void DHT::begin(void) {
  // Thiết lập các chân
  pinMode(_pin, INPUT_PULLUP);
  /*
  *   Sử dụng giá trị _lastreadtime để đảm bảo rằng mililis() - thời gian đọc cuối cùng 
  *   sẽ lớn hơn hoặc bằng MIN_INTERVAL ngay lập tức ! Lưu ý rằng
  */
  _lastreadtime = -MIN_INTERVAL;
  DEBUG_PRINT("Max clock cycles: "); DEBUG_PRINTLN(_maxcycles, DEC);
}

//boolean S == Tỉ lệ !!! True == Đọc độ F; False == Đọc độ Cd
float DHT::readTemperature(bool S, bool force) {
  float f = NAN;  // Cho F = nan -->> println(f) ---> nan

  /*
  *   Khúc này là đọc cái force trong hàm read 
  *   Nếu read(force) --> false ->> thì chạy vào sw
  *   Nếu read(force) --> true  ->> thì return về giá trị f = NAN;
  */
  if (read(force))  
  {
    switch (_type) 
    {
      case DHT11:
        f = data[2];    /// Nhiệt độ được lấy từ data[2] --->    
        if(S) {
          f = convertCtoF(f);
        }
        break;
      case DHT22:
      case DHT21:
        f = data[2] & 0x7F;
        f *= 256;
        f += data[3];
        f *= 0.1;
        if (data[2] & 0x80) 
        {
          f *= -1;
        }
        if(S) {
        f = convertCtoF(f);
        }
        break;
    }
  }
  return f;
}

float DHT::convertCtoF(float c) {
  return c * 1.8 + 32;
}

float DHT::convertFtoC(float f) {
  return (f - 32) * 0.55555;
}

float DHT::readHumidity(bool force) {
  float f = NAN;
  if (read()) {
    switch (_type) {
    case DHT11:
      f = data[0];
      break;
    case DHT22:
    case DHT21:
      f = data[0];
      f *= 256;
      f += data[1];
      f *= 0.1;
      break;
    }
  }
  return f;
}

//boolean isFahrenheit: True == Fahrenheit; False == Celcius
float DHT::computeHeatIndex(float temperature, float percentHumidity, bool isFahrenheit) {
  // Using both Rothfusz and Steadman's equations
  // http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml
  float hi;

  if (!isFahrenheit)
    temperature = convertCtoF(temperature);

  hi = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) + (percentHumidity * 0.094));

  if (hi > 79) {
    hi = -42.379 +
             2.04901523 * temperature +
            10.14333127 * percentHumidity +
            -0.22475541 * temperature*percentHumidity +
            -0.00683783 * pow(temperature, 2) +
            -0.05481717 * pow(percentHumidity, 2) +
             0.00122874 * pow(temperature, 2) * percentHumidity +
             0.00085282 * temperature*pow(percentHumidity, 2) +
            -0.00000199 * pow(temperature, 2) * pow(percentHumidity, 2);

    if((percentHumidity < 13) && (temperature >= 80.0) && (temperature <= 112.0))
      hi -= ((13.0 - percentHumidity) * 0.25) * sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);

    else if((percentHumidity > 85.0) && (temperature >= 80.0) && (temperature <= 87.0))
      hi += ((percentHumidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
  }

  return isFahrenheit ? hi : convertFtoC(hi);
}

boolean DHT::read(bool force) {
  /*
  *   Kiểm tra sensor đã được đọc ít hơn ****(2000)->(2s) trước & quay lại 
  *   để sử dụng lần đọc cuối cùng !!!
  */

  // -> Tạo biến lấy thời gian hiện tại ! unint32_t --> 0 to 4,294,967,295
  uint32_t currenttime = millis();
  // -> Hàm if nếu force = false , lấy thời gian hiện tại trừ thời gian đọc cuối cùng nếu < 2000 (2s) 

  if (!force && ((currenttime - _lastreadtime) < 2000)) {
    /*
        _lastresult là hàm bool !
    */
    return _lastresult; // Trả lại số đo chính xác cuối cùng !!!
  }
  //Lấy thời gian đọc cuối cùng đưa vào hàm currenttime;
  _lastreadtime = currenttime;

  /*  Đưa 40 bit dữ liệu nhận được về bằng 0 !
      Tự nhiên nhớ lại hồi học quá à hơi lan man 1 bit = 8byte :)) ôn lại kiến thức xíu 
      Vậy là sử dụng 4 bit ---> 32 byte
   */
  data[0] = data[1] = data[2] = data[3] = data[4] = 0;

  /* Gửi tín hiệu bắt đầu
   Cái này đọc DHT của thằng adafruit!
  --> https://cdn-shop.adafruit.com/datasheets/Digital+humidity+and+temperature+sensor+AM2302.pdf
  */

  /*  Một mẫu ví dụ 
    0000 0010 1000 1100 --- 0000 0001 0101 1111 --- 1110 1110
    16 bits RH data           16 bits T data        check sum
    Đầu tiên chuyển 16 bit dữ liệu RH từ hệ thống nhị phân sang hệ thập phân
             chuyển 16 bit dữ liệu T từ hệ thống nhị phân sang hệ thập phân
    0000 0010 1000 1100 (Binary system) --->    652 (Decimal system) --> RH=652/10=65.2%RH 
    0000 0001 0101 1111 (Binary system) --->    351 (Decimal system) --> T=351/10=35.1℃
    --> RH = 65.2%
    --> TH = 35.1%
  */  

  /*  Bắt đầu quá trình đọc 
   *  Đưa trạng thái chân lên cao !
  */

  digitalWrite(_pin, HIGH);
********  delay(250); // ---> Cái khúc này sao lại delay 250 nhỉ ??/

    /*  Khúc này là đưa tín hiệu chân xuống mức thấp trong khoảng 20ms
    *   Đọc trên datasheet thì nó bảo kéo xuống 1-10 ms (để test thử) ***test***
    *   Mà méo hiểu sao cái khúc trên nó đưa điện áp lên mức cao nhỉ ???
    *   Xong rồi xuống đây nó mới khai bán chân là OUTPUT ???
    *   Thấy cũng hơi khó hiểu đoạn này ??
    */
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delay(20);

  uint32_t cycles[80];
  /* Cái khúc này gọi là lệnh phức =))
   * Nhiều lệnh đơn có thể kết nối lại thành một lệnh phức bằng 
   * cách rào chúng bên trong các dấu ngoặc xoắn ( ôn lại kiến thức tí )
  */
  {
    /*  Tắt ngắt tạm thời nói chung cho khúc code dưới không bị gián đoạn :))
    *   Còn cái InterruptLock là gì thì mở class bên file .h đọc 
    *   Hiểu nôm na là tắt cái interrup này để chạy chương trình yêu cầu cao về thời gian
    *   để tập trung xử lí dữ liệu 
    *   Link read : http://arduino.vn/reference/nointerrupts 
    */
    InterruptLock lock;

    /*  Kết thúc tín hiệu đầu bằng cách đưa chân đọc lên cao và chờ trong 40 micros
     *  Trong datasheet thì nó bảo là thời gian chỉ cần 20 - 40 mircƯo(để test thử) ***test***
     *  Link datasheet cho lười kéo lên kiếm : 
     *  --> https://cdn-shop.adafruit.com/datasheets/Digital+humidity+and+temperature+sensor+AM2302.pdf
    */
    digitalWrite(_pin, HIGH);
    delayMicroseconds(40);

    /*
     *  Khúc này là bắt đầu đọc data để lấy giá trị từ cảm biến !
    */
    pinMode(_pin, INPUT_PULLUP);
    delayMicroseconds(10);  // Chắc sợ VĐK chạy chậm không kịp kéo lên nó tạm thời cho nó delay 10 Micro :))

    /*  
     *  Khúc này vẫn trả lại số do cuối cùng của _lastresult
     *  Đợi tín hiệu mức thấp trong 80 micros để lấy tín hiệu phản hồi
     *  Sau đó đợi tín hiệu cao trong 80 micros để chuẩn bị gửi dữ liệu
     *  Đọc tiếp đoạn dưới 1
     *  À cái expectPulse là gì thì kéo xuống dưới cùng đọc !
    */

    /*
     *  Trong lúc đọc pulse thì _lastresult trở về false
    */

    if (expectPulse(LOW) == 0) {
      DEBUG_PRINTLN(F("Timeout waiting for start signal low pulse."));
      _lastresult = false;
      return _lastresult;
    }
    if (expectPulse(HIGH) == 0) {
      DEBUG_PRINTLN(F("Timeout waiting for start signal high pulse."));
      _lastresult = false;
      return _lastresult;
    }

    /*  
        Bây giờ sẽ đọc 40 bthấpits được gửi từ sensor ! Mỗi lầ truyền bit bắt đầu với 
        mức điện áp xung  50 micros , theo độ dài tín hiệu mức điện áp quyết định bit là 0 hay 1
        Nếu xung cao ~28 micros ---> bit = 0;
        Nếu xung cao ~70 micros ---> bit = 1;
        Đo số chu kì của xung thấp 50micros sau đó so sánh với với số chu kỳ xung cao để xác định xem 
        bit là 0 if (high cycle < low cycle);
        bit là 1 if (high cycle > low cycle);
        Các xung sẽ được đọc thành một mảng !!!
    */

    /*  uint32_t cycles[80]; được khai báo ở trên
        Đợi i đọc ở mức LOW --> Sau đó tăng i++ rồi đợi mức HIGH
        Sau đó i+2 cho đến hết chu kỳ ?
        Mà tại sao lại i = i+2    ????

      Lưu thành một mảng cycles[80]
    */ 

    for (int i=0; i<80; i+=2) {
      cycles[i]   = expectPulse(LOW); 
      cycles[i+1] = expectPulse(HIGH);
    }
  } 

  /*
    Đo số chu kì của xung thấp 50micros sau đó so sánh với với số chu kỳ xung cao để xác định xem 
        bit là 0 if (high cycle < low cycle);
        bit là 1 if (high cycle > low cycle);
  */

  for (int i=0; i<40; ++i) {
    uint32_t lowCycles  = cycles[2*i];
    uint32_t highCycles = cycles[2*i+1];

    if ((lowCycles == 0) || (highCycles == 0)) {
      DEBUG_PRINTLN(F("Timeout waiting for pulse."));
      _lastresult = false;
      return _lastresult;
    }
    data[i/8] <<= 1;
    // Khúc này so sánh chu kỳ cao nếu lớn hơn chu kỳ thất thì cái bit đó là 1
    if (highCycles > lowCycles) {
      data[i/8] |= 1;
    }
    // Bình thường bit nó là 0 nên cũng không cần kiểm tra bit 0
  }

  /*
      HEX là hệ thập lục phân !
      DEC : là hệ thập phân !
      BINary là hệ nhị phân !
  */

  DEBUG_PRINTLN(F("Received:"));
  DEBUG_PRINT(data[0], HEX); DEBUG_PRINT(F(", "));  ---> 8 byte of humidity
  DEBUG_PRINT(data[1], HEX); DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[2], HEX); DEBUG_PRINT(F(", "));  ---> 8 byte of temperature
  DEBUG_PRINT(data[3], HEX); DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[4], HEX); DEBUG_PRINT(F(" =? "));
  DEBUG_PRINTLN((data[0] + data[1] + data[2] + data[3]) & 0xFF, HEX);

  // Check we read 40 bits and that the checksum matches.
  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
    _lastresult = true;
    return _lastresult;
  }
  else {
    DEBUG_PRINTLN(F("Checksum failure!"));
    _lastresult = false;
    return _lastresult;
  }
}

/*  Nôm na là đường tín hiệu được chỉ định trong một thời gian và sẽ return
 *  về số lượng chu kỳ vòng đã sử dụng ở trước đó ( cái số chu kì này sử dụng để so sánh 
 *  thời gian tương đối giữa hai xung !
 *  If ( time > 1/1000 s) ---> return 0 ---> thất bại !
 *  Cái ông viết thư viện kêu tham khảo hàm pulseInLong theo cái link này
 *  https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/wiring_pulse.c
*/

uint32_t DHT::expectPulse(bool level) {
  uint32_t count = 0;
  /*   Vì dùng AVR nên sẽ truy cập  GPIO trực tiếp vì nó nhanh , ngon bổ rẻ 
   *   cho việc bắt các xung độ dài 10 s!!!
   *   level là biến được khai báo trên hàm read --> level = LOW -> low , level = HIGH -> true;
  */
  #ifdef __AVR
    /*    _bit = digitalPinToBitMask(pin) <--> (13)
     *    portState = 0 nếu level LOW
     *    portState = _bit nếu level HIGH (ex 13 ---> 32)
    */
    uint8_t portState = level ? _bit : 0;
    /*  
     *  Trỏ giá trị của 
     *  Khúc này so sánh portSate với 
     *  _port = digitalPinToPort(_pin)
     *  Port B --> 2 : 8->13 
     *  Port C --> 3 : AnalogInputPin;
     *  Port D --> 4 : 0->7 
     *  khúc này là lấy địa chỉ của biến bit và truyền tham số _port vào
     *  Đại loại là so sánh cái chân khai báo nó nằm cùng portState đọc được không
     *  Ví dụ chân trỏ giá trị của portInputRegister(13) coi có bằng portState không ?
    */
    while ((*portInputRegister(_port) &_bit) == portState) {
    // Cho cái biến count nó đếm tới 16.000 , nếu lớn hơn 16.000 thì chạy về 0 ***_maxcycles đọc ở trên ra 16 000
      if (count++ >= _maxcycles) {
        return 0; // Exceeded timeout, fail.
      }
    }
  // Nếu không phải AVR thì quay lại dùng digital cho nhanh , cái này áp dụng cho ESP8266
  #else
    /*
     *  level có hai mức HIGH và LOW
     *  Trong khi đọc chân Pin == level thì sẽ chạy hàm này !
     *  
    */
    while (digitalRead(_pin) == level) {
      if (count++ >= _maxcycles) {
        // Cho cái biến count nó đếm tới 16.000 , nếu lớn hơn 16.000 thì chạy về 0 ***_maxcycles đọc ở trên ra 16 000
        return 0; // Exceeded timeout, fail.
      }
    }
  #endif

  return count; // trả lại giá trị count;
}
