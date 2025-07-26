#include "_rs_ssd1306.h"       // SSD1306 ekran başlık dosyası
#include "_rs_fonts.h"         // Font tablosu başlık dosyası


// Frame Buffer
uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];     // Ekran verilerini tutan buffer
/*

Bu satır, SSD1306 ekranı için bir buffer (ara bellek) tanımlar:
	SSD1306_WIDTH: Ekranın yatay piksel sayısı (ör. 128)
	SSD1306_HEIGHT: Ekranın dikey piksel sayısı (ör. 64)
	/ 8: Her byte 8 dikey pikseli tutar (SSD1306’da 8 satır = 1 byte)

*/

// Dahili ekran durum bayrağı
static uint8_t SSD1306_Initialized = 0;                         // Ekranın başlatılıp başlatılmadığını tutan bayrak

// =======================
// 1) I2C Komut Gönderimi
// =======================

void SSD1306_WriteCommand(uint8_t command)                       // SSD1306'ya komut gönderen fonksiyon
{
    uint8_t data[2];
    data[0] = 0x00;  // Control byte: Co = 0, D/C# = 0 -> Komut
    data[1] = command;
    I2C1_Write(SSD1306_I2C_ADDR, data, 2);                      // I2C ile komutu gönder
}

/*

Bu fonksiyonun amacı, SSD1306 ekranına bir komut göndermektir.

Nasıl çalışır:
	data[0] = 0x00;
		Kontrol byte’ı: Co = 0, D/C# = 0 → Komut gönderileceğini belirtir.
	data[1] = command;
		Gönderilecek komut değeri.
	I2C1_Write(SSD1306_I2C_ADDR, data, 2);
		I2C üzerinden SSD1306’ya 2 byte (kontrol + komut) gönderilir.

Sonuç:
	SSD1306 ekranına istenen komut I2C ile iletilir.

*/

// =======================
// 2) Ekran Başlatma
// =======================

void SSD1306_Init(void)                                          // SSD1306 ekranı başlatan fonksiyon
{
    I2C1_Init();                                                 // I2C1 modülünü başlat

    // SSD1306 Init Komutları (Standart 128x64 için)
    SSD1306_WriteCommand(0xAE); // Display OFF                   // Ekranı kapat

    SSD1306_WriteCommand(0x20); // Memory Addressing Mode        // Bellek adresleme modu seç
    SSD1306_WriteCommand(0x00); // Horizontal Addressing Mode    // Yatay adresleme modu

    SSD1306_WriteCommand(0xB0); // Page Start Address            // Sayfa başlangıç adresi
    SSD1306_WriteCommand(0xC8); // COM Output Scan Direction     // COM çıkış yönü (ters)
    SSD1306_WriteCommand(0x00); // Low column address            // Düşük sütun adresi
    SSD1306_WriteCommand(0x10); // High column address           // Yüksek sütun adresi

    SSD1306_WriteCommand(0x40); // Start line address            // Başlangıç satır adresi
    SSD1306_WriteCommand(0x81); // Contrast control              // Kontrast ayarı
    SSD1306_WriteCommand(0xFF);                                  // Maksimum kontrast

    SSD1306_WriteCommand(0xA1); // Segment re-map                // Segment yeniden eşleme
    SSD1306_WriteCommand(0xA6); // Normal display                // Normal ekran modu

    SSD1306_WriteCommand(0xA8); // Multiplex ratio               // Multiplex oranı
    SSD1306_WriteCommand(0x3F);                                  // 1/64 duty

    SSD1306_WriteCommand(0xA4); // Output RAM to Display         // RAM'deki veriyi ekrana aktar

    SSD1306_WriteCommand(0xD3); // Display offset                // Ekran ofseti
    SSD1306_WriteCommand(0x00);                                  // Ofset yok

    SSD1306_WriteCommand(0xD5); // Display clock divide          // Ekran saat bölücü
    SSD1306_WriteCommand(0xF0);                                  // Saat ayarı

    SSD1306_WriteCommand(0xD9); // Pre-charge period             // Ön şarj süresi
    SSD1306_WriteCommand(0x22);                                  // Süre ayarı

    SSD1306_WriteCommand(0xDA); // COM pins                      // COM pin yapılandırması
    SSD1306_WriteCommand(0x12);                                  // Ayar

    SSD1306_WriteCommand(0xDB); // VCOM detect                   // VCOMH voltaj seviyesi
    SSD1306_WriteCommand(0x20);                                  // Ayar

    SSD1306_WriteCommand(0x8D); // Charge pump                   // Şarj pompası ayarı
    SSD1306_WriteCommand(0x14);                                  // Şarj pompası aktif

    SSD1306_WriteCommand(0xAF); // Display ON                    // Ekranı aç

    // Buffer temizle
    SSD1306_Fill(0);                                            // Buffer'ı sıfırla (ekranı temizle)
    SSD1306_UpdateScreen();                                     // Temiz ekranı göster

    SSD1306_Initialized = 1;                                    // Başlatıldı bayrağını ayarla
}

/*

SSD1306 ekranını kullanıma hazır hale getirmek için gönderilmesi gereken temel komutlar ve sıralaması aşağıdaki gibidir. Bu sıralama, ekranın doğru şekilde başlatılması için gereklidir ve genellikle datasheet’te önerilen adımlardır:

SSD1306 Başlatma Komutları ve Sıralaması
	Ekranı kapat:
		0xAE (Display OFF)

	Bellek adresleme modu seç:
		0x20 (Memory Addressing Mode)
		0x00 (Horizontal Addressing Mode)

	Sayfa başlangıç adresi:
		0xB0

	COM çıkış yönü:
		0xC8 (Scan direction)

	Sütun adresleri:
		0x00 (Low column address)
		0x10 (High column address)

	Başlangıç satır adresi:
		0x40

	Kontrast ayarı:
		0x81
		0xFF (Maksimum kontrast)

	Segment yeniden eşleme:
		0xA1

	Ekran modu:
		0xA6 (Normal display)

	Multiplex oranı:
		0xA8
		0x3F (1/64 duty)

	RAM'deki veriyi ekrana aktar:
		0xA4

	Ekran ofseti:
		0xD3
		0x00

	Saat bölücü:
		0xD5
		0xF0

	Ön şarj süresi:
		0xD9
		0x22

	COM pin yapılandırması:
		0xDA
		0x12

	VCOMH voltaj seviyesi:
		0xDB
		0x20

	Şarj pompası ayarı:
		0x8D
		0x14 (Charge pump enable)

	Ekranı aç:
		0xAF (Display ON)

	Kısaca: Minimum Gerekli Komutlar
		0xAE (Display OFF)
		Adresleme ve ekran parametreleri (yukarıdaki sıralama ile)
		0xAF (Display ON)

Notlar
	Komutlar mutlaka sıralı gönderilmelidir.
	Bazı parametreler (kontrast, multiplex, scan direction vs.) ekran tipine göre değişebilir.
	Başlatma sonrası ekranı temizlemek için buffer sıfırlanıp ekrana gönderilir.

Komut açıklamaları:
	0xAE	Display OFF: Ekranı kapatır. Başlatma sırasında ekran kapalı olmalı.
	0x20	Memory Addressing Mode: Bellek adresleme modunu seçer.
	0x00	Horizontal Addressing Mode: Yatay adresleme modunu aktif eder.
	0xB0	Page Start Address: Sayfa başlangıç adresini ayarlar.
	0xC8	COM Output Scan Direction: Satırların tarama yönünü belirler (ters).
	0x00	Low Column Address: Sütun adresinin düşük kısmını ayarlar.
	0x10	High Column Address: Sütun adresinin yüksek kısmını ayarlar.
	0x40	Start Line Address: Ekranın başlangıç satırını ayarlar.
	0x81	Contrast Control: Kontrast ayar komutu.
	0xFF	Contrast Value: Kontrastı maksimuma ayarlar.
	0xA1	Segment Re-map: Segmentlerin yönünü değiştirir (yatay ters çevirme).
	0xA6	Normal Display: Normal ekran modunu seçer (ters mod kapalı).
	0xA8	Multiplex Ratio: Multiplex oranı ayar komutu.
	0x3F	Multiplex Value: 1/64 duty (128x64 ekran için).
	0xA4	Output RAM to Display: RAM'deki veriyi ekrana aktarır.
	0xD3	Display Offset: Ekran ofsetini ayarlar.
	0x00	Offset Value: Ofset yok (sıfır).
	0xD5	Display Clock Divide: Ekran saat bölücü ayar komutu.
	0xF0	Clock Value: Saat bölücü ve frekans ayarı.
	0xD9	Pre-charge Period: Ön şarj süresi ayar komutu.
	0x22	Pre-charge Value: Ön şarj süresi değeri.
	0xDA	COM Pins: COM pin yapılandırma komutu.
	0x12	COM Pin Value: COM pin ayarı (128x64 için).
	0xDB	VCOM Detect: VCOMH voltaj seviyesi ayar komutu.
	0x20	VCOM Value: VCOMH voltaj seviyesi.
	0x8D	Charge Pump: Şarj pompası ayar komutu.
	0x14	Charge Pump Enable: Şarj pompasını aktif eder.
	0xAF	Display ON: Ekranı açar, görüntülemeyi başlatır.

*/

// =======================
// 3) Ekranı Temizle veya Doldur
// =======================

void SSD1306_Fill(uint8_t color)                                // Ekranı tamamen doldur veya temizle
{
    for (uint32_t i = 0; i < sizeof(SSD1306_Buffer); i++)
    {
        SSD1306_Buffer[i] = (color) ? 0xFF : 0x00;              // 1 ise tüm bitler 1, 0 ise tüm bitler 0
    }
}

/*
 *
Bu fonksiyonun amacı, ekranın tamamını tek seferde doldurmak veya temizlemektir.

Nasıl çalışır:
	color 1 ise: buffer’daki tüm byte’lar 0xFF olur (tüm pikseller yanık).
	color 0 ise: buffer’daki tüm byte’lar 0x00 olur (tüm pikseller sönük).
	Döngü ile buffer’ın tüm elemanları ayarlanır.

Sonuç:
	Ekran tamamen siyah (temiz) veya tamamen beyaz (dolu) olur. Görüntülemek için SSD1306_UpdateScreen() çağrılmalıdır.

*/

// =======================
// 4) Piksel İşleme
// =======================

void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color)     // Belirli bir pikseli ayarla
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;      // Sınır kontrolü

    uint16_t byteIndex = x + (y / 8) * SSD1306_WIDTH;           // Pikselin buffer'daki byte adresi

    if (color)
        SSD1306_Buffer[byteIndex] |= (1 << (y % 8));            // Pikseli 1 yap
    else
        SSD1306_Buffer[byteIndex] &= ~(1 << (y % 8));           // Pikseli 0 yap
}

/*

Bu fonksiyonun amacı, ekran buffer’ında belirli bir pikseli yakmak veya söndürmektir.
Yani, (x, y) koordinatındaki pikseli buffer’da 1 (yanık) veya 0 (sönük) olarak ayarlar

Sınır kontrolü:
	Eğer x veya y ekran boyutlarını aşarsa fonksiyon çıkış yapar.

Buffer adresi hesaplama:
	Her pikselin buffer’daki yeri:
	byteIndex = x + (y / 8) * SSD1306_WIDTH;

	Her byte 8 dikey pikseli tutar.
	(y / 8) ile doğru sayfa (byte satırı) bulunur.

Pikseli ayarlama:
	color 1 ise: ilgili bit 1 yapılır (piksel yanar).
	color 0 ise: ilgili bit 0 yapılır (piksel söner).

Sonuç:
	Buffer’da istenen piksel ayarlanır. Ekranda görünmesi için SSD1306_UpdateScreen() çağrılmalıdır.

*/

// =======================
// 5) Ekranı Güncelle
// =======================

void SSD1306_UpdateScreen(void)                                 // Buffer'daki veriyi ekrana aktar
{
    for (uint8_t page = 0; page < 8; page++)                   // 8 sayfa (her biri 8 satır)
    {
        SSD1306_WriteCommand(0xB0 + page); // Page address     // Sayfa adresini ayarla
        SSD1306_WriteCommand(0x00);        // Low column start address
        SSD1306_WriteCommand(0x10);        // High column start address

        uint8_t data[SSD1306_WIDTH + 1];
        data[0] = 0x40;  // Control byte: Co = 0, D/C# = 1 -> Data

        for (uint8_t i = 0; i < SSD1306_WIDTH; i++)
        {
            data[i + 1] = SSD1306_Buffer[page * SSD1306_WIDTH + i]; // Buffer'dan veriyi al
        }

        I2C1_Write(SSD1306_I2C_ADDR, data, SSD1306_WIDTH + 1);      // Sayfa verisini I2C ile gönder
    }
}

/*

Bu fonksiyonun amacı, ekran buffer’ında tutulan görüntü verisini SSD1306 ekranına aktarmaktır.
Yani, ekranda gösterilmek istenen tüm piksel bilgilerini I2C üzerinden ekrana gönderir ve ekranı günceller

1. Sayfa Döngüsü
	SSD1306 ekranı 8 sayfadan oluşur (her sayfa 8 satır içerir).
	Her döngüde bir sayfa ekrana aktarılır.
2. Sayfa ve Sütun Adreslerini Ayarlama
	Ekranda hangi sayfaya ve sütuna veri yazılacağını belirtir.
3. Veri Hazırlama
	İlk byte kontrol byte’ıdır (0x40: Co=0, D/C#=1 → Data mode).
	Sonraki byte’lar ekrana yazılacak veridir.
4. Buffer’dan Veriyi Almak
	Her sayfa için buffer’daki ilgili veriler alınır ve data dizisine eklenir.
5. Veriyi I2C ile Ekrana Göndermek
	Hazırlanan veri dizisi I2C üzerinden ekrana gönderilir.
Özet:
	Bu fonksiyon, buffer’daki tüm ekran verisini 8 sayfa halinde SSD1306 ekranına aktarır.
	Her sayfa için adres ayarlanır, veri hazırlanır ve I2C ile gönderilir.

*/

// =======================
// 6) Yazı & String Fonksiyonları (Basit)
// =======================


void SSD1306_DrawChar(uint8_t x, uint8_t y, char c)              // Ekrana karakter yazan fonksiyon
{
    for (uint8_t i = 0; i < 5; i++)
    {
        uint8_t line = Font5x7[c - 32][i];                       // Karakterin font datasını al.
        for (uint8_t j = 0; j < 8; j++)
        {
            if (line & 0x01)
                SSD1306_DrawPixel(x + i, y + j, 1);              // Bit 1 ise pikseli yak
            else
                SSD1306_DrawPixel(x + i, y + j, 0);              // Bit 0 ise pikseli söndür
            line >>= 1;                                          // Bir sonraki bite geç
        }
    }
}

/*

Font5x7 dizisi, her karakter için 5 sütun ve 8 satırdan oluşan bitmap font verisi tutar.
c - 32 ile ASCII tablosunda boşluktan (' ') başlatılır.
i sütun indeksidir; karakterin 5 sütunundan birini seçer.

Her sütun için 8 bitlik veriyi satırlara ayırır.
line & 0x01 ile en düşük bit kontrol edilir:
Bit 1 ise, ilgili pikseli yak (SSD1306_DrawPixel(x + i, y + j, 1)).
Bit 0 ise, pikseli söndür (SSD1306_DrawPixel(x + i, y + j, 0)).
Her döngüde line sağa kaydırılır, bir sonraki satırın bitine geçilir.

x + i ile karakterin sütununu, y + j ile satırını belirler.
Her bit, ekranda doğru konuma yazılır

Özet Akış
	Karakterin font datası alınır.
	Her sütun için 8 bitlik satır verisi ayrıştırılır.
	Her bit ekranda ilgili piksele yazılır.

"a" karakterini ekrana çizdirmek için SSD1306_DrawChar fonksiyonunun adım adım nasıl çalıştığı:
	1. Font Dizisinden "a" Karakterinin Bitmap'ini Almak
		ASCII kodu: 'a' = 97
		Font dizisinde: Font5x7[97 - 32] = Font5x7[65]
		Dizideki değerler: {0x20, 0x54, 0x54, 0x54, 0x78}
		Her eleman bir sütunu (column) temsil eder, her sütun 8 bitlik bir satır bilgisidir.

	2. Sütunları ve Satırları Döngüyle İşlemek
		İlk Sütun (i = 0)
			line = 0x20 → Binary: 0010 0000
			Satır döngüsü (j = 0'dan j = 7'ye):
			Her bit için:
			Bit 0: 0 → Piksel sönük
			Bit 1: 0 → Piksel sönük
			Bit 2: 1 → Piksel yanık (SSD1306_DrawPixel(x+0, y+2, 1))
			Bit 3: 0 → Piksel sönük
			Bit 4: 0 → Piksel sönük
			Bit 5: 0 → Piksel sönük
			Bit 6: 0 → Piksel sönük
			Bit 7: 0 → Piksel sönük
		İkinci Sütun (i = 1)
			line = 0x54 → Binary: 0101 0100
			Satır döngüsü:
			Bit 0: 0 → Piksel sönük
			Bit 1: 0 → Piksel sönük
			Bit 2: 1 → Piksel yanık
			Bit 3: 0 → Piksel sönük
			Bit 4: 1 → Piksel yanık
			Bit 5: 0 → Piksel sönük
			Bit 6: 1 → Piksel yanık
			Bit 7: 0 → Piksel sönük
		Üçüncü Sütun (i = 2)
			line = 0x54 → Aynı şekilde işlenir.
		Dördüncü Sütun (i = 3)
			line = 0x54 → Aynı şekilde işlenir.
		Beşinci Sütun (i = 4)
			line = 0x78 → Binary: 0111 1000
			Satır döngüsü:
			Bit 0: 0 → Piksel sönük
			Bit 1: 0 → Piksel sönük
			Bit 2: 0 → Piksel sönük
			Bit 3: 1 → Piksel yanık
			Bit 4: 1 → Piksel yanık
			Bit 5: 1 → Piksel yanık
			Bit 6: 1 → Piksel yanık
			Bit 7: 0 → Piksel sönük
	3. Sonuç
		Fonksiyon, "a" karakterinin bitmap'ini sütun sütun ve satır satır okur.
		Her bit için, ekranda ilgili pikseli yakar veya söndürür.
		Sonuçta "a" karakteri ekranda 5x7 piksel olarak çizilmiş olur.

*/

void SSD1306_DrawString(uint8_t x, uint8_t y, const char* str)   // Ekrana string yazan fonksiyon
{
    while (*str)
    {
        SSD1306_DrawChar(x, y, *str++);                          // Her karakteri sırayla yaz
        x += 6; // 5 px + 1 boşluk                               // Bir sonraki karakter için x'i kaydır
    }
}

/*

1. String İçindeki Karakterleri Tek Tek İşlemek
	*str ifadesi, stringin ilk karakterini alır.
	Her karakter için SSD1306_DrawChar fonksiyonu çağrılır ve ekrana yazılır.
	str++ ile bir sonraki karaktere geçilir.
	Her karakterden sonra x koordinatı 6 piksel artırılır (5 piksel karakter genişliği + 1 piksel boşluk).
2. Sonuç
	Fonksiyon, stringdeki tüm karakterleri soldan sağa ekrana çizer.
	Her karakterin bitmap'i font tablosundan alınır ve ekrana yerleştirilir.
	Karakterler arasında 1 piksel boşluk bırakılır.
Özet:
	Bu fonksiyon, bir stringi ekrana yatay olarak, karakter karakter yazdırır.
	Her karakterin x konumu bir önceki karakterin bitiminden sonra başlar.

Bu while (*str) döngüsünden, stringin sonuna gelindiğinde çıkılır.
	C dilinde stringler null karakter ('\0', ASCII 0) ile biter.
	*str ifadesi, stringin o anki karakterini gösterir.
	Son karakterden sonra *str artık '\0' olur ve while koşulu false olur, döngü sonlanır.

	Özet:
		Döngü, stringin sonundaki null karaktere ('\0') ulaşınca otomatik olarak biter.

*/



