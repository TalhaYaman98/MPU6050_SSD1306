#include "_rs_mpu6050.h"
#include "_rs_i2c_config.h"



void MPU6050_Init(void)
{
	uint8_t check, data;
	I2C1_ReadByte(MPU6050_ADDR, WHO_AM_I_REG, &check);  // I2C1 kullanarak MPU6050'nin WHO_AM_I register'ını oku.
														// MPU6050_ADDR << 1: MPU6050'nin 7-bit I2C adresini 8-bit yazma adresine dönüştürür.
														// WHO_AM_I_REG: Cihazın kimliğini içeren register adresi.
														// I2C_MEMADD_SIZE_8BIT: Register adresinin 8 bit olduğunu belirtir.
														// &check: Okunan baytın depolanacağı 'check' değişkeninin adresi.
														// 1: Okunacak bayt sayısı.
														// 10: Zaman aşımı süresi (ms).

	if(check == 0x68)  											// MPU6050'nin ID'si 0x68'e eşitse (doğru cihazın bağlı olduğunu kontrol et).
	{
		// Sleep mode kapat
		data = 0;   											// 'data' değişkenini 0 yaparak uyku modunu kapatma komutunu hazırla.
		I2C1_WriteByte(MPU6050_ADDR, PWR_MGMT_1_REG, data);		// MPU6050'nin güç yönetim register'ına (PWR_MGMT_1_REG) 0 yazarak uyku modunu kapat.
																// Bu, sensörün çalışmaya başlamasını sağlar.
	}

}


void MPU6050_ReadGyro(float *gyroX, float *gyroY, float *gyroZ)
{
	uint8_t buffer[6];  // MPU6050'den ardışık olarak okunacak 6 baytlık veriyi (genellikle ivme veya cayro verisinin H ve L baytları)
						// depolamak için kullanılan 8-bitlik bir dizi.

	int16_t Gyro_X_RAW; // MPU6050'den okunan ham X ekseni cayro verisini depolamak için 16-bit işaretli tamsayı (signed integer) değişken.
	int16_t Gyro_Y_RAW; // MPU6050'den okunan ham Y ekseni cayro verisini depolamak için 16-bit işaretli tamsayı değişken.
	int16_t Gyro_Z_RAW; // MPU6050'den okunan ham Z ekseni cayro verisini depolamak için 16-bit işaretli tamsayı değişken.

	I2C1_ReadBytes(MPU6050_ADDR, GYRO_XOUT_H_REG, buffer, 6);	// I2C1 kullanarak MPU6050'den jiroskop verilerini oku.
																// MPU6050_ADDR : MPU6050'nin 7-bit adresini 8-bit yazma adresine dönüştürür.
																// GYRO_XOUT_H_REG: Okumaya başlanacak register adresi (X ekseni yüksek baytı).
																// BUFFER: Okunan verilerin depolanacağı dizi.
																// 6: Okunacak bayt sayısı (X, Y, Z için her biri 2 bayt, toplam 6).

	Gyro_X_RAW = (buffer[0] << 8) | buffer[1]; // X ekseni ham jiroskop değerini hesapla. Yüksek bayt (buffer[0]) sola 8 bit kaydırılır ve düşük bayt (buffer[1]) ile OR'lanır.
	Gyro_Y_RAW = (buffer[2] << 8) | buffer[3]; // Y ekseni ham jiroskop değerini hesapla.
	Gyro_Z_RAW = (buffer[4] << 8) | buffer[5]; // Z ekseni ham jiroskop değerini hesapla.

    *gyroX = (float)Gyro_X_RAW / 131.0f; // Ham X jiroskop değerini derece/saniye cinsinden float değere dönüştür. 131.0f MPU6050'nin varsayılan 250 derece/saniye hassasiyet ölçeğidir (FS_SEL=0 için).
    *gyroY = (float)Gyro_Y_RAW / 131.0f; // Ham Y jiroskop değerini derece/saniye cinsinden float değere dönüştür.
    *gyroZ = (float)Gyro_Z_RAW / 131.0f; // Ham Z jiroskop değerini derece/saniye cinsinden float değere dönüştür.

}


void MPU6050_ReadAccel(float *accelX, float *accelY, float *accelZ) { 			// MPU6050 ivmeölçer verilerini okuyan fonksiyon. Ax, Ay, Az ivme değerlerinin atanacağı işaretçilerdir.
    uint8_t buffer[6];				// I2C üzerinden okunan ham verileri depolamak için 6 baytlık bir dizi tanımlanır.

    int16_t Accel_X_RAW; // MPU6050'den okunan ham X ekseni ivmeölçer verisini depolamak için 16-bit işaretli tamsayı değişken.
    int16_t Accel_Y_RAW; // MPU6050'den okunan ham Y ekseni ivmeölçer verisini depolamak için 16-bit işaretli tamsayı değişken.
    int16_t Accel_Z_RAW; // MPU6050'den okunan ham Z ekseni ivmeölçer verisini depolamak için 16-bit işaretli tamsayı değişken.

    I2C1_ReadBytes(MPU6050_ADDR, ACCEL_XOUT_H_REG, buffer, 6); 	// I2C1 kullanarak MPU6050'den ivme verilerini oku.
																											// MPU6050_ADDR << 1: MPU6050'nin 7-bit adresini 8-bit yazma adresine dönüştürür.
																											// ACCEL_XOUT_H_REG: Okumaya başlanacak register adresi (X ekseni yüksek baytı).
																											// buffer: Okunan verilerin depolanacağı dizi.
																											// 6: Okunacak bayt sayısı (X, Y, Z için her biri 2 bayt, toplam 6).

    Accel_X_RAW = (buffer[0] << 8) | buffer[1];    // X ekseni ham ivme değerini hesapla. Yüksek bayt (Rec_Data[0]) sola 8 bit kaydırılır ve düşük bayt (Rec_Data[1]) ile OR'lanır.
    Accel_Y_RAW = (buffer[2] << 8) | buffer[3];    // Y ekseni ham ivme değerini hesapla.
    Accel_Z_RAW = (buffer[4] << 8) | buffer[5];    // Z ekseni ham ivme değerini hesapla.

    *accelX = (float)Accel_X_RAW / 16384.0f;   // Ham X ivme değerini G cinsinden float değere dönüştür. 16384.0f MPU6050'nin varsayılan 2g hassasiyet ölçeğidir (FS_SEL=0 için).
    *accelY = (float)Accel_Y_RAW / 16384.0f;   // Ham Y ivme değerini G cinsinden float değere dönüştür.
    *accelZ = (float)Accel_Z_RAW / 16384.0f;   // Ham Z ivme değerini G cinsinden float değere dönüştür.
}
