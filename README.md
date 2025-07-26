Proje Adı : MPU6050_SSD1306

Platform : STM32F4 Serisi (STM32F407 Discovery board)

Geliştirme Ortamı: STM32CubeIDE (Register seviyesinde)

Açıklama
Kodlar STM32 register seviyesinde yazıldığından öğrenim amaçlı mükemmel bir örnektir. "_rs_i2c_config.c" dosyasındaki fonksiyonlar I2C'nin temel çalışma prensiplerini yansıtır (START, STOP, ACK/NACK, adresleme vs.).
MPU6050 sensöründen okuduğumuz verileri SSD1306 oled ekran üzerinde görsel olarak gösterilmektedir.

Özellikler
• MPU6050 ivmeölçer/jyroskop sensöründen gerçek zamanlı olarak 6 eksenli hareket verileri (ivme: X/Y/Z, jiroskop: X/Y/Z) okunmaktadır.
• SSD1306 OLED ekran üzerinde görsel olarak gösterilmektedir.
• I2C haberleşmesi register seviyesinde programlanmıştır.
• Sensör okumaları ve okunan değerlerin işlenmesi için gerekli kodlar yazılmıştır.

Donanım Bağlantıları
MPU6050:
• PB6 → SCL (Clock) 
• PB7 → SDA (Data)
• GND → AD0
SSD1306:
• PB6 → SCL (Clock) 
• PB7 → SDA (Data)

Yazılım Bağımlılıkları
• Proje cubemx ile açılıp sadece clock ayarlamaları yapılmıştır. I2C haberleşmesi ve sensör okumları register seviyesinde yazılmıştır.

Kurulum & Derleme
• STM32CubeIDE ile yeni bir STM32F4 projesi oluşturun.
• Core klasöründeki .h ve .c uzantılı dosyaları proje klasörüne ekleyin.

CubeMX yapılandırmasında:
• CubeMX üzerinden clock ayarını yapın (HSE 8MHz -> HCLK = 168MHz)

Dosya Yapısı
├── Src/
│ ├ _rs_i2c_config.c
│ ├ _rs_mpu6050.c
│ ├ _rs_ssd1306.c
│ ├ main.c
│ └ ...
└── Inc/
│ ├ _rs_fonts.h
│ ├ _rs_i2c_config.h
│ ├ _rs_mpu6050.h
│ ├ _rs_ssd1306.h
│ └ ...
└── ...
