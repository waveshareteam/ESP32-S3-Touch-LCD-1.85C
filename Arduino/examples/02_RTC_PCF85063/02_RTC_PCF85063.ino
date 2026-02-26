
#include "RTC_PCF85063.h"

// 辅助函数：将单个数字（0-99）格式化为两位（不足补0）
void print_two_digits(uint8_t num) {
    if (num < 10) {
        Serial.print("0");  // 小于10则补0
    }
    Serial.print(num);
}

// 辅助函数：将星期数字（dotw）转换为文字（根据实际定义调整）
// 假设 dotw=0→周日，1→周一，...，6→周六
void print_weekday(uint8_t dotw) {
    switch (dotw) {
        case 0: Serial.print("日"); break;
        case 1: Serial.print("一"); break;
        case 2: Serial.print("二"); break;
        case 3: Serial.print("三"); break;
        case 4: Serial.print("四"); break;
        case 5: Serial.print("五"); break;
        case 6: Serial.print("六"); break;
        default: Serial.print("?");  // 无效值
    }
}

void print_datetime() {
    // 打印日期（年-月-日 星期）
    Serial.print("日期：");
    Serial.print(datetime.year);
    Serial.print("-");
    print_two_digits(datetime.month);  // 确保月份占两位（如02月）
    Serial.print("-");
    print_two_digits(datetime.day);    // 确保日期占两位（如05日）
    Serial.print("  星期");
    print_weekday(datetime.dotw);      // 转换星期数字为文字（如1→一）

    // 打印时间（时:分:秒）
    Serial.print("  时间：");
    print_two_digits(datetime.hour);   // 确保小时占两位（如08时）
    Serial.print(":");
    print_two_digits(datetime.minute); // 确保分钟占两位（如03分）
    Serial.print(":");
    print_two_digits(datetime.second); // 确保秒占两位（如09秒）
    Serial.println();  // 换行
}

void setup()
{
  Serial.begin(115200);
  I2C_Init();
  PCF85063_Init();
}

void loop() {
  PCF85063_Loop();
  print_datetime();
  vTaskDelay(pdMS_TO_TICKS(1000));
}
