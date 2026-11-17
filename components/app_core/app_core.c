#include "app_core.h"

#include "bsp_waveshare_35.h"
#include "esp_app_desc.h"
#include "esp_log.h"

static const char *const TAG = "app_core";

void app_core_start(void)
{
    const esp_app_desc_t *app = esp_app_get_description();
    ESP_LOGI(TAG, "%s %s, ESP-IDF %s", app->project_name, app->version, app->idf_ver);

    esp_err_t err = bsp_waveshare_35_check_memory();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "BOOT_SMOKE_FAIL: %s", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(TAG, "BOOT_SMOKE_PASS");
}
