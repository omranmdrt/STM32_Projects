/**
 * @file ft24c02a_eeprom.h
 * @brief FT24C02A-usr-b EEPROM kütüphanesi başlık dosyası
 * @author Manus AI
 * @date Haziran 2025
 * 
 * Bu kütüphane, FT24C02A-usr-b EEPROM ile I2C protokolü üzerinden
 * haberleşmek için gerekli fonksiyonları içerir.
 */

#ifndef FT24C02A_EEPROM_H
#define FT24C02A_EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Gerekli başlık dosyaları */
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief FT24C02A EEPROM için varsayılan I2C adresi (7-bit)
 * @note A0, A1, A2 pinleri GND'ye bağlı olduğunda bu adres kullanılır
 */
#define FT24C02A_DEFAULT_ADDR     0x50

/**
 * @brief FT24C02A EEPROM için yazma döngüsü süresi (ms)
 * @note Bir yazma işleminden sonra bu süre kadar beklenmesi gerekir
 */
#define FT24C02A_WRITE_CYCLE_TIME 5

/**
 * @brief FT24C02A EEPROM için maksimum bellek boyutu (byte)
 * @note FT24C02A toplam 256 byte (2 Kbit) kapasiteye sahiptir
 */
#define FT24C02A_MAX_SIZE         256

/**
 * @brief FT24C02A EEPROM için maksimum sayfa boyutu (byte)
 * @note Sayfa yazma işleminde en fazla 16 byte yazılabilir
 */
#define FT24C02A_PAGE_SIZE        16

/**
 * @brief FT24C02A EEPROM kütüphanesi için hata kodları
 */
typedef enum {
    FT24C02A_OK = 0,          /**< İşlem başarılı */
    FT24C02A_ERROR_I2C,       /**< I2C haberleşme hatası */
    FT24C02A_ERROR_ADDR,      /**< Geçersiz bellek adresi */
    FT24C02A_ERROR_SIZE,      /**< Geçersiz veri boyutu */
    FT24C02A_ERROR_TIMEOUT,   /**< İşlem zaman aşımına uğradı */
    FT24C02A_ERROR_PARAM,     /**< Geçersiz parametre */
	FT24C02A_ERROR			  /**< Sİlme HATASI       */
} FT24C02A_Status;

/**
 * @brief FT24C02A EEPROM kütüphanesi için I2C haberleşme fonksiyon işaretçileri
 * @note Bu yapı, farklı mikrodenetleyiciler için I2C fonksiyonlarını soyutlar
 */
typedef struct {
    /**
     * @brief I2C üzerinden veri gönderme fonksiyonu
     * @param devAddr Cihaz adresi (7-bit)
     * @param pData Gönderilecek veri dizisi
     * @param size Gönderilecek veri boyutu
     * @param timeout Zaman aşımı süresi (ms)
     * @return İşlem başarılı ise 0, değilse hata kodu
     */
    int (*i2c_write)(uint8_t devAddr, const uint8_t* pData, uint16_t size, uint32_t timeout);
    
    /**
     * @brief I2C üzerinden veri alma fonksiyonu
     * @param devAddr Cihaz adresi (7-bit)
     * @param pData Alınacak veri dizisi
     * @param size Alınacak veri boyutu
     * @param timeout Zaman aşımı süresi (ms)
     * @return İşlem başarılı ise 0, değilse hata kodu
     */
    int (*i2c_read)(uint8_t devAddr, uint8_t* pData, uint16_t size, uint32_t timeout);
    
    /**
     * @brief Bekleme fonksiyonu (ms)
     * @param ms Bekleme süresi (ms)
     */
    void (*delay_ms)(uint32_t ms);
} FT24C02A_IO;

/**
 * @brief FT24C02A EEPROM kütüphanesi için yapılandırma yapısı
 */
typedef struct {
    uint8_t DevAddr;      /**< Cihaz adresi (7-bit) */
    FT24C02A_IO IO;       /**< I2C haberleşme fonksiyonları */
} FT24C02A_HandleTypeDef;

/**
 * @brief FT24C02A EEPROM'a tek bayt veri yazma
 * 
 * Bu fonksiyon, FT24C02A EEPROM'un belirtilen adresine tek bir bayt veri yazar.
 * Yazma işlemi tamamlandıktan sonra, yazma döngüsü süresi (TWC) kadar beklenir.
 * 
 * @param handle FT24C02A yapılandırma yapısı
 * @param memAddr Bellek adresi (0-255)
 * @param data Yazılacak veri
 * @return İşlem durumu
 */
FT24C02A_Status FT24C02A_WriteByte(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, uint8_t data);

/**
 * @brief FT24C02A EEPROM'dan tek bayt veri okuma
 * 
 * Bu fonksiyon, FT24C02A EEPROM'un belirtilen adresinden tek bir bayt veri okur.
 * 
 * @param handle FT24C02A yapılandırma yapısı
 * @param memAddr Bellek adresi (0-255)
 * @param pData Okunan verinin kaydedileceği adres
 * @return İşlem durumu
 */
FT24C02A_Status FT24C02A_ReadByte(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, uint8_t *pData);

/**
 * @brief FT24C02A EEPROM'a çoklu veri yazma
 *
 * @param handle FT24C02A yapılandırma yapısı
 * @param memAddr Başlangıç adresi
 * @param pData Yazılacak veriler
 * @param size Veri boyutu
 * @return İşlem durumu
 */
FT24C02A_Status FT24C02A_WriteBuffer(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, const uint8_t *pData, uint16_t size);

/**
 * @brief FT24C02A EEPROM'dan çoklu veri okuma
 *
 * @param handle FT24C02A yapılandırma yapısı
 * @param memAddr Başlangıç adresi
 * @param pData Okunacak veriler için buffer
 * @param size Okunacak veri boyutu
 * @return İşlem durumu
 */
FT24C02A_Status FT24C02A_ReadBuffer(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, uint8_t *pData, uint16_t size);

/**
 * @brief EEPROM'a bir sayfa veri yaz (max 16 byte)
 * @param handle EEPROM yapılandırma yapısı
 * @param memAddr Başlangıç adresi (sayfa içinde kalmalı)
 * @param pData Yazılacak veriler
 * @param size Veri boyutu (1–16)
 * @return İşlem durumu
 */
FT24C02A_Status FT24C02A_WritePage(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, const uint8_t *pData, uint16_t size);

/**
 * @brief EEPROM'dan bir sayfa veri oku (max 16 byte)
 * @param handle EEPROM yapılandırma yapısı
 * @param memAddr Başlangıç adresi
 * @param pData Okunacak veriler için buffer
 * @param size Okunacak veri boyutu
 * @return İşlem durumu
 */
FT24C02A_Status FT24C02A_ReadPage(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, uint8_t *pData, uint16_t size);

/**
 * @brief EEPROM'a uzun veri yazma işlemini sayfa sayfa gerçekleştirir
 *
 * Bu fonksiyon, EEPROM'un sayfa sınırlarını aşmadan, verilen veriyi
 * başlangıç adresinden başlayarak sayfalara bölerek yazar.
 * Her sayfa maksimum 16 byte veri içerebilir.
 *
 * @param handle    EEPROM yapılandırma yapısı
 * @param startAddr Yazma işleminin başlayacağı başlangıç adresi (0-255)
 * @param pData     Yazılacak veri dizisi
 * @param size      Yazılacak veri boyutu (byte)
 * @return FT24C02A_Status
 *         - FT24C02A_OK: Yazma işlemi başarılı
 *         - FT24C02A_ERROR_PARAM: Geçersiz parametre
 *         - FT24C02A_ERROR_ADDR: Geçersiz bellek adresi (adres + size > maksimum)
 *         - FT24C02A_ERROR_SIZE: Geçersiz veri boyutu (0)
 *         - Diğer hata kodları I2C iletişim sorunlarını belirtir
 */
FT24C02A_Status FT24C02A_WriteLongData(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, const uint8_t *pData, uint16_t size);

/**
 * @brief EEPROM'dan uzun veri okuma işlemini sayfa sayfa gerçekleştirir
 *
 * Bu fonksiyon, EEPROM'dan verilen başlangıç adresinden başlayarak
 * belirlenen boyutta veriyi sayfa sınırlarını aşmadan parçalara bölerek okur.
 * Her sayfa maksimum 16 byte veri içerebilir.
 *
 * @param handle    EEPROM yapılandırma yapısı
 * @param startAddr Okuma işleminin başlayacağı başlangıç adresi (0-255)
 * @param pData     Okunan verilerin kaydedileceği buffer
 * @param size      Okunacak veri boyutu (byte)
 * @return FT24C02A_Status
 *         - FT24C02A_OK: Okuma işlemi başarılı
 *         - FT24C02A_ERROR_PARAM: Geçersiz parametre
 *         - FT24C02A_ERROR_ADDR: Geçersiz bellek adresi (adres + size > maksimum)
 *         - FT24C02A_ERROR_SIZE: Geçersiz veri boyutu (0)
 *         - Diğer hata kodları I2C iletişim sorunlarını belirtir
 */
FT24C02A_Status FT24C02A_ReadLongData(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, uint8_t *pData, uint16_t size);

FT24C02A_Status FT24C02A_Clear(FT24C02A_HandleTypeDef *handle);



#ifdef __cplusplus
}
#endif

#endif /* FT24C02A_EEPROM_H */
