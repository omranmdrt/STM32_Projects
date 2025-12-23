/**

 * Bu kütüphane, FT24C02A-usr-b EEPROM ile I2C protokolü üzerinden
 * haberleşmek için gerekli fonksiyonları içerir.
 */

#include "ft24c02a_eeprom.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief FT24C02A EEPROM'a tek bayt veri yazma
 * 
 * Bu fonksiyon, FT24C02A EEPROM'un belirtilen adresine tek bir bayt veri yazar.
 * Yazma işlemi tamamlandıktan sonra, yazma döngüsü süresi (TWC) kadar beklenir.
 * 
 * I2C protokolü üzerinden yazma işlemi şu adımları içerir:
 * 1. START koşulu
 * 2. Cihaz adresi + Yazma biti (0)
 * 3. Bellek adresi
 * 4. Yazılacak veri
 * 5. STOP koşulu
 * 6. Yazma döngüsü süresi kadar bekleme
 * 
 * @param handle FT24C02A yapılandırma yapısı
 * @param memAddr Bellek adresi (0-255)
 * @param data Yazılacak veri
 * @return İşlem durumu
 */
FT24C02A_Status FT24C02A_WriteByte(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, uint8_t data)
{
    /* Parametre kontrolü */
    if (handle == NULL || handle->IO.i2c_write == NULL || handle->IO.delay_ms == NULL) {
        return FT24C02A_ERROR_PARAM;
    }
    
    /* Bellek adresi kontrolü */
    if (memAddr >= FT24C02A_MAX_SIZE) {
        return FT24C02A_ERROR_ADDR;
    }
    
    /* Veri buffer'ı hazırlama */
    uint8_t buffer[2];
    buffer[0] = memAddr;  /* Bellek adresi */
    buffer[1] = data;     /* Yazılacak veri */
    
    /* I2C üzerinden veriyi gönderme */
    int status = handle->IO.i2c_write(handle->DevAddr, buffer, 2, 100);
    if (status != 0) {
        return FT24C02A_ERROR_I2C;
    }
    
    /* Yazma döngüsü süresi kadar bekleme */
    handle->IO.delay_ms(FT24C02A_WRITE_CYCLE_TIME);
    
    return FT24C02A_OK;
}

/**
 * @brief FT24C02A EEPROM'dan tek bayt veri okuma
 * 
 * Bu fonksiyon, FT24C02A EEPROM'un belirtilen adresinden tek bir bayt veri okur.
 * 
 * I2C protokolü üzerinden okuma işlemi şu adımları içerir:
 * 1. START koşulu
 * 2. Cihaz adresi + Yazma biti (0)
 * 3. Bellek adresi
 * 4. Yeniden START koşulu
 * 5. Cihaz adresi + Okuma biti (1)
 * 6. Veri okuma
 * 7. STOP koşulu
 * 
 * @param handle FT24C02A yapılandırma yapısı
 * @param memAddr Bellek adresi (0-255)
 * @param pData Okunan verinin kaydedileceği adres
 * @return İşlem durumu
 */
FT24C02A_Status FT24C02A_ReadByte(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, uint8_t *pData)
{
    /* Parametre kontrolü */
    if (handle == NULL || handle->IO.i2c_write == NULL || 
        handle->IO.i2c_read == NULL || pData == NULL) {
        return FT24C02A_ERROR_PARAM;
    }
    
    /* Bellek adresi kontrolü */
    if (memAddr >= FT24C02A_MAX_SIZE) {
        return FT24C02A_ERROR_ADDR;
    }
    
    /* Önce bellek adresini gönder */
    int status = handle->IO.i2c_write(handle->DevAddr, &memAddr, 1, 100);
    if (status != 0) {
        return FT24C02A_ERROR_I2C;
    }
    
    /* Sonra veriyi oku */
    status = handle->IO.i2c_read(handle->DevAddr, pData, 1, 100);
    if (status != 0) {
        return FT24C02A_ERROR_I2C;
    }
    
    return FT24C02A_OK;
}


// çoklu veri yazma

/**
 * @brief FT24C02A EEPROM'a ardışık birden fazla veri yazma
 *
 * Bu fonksiyon, FT24C02A EEPROM'un belirtilen başlangıç adresinden itibaren
 * sıralı bir şekilde tek tek bayt yazar.
 *
 * Her bayt için `FT24C02A_WriteByte()` fonksiyonu çağrılır ve yazma döngüsü beklenir.
 * Sayfa sınırlarına dikkat etmeden her bayt ayrı yazılır.
 *
 * @param handle FT24C02A yapılandırma yapısı
 * @param memAddr Başlangıç adresi (0–255)
 * @param pData Yazılacak verilerin adresi
 * @param size Yazılacak veri boyutu
 * @return İşlem durumu
 */

FT24C02A_Status FT24C02A_WriteBuffer(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, const uint8_t *pData, uint16_t size)
{
    if (handle == NULL || handle->IO.i2c_write == NULL || handle->IO.delay_ms == NULL || pData == NULL) {
        return FT24C02A_ERROR_PARAM;
    }

    if ((memAddr + size) > FT24C02A_MAX_SIZE) {
        return FT24C02A_ERROR_SIZE;
    }

    for (uint16_t i = 0; i < size; i++) {
        FT24C02A_Status status = FT24C02A_WriteByte(handle, memAddr + i, pData[i]);
        if (status != FT24C02A_OK) {
            return status;
        }
    }

    return FT24C02A_OK;
}

//çoklu veri okuma

/**
 * @brief FT24C02A EEPROM'dan ardışık birden fazla veri okuma
 *
 * Bu fonksiyon, EEPROM'dan belirtilen başlangıç adresinden başlayarak,
 * her adresten bir bayt okuyarak `size` kadar veri çeker.
 *
 * Her bayt için `FT24C02A_ReadByte()` çağrılır.
 *
 * @param handle FT24C02A yapılandırma yapısı
 * @param memAddr Başlangıç adresi (0–255)
 * @param pData Okunan verilerin kaydedileceği tampon
 * @param size Okunacak veri boyutu
 * @return İşlem durumu
 */

FT24C02A_Status FT24C02A_ReadBuffer(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, uint8_t *pData, uint16_t size)
{
    if (handle == NULL || handle->IO.i2c_write == NULL || handle->IO.i2c_read == NULL || pData == NULL) {
        return FT24C02A_ERROR_PARAM;
    }

    if ((memAddr + size) > FT24C02A_MAX_SIZE) {
        return FT24C02A_ERROR_SIZE;
    }

    for (uint16_t i = 0; i < size; i++) {
        FT24C02A_Status status = FT24C02A_ReadByte(handle, memAddr + i, &pData[i]);
        if (status != FT24C02A_OK) {
            return status;
        }
    }

    return FT24C02A_OK;
}

//EEPROM'a bir sayfa veri yaz (max 16 byte)
/**
 * @brief EEPROM'a bir sayfa veri yaz (max 16 byte)
 *
 * Bu fonksiyon, FT24C02A EEPROM'un bir sayfası içinde kalan veri bloğunu tek seferde yazar.
 * Sayfa sınırlarını aşamaz. 1–16 bayt arası veri yazılabilir.
 *
 * I2C protokolü ile:
 * 1. START
 * 2. Cihaz adresi + yazma biti
 * 3. Başlangıç bellek adresi
 * 4. Veri bloğu (max 16 byte)
 * 5. STOP
 *
 * @param handle EEPROM yapılandırma yapısı
 * @param memAddr Başlangıç adresi (sayfa içinde kalmalı)
 * @param pData Yazılacak veriler
 * @param size Veri boyutu (1–16)
 * @return İşlem durumu
 */


FT24C02A_Status FT24C02A_WritePage(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, const uint8_t *pData, uint16_t size)
{
    if (handle == NULL || handle->IO.i2c_write == NULL || handle->IO.delay_ms == NULL || pData == NULL) {
        return FT24C02A_ERROR_PARAM;
    }

    if (size == 0 || size > FT24C02A_PAGE_SIZE) {
        return FT24C02A_ERROR_SIZE;
    }

    if ((memAddr + size) > FT24C02A_MAX_SIZE) {
        return FT24C02A_ERROR_ADDR;
    }

    // Sayfa sınırını geçiyor mu kontrol et
    uint8_t page_start = memAddr & ~(FT24C02A_PAGE_SIZE - 1);  // Sayfa başlangıcı
    if ((memAddr + size - 1) >= (page_start + FT24C02A_PAGE_SIZE)) {
        return FT24C02A_ERROR_PARAM; // Sayfa sınırını aşmak yasak
    }

    uint8_t buffer[FT24C02A_PAGE_SIZE + 1];
    buffer[0] = memAddr;
    for (uint16_t i = 0; i < size; i++) {
        buffer[i + 1] = pData[i];
    }

    int status = handle->IO.i2c_write(handle->DevAddr, buffer, size + 1, 100);
    if (status != 0) {
        return FT24C02A_ERROR_I2C;
    }

    handle->IO.delay_ms(FT24C02A_WRITE_CYCLE_TIME);
    return FT24C02A_OK;
}

// EEPROM'dan bir sayfa veri oku (max 16 byte)
/**
 * @brief EEPROM'dan bir sayfa veri oku (max 16 byte)
 *
 * Bu fonksiyon, EEPROM'dan belirtilen adresten başlayarak `size` kadar veriyi tek seferde okur.
 *
 * I2C işlemi:
 * 1. START
 * 2. Cihaz adresi + yazma biti
 * 3. Bellek adresi
 * 4. START (restart)
 * 5. Cihaz adresi + okuma biti
 * 6. Veri bloğunu al
 * 7. STOP
 *
 * @param handle EEPROM yapılandırma yapısı
 * @param memAddr Başlangıç adresi
 * @param pData Okunan verilerin kaydedileceği tampon
 * @param size Okunacak veri miktarı (1–16 byte önerilir)
 * @return İşlem durumu
 */

FT24C02A_Status FT24C02A_ReadPage(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, uint8_t *pData, uint16_t size)
{
    if (handle == NULL || handle->IO.i2c_write == NULL || handle->IO.i2c_read == NULL || pData == NULL) {
        return FT24C02A_ERROR_PARAM;
    }

    if ((memAddr + size) > FT24C02A_MAX_SIZE) {
        return FT24C02A_ERROR_ADDR;
    }

    int status = handle->IO.i2c_write(handle->DevAddr, &memAddr, 1, 100);
    if (status != 0) {
        return FT24C02A_ERROR_I2C;
    }

    status = handle->IO.i2c_read(handle->DevAddr, pData, size, 100);
    if (status != 0) {
        return FT24C02A_ERROR_I2C;
    }

    return FT24C02A_OK;
}

/**
 * @brief EEPROM'a çok uzun veri yaz (birden fazla sayfa)
 *
 * Bu fonksiyon, büyük boyutlu veriyi EEPROM’a yazmak için tasarlanmıştır.
 * Veriyi parçalara ayırarak her bir parçayı `FT24C02A_WritePage()` ile yazar.
 * Sayfa sınırına göre veri kesilir ve bir sonraki sayfaya devam eder.
 *
 * @param handle EEPROM yapılandırma yapısı
 * @param memAddr Başlangıç adresi
 * @param pData Yazılacak verinin adresi
 * @param size Yazılacak toplam veri boyutu
 * @return İşlem durumu
 */

FT24C02A_Status FT24C02A_WriteLongData(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, const uint8_t *pData, uint16_t size)
{
    if (!handle || !handle->IO.i2c_write || !handle->IO.delay_ms || !pData)
        return FT24C02A_ERROR_PARAM;
    if ((memAddr + size) > FT24C02A_MAX_SIZE)
        return FT24C02A_ERROR_ADDR;
    if (size == 0)
        return FT24C02A_ERROR_SIZE;

    size_t written = 0;
    while (written < size) {
        // Sayfa içinde kalan boş alan
        uint8_t pageOffset = (memAddr + written) % FT24C02A_PAGE_SIZE;
        uint8_t spaceInPage = FT24C02A_PAGE_SIZE - pageOffset;

        // Yazılacak byte sayısı (sayfa sınırına dikkat)
        size_t chunkSize = (size - written) < spaceInPage ? (size - written) : spaceInPage;

        FT24C02A_Status status = FT24C02A_WritePage(handle, memAddr + written, &pData[written], chunkSize);
        if (status != FT24C02A_OK)
            return status;

        written += chunkSize;
    }

    return FT24C02A_OK;
}

/**
 * @brief EEPROM'dan çok uzun veri oku (birden fazla sayfa)
 *
 * Bu fonksiyon, EEPROM’dan büyük veri blokları okumak için kullanılır.
 * Veriler sayfa sayfa `FT24C02A_ReadPage()` fonksiyonu kullanılarak okunur.
 *
 * @param handle EEPROM yapılandırma yapısı
 * @param memAddr Başlangıç adresi
 * @param pData Okunan verinin yazılacağı tampon
 * @param size Okunacak toplam veri boyutu
 * @return İşlem durumu
 */

FT24C02A_Status FT24C02A_ReadLongData(FT24C02A_HandleTypeDef *handle, uint8_t memAddr, uint8_t *pData, uint16_t size)
{
    if (!handle || !handle->IO.i2c_read || !pData)
        return FT24C02A_ERROR_PARAM;
    if ((memAddr + size) > FT24C02A_MAX_SIZE)
        return FT24C02A_ERROR_ADDR;
    if (size == 0)
        return FT24C02A_ERROR_SIZE;

    size_t read = 0;
    while (read < size) {
        uint8_t pageOffset = (memAddr + read) % FT24C02A_PAGE_SIZE;
        uint8_t spaceInPage = FT24C02A_PAGE_SIZE - pageOffset;

        size_t chunkSize = (size - read) < spaceInPage ? (size - read) : spaceInPage;

        FT24C02A_Status status = FT24C02A_ReadPage(handle, memAddr + read, &pData[read], chunkSize);
        if (status != FT24C02A_OK)
            return status;

        read += chunkSize;
    }

    return FT24C02A_OK;
}

FT24C02A_Status FT24C02A_Clear(FT24C02A_HandleTypeDef *handle)
{
    uint8_t bos_veri[8];
    memset(bos_veri, 0xFF, sizeof(bos_veri));  // EEPROM silinmiş hali genelde 0xFF'tir

    for (uint16_t addr = 0; addr < FT24C02A_MAX_SIZE; addr += sizeof(bos_veri))
    {
        if (FT24C02A_WritePage(handle, addr, bos_veri, sizeof(bos_veri)) != FT24C02A_OK)
        {
            printf("Silme hatası, adres: %d\r\n", addr);
            return FT24C02A_ERROR;
        }
    }

    printf("EEPROM başarıyla temizlendi.\r\n");
    return FT24C02A_OK;
}

