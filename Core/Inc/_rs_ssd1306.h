#ifndef SSD1306_H                       // Header dosyasının birden fazla eklenmesini önler
#define SSD1306_H

#include "stm32f4xx.h"                       // STM32F4 serisi için temel başlık dosyası
#include "_rs_i2c_config.h"                   // Kendi I2C yapılandırma/sürücü başlık dosyan

// SSD1306 I2C Adresi (bağlantıya göre 0x3C veya 0x3D olabilir)
#define SSD1306_I2C_ADDR  0x3C               // SSD1306'nın I2C adresi

// Ekran boyutları
#define SSD1306_WIDTH     128                // Ekran genişliği (piksel)
#define SSD1306_HEIGHT    64                 // Ekran yüksekliği (piksel)

// Buffer
extern uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8]; // Ekran verilerini tutan buffer (harici tanımlanacak)

// Temel fonksiyonlar
void SSD1306_Init(void);                     // Ekranı başlatan fonksiyon prototipi
void SSD1306_UpdateScreen(void);             // Buffer'daki veriyi ekrana aktaran fonksiyon prototipi
void SSD1306_Fill(uint8_t color);            // Ekranı tamamen dolduran fonksiyon prototipi
void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color); // Belirli bir pikseli çizen fonksiyon prototipi

// Yazı fonksiyonları
void SSD1306_DrawChar(uint8_t x, uint8_t y, char c);            // Ekrana karakter yazan fonksiyon prototipi
void SSD1306_DrawString(uint8_t x, uint8_t y, const char* str); // Ekrana string yazan fonksiyon prototipi

// Şekil fonksiyonları (isteğe bağlı)
void SSD1306_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);  // Doğru çizen fonksiyon prototipi
void SSD1306_DrawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h); // Dikdörtgen çizen fonksiyon prototipi

// Dahili kontrol fonksiyonları
void SSD1306_WriteCommand(uint8_t command);                  // Komut gönderen fonksiyon prototipi
void SSD1306_WriteData(uint8_t* data, uint16_t size);        // Veri gönderen fonksiyon prototipi

#endif // __SSD1306_H__                                      // Header dosyası korumasının
