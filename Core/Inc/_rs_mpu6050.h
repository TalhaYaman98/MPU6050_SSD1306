#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f4xx.h"

#define SMPLRT_DIV_REG 0x19 // MPU6050 sensöründe Örnekleme Hızı Bölücü (Sample Rate Divider) register'ının adresi.
                            // Bu register, sensörün dahili örnekleme hızını belirler.

#define GYRO_CONFIG_REG 0x1B // MPU6050 sensöründe Cayro Konfigürasyonu (Gyroscope Configuration) register'ının adresi.
                            // Bu register, cayronun tam ölçek aralığını (Full-Scale Range) ve kendi kendini test (self-test) ayarlarını yapar.

#define ACCEL_CONFIG_REG 0x1C // MPU6050 sensöründe İvmeölçer Konfigürasyonu (Accelerometer Configuration) register'ının adresi.
                             // Bu register, ivmeölçerin tam ölçek aralığını (Full-Scale Range) ve kendi kendini test (self-test) ayarlarını yapar.

#define ACCEL_XOUT_H_REG 0x3B // MPU6050 sensöründe İvmeölçer X Ekseni Çıkışı (Yüksek Byte) register'ının adresi.
                             // X ekseni ivme verisinin yüksek 8 bitini içerir. İvme verisi 16 bittir (H ve L registerları).

#define TEMP_OUT_H_REG 0x41 // MPU6050 sensöründe Sıcaklık Çıkışı (Yüksek Byte) register'ının adresi.
                           // Dahili sıcaklık sensörünün yüksek 8 bitlik verisini içerir.

#define GYRO_XOUT_H_REG 0x43 // MPU6050 sensöründe Cayro X Ekseni Çıkışı (Yüksek Byte) register'ının adresi.
                           // X ekseni cayro verisinin yüksek 8 bitini içerir. Cayro verisi 16 bittir (H ve L registerları).

#define PWR_MGMT_1_REG 0x6B // MPU6050 sensöründe Güç Yönetimi 1 (Power Management 1) register'ının adresi.
                           // Sensörün uyku modu, saat kaynağı seçimi ve resetleme gibi güç ayarlarını kontrol eder.

#define WHO_AM_I_REG 0x75 // MPU6050 sensöründe "Ben Kimim?" (Who Am I) register'ının adresi.
                         // Sensörün kimliğini doğrulamak için kullanılan sabit bir değeri (genellikle MPU6050 için 0x68) içerir.
                         // Cihazın doğru şekilde bağlandığını kontrol etmek için kullanılır.

#define MPU6050_ADDR 0x68 // MPU6050 sensörünün I2C bus üzerindeki varsayılan 7-bit adresidir (AD0 pimi LOW ise).
                         // I2C haberleşmesinde sensöre erişmek için kullanılır. (Bazen 8-bit adresi için 0xD0 veya 0xD1 olarak da görülebilir, bu durumda son bit RW bitidir).

void MPU6050_Init(void); // MPU6050 başlatma fonksiyonu
void MPU6050_ReadGyro(float *gyroX, float *gyroY, float *gyroZ); // Jiroskop verilerini oku

#endif
