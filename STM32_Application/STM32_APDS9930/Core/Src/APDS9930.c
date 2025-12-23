/*
 * APDS9930.c
 *
 *  Created on: Dec 16, 2025
 *      Author: Umran
 */

#include "APDS9930.h"

// I2C'ye bir byte veri yazan yardımcı fonksiyon
static HAL_StatusTypeDef write_byte(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t value) {
    uint8_t data[2];
    data[0] = reg;
    data[1] = value;
    // 0x80 komutu, adresin otomatik artmasını sağlar (datasheet'e göre)
    return HAL_I2C_Master_Transmit(hi2c, (APDS9930_I2C_ADDR << 1), data, 2, HAL_MAX_DELAY);
}

// I2C'den bir byte veri okuyan yardımcı fonksiyon
static HAL_StatusTypeDef read_byte(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *value) {
    // Önce okunacak register'ın adresini gönder
    HAL_I2C_Master_Transmit(hi2c, (APDS9930_I2C_ADDR << 1), &reg, 1, HAL_MAX_DELAY);
    // Sonra o adresten veriyi oku
    return HAL_I2C_Master_Receive(hi2c, (APDS9930_I2C_ADDR << 1), value, 1, HAL_MAX_DELAY);
}

// I2C'den iki byte (word) veri okuyan yardımcı fonksiyon
static HAL_StatusTypeDef read_word(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *value) {
    uint8_t data[2];
    // Önce okunacak register'ın başlangıç adresini gönder
    HAL_I2C_Master_Transmit(hi2c, (APDS9930_I2C_ADDR << 1), &reg, 1, HAL_MAX_DELAY);
    // Sonra o adresten 2 byte'lık veriyi oku
    HAL_I2C_Master_Receive(hi2c, (APDS9930_I2C_ADDR << 1), data, 2, HAL_MAX_DELAY);
    // Düşük ve yüksek byte'ları birleştirerek 16-bitlik değeri oluştur
    *value = (data[1] << 8) | data[0];
    return HAL_OK;
}

// Sensörü başlatan ana fonksiyon
uint8_t APDS9930_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t id = 0;

    // Sensörün kimliğini (ID) oku
    read_byte(hi2c, APDS9930_ID, &id);

    // Beklenen ID (0x39) ile eşleşiyor mu kontrol et
    if (id != 0x39) {
        return 0; // Hata: Sensör bulunamadı veya yanlış sensör
    }

    // Sensörü aktif et (PON), Yakınlık (PEN) ve Işık (AEN) algılamayı aç
    // 0x03 = PON | AEN
    // 0x07 = PON | AEN | PEN
    write_byte(hi2c, APDS9930_ENABLE, 0x07);

    // Gerekli diğer ayarlar (zamanlama, kazanç vb.) buraya eklenebilir.
    // Şimdilik varsayılan değerleri kullanıyoruz.
    // Örneğin, yakınlık için puls sayısı ve uzunluğu
    write_byte(hi2c, APDS9930_PPULSE, 8); // 8 puls, 8us uzunluğunda
    // Kontrol register'ı ile kazanç ayarları
    write_byte(hi2c, APDS9930_CONTROL, 0x00); // 1x ALS/PXY Gain

    return 1; // Başarılı
}

// Yakınlık verisini okuyan fonksiyon
uint8_t APDS9930_ReadProximity(I2C_HandleTypeDef *hi2c, uint16_t *proximity) {
    if (read_word(hi2c, APDS9930_PDATAL, proximity) == HAL_OK) {
        return 1; // Başarılı
    }
    return 0; // Hata
}

// Ortam ışığı verisini okuyan fonksiyon
uint8_t APDS9930_ReadAmbientLight(I2C_HandleTypeDef *hi2c, uint16_t *light) {
    if (read_word(hi2c, APDS9930_CDATAL, light) == HAL_OK) {
        return 1; // Başarılı
    }
    return 0; // Hata
}
