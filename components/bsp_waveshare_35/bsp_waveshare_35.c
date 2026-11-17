#include "bsp_waveshare_35.h"

#include <inttypes.h>
#include <stdbool.h>

#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "esp_psram.h"
#include "esp_system.h"

static const char *const TAG = "bsp_waveshare_35";
static const uint32_t EXPECTED_FLASH_BYTES = 16 * 1024 * 1024;
static const size_t EXPECTED_PSRAM_BYTES = 8 * 1024 * 1024;

esp_err_t bsp_waveshare_35_check_memory(void)
{
    esp_chip_info_t chip;
    esp_chip_info(&chip);
    ESP_LOGI(TAG, "Target board: Waveshare ESP32-S3-Touch-LCD-3.5, PCB revision unverified");
    ESP_LOGI(TAG, "ESP32-S3 cores=%u, chip_revision=%u, reset_reason=%d", (unsigned)chip.cores,
             (unsigned)chip.revision, (int)esp_reset_reason());

    const bool psram_ready = esp_psram_is_initialized();
    const size_t psram_bytes = psram_ready ? esp_psram_get_size() : 0;
    ESP_LOGI(TAG, "PSRAM detected=%s, bytes=%zu", psram_ready ? "yes" : "no", psram_bytes);

    uint32_t flash_bytes;
    esp_err_t err = esp_flash_get_physical_size(NULL, &flash_bytes);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Flash size query failed: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "Physical flash bytes=%" PRIu32, flash_bytes);
    if (flash_bytes != EXPECTED_FLASH_BYTES || psram_bytes != EXPECTED_PSRAM_BYTES) {
        ESP_LOGE(TAG, "Expected flash=%" PRIu32 " bytes, PSRAM=%zu bytes", EXPECTED_FLASH_BYTES,
                 EXPECTED_PSRAM_BYTES);
        return ESP_ERR_INVALID_SIZE;
    }

    return ESP_OK;
}
