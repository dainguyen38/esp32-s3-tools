#pragma once

#include "esp_err.h"

// Log boot diagnostics and check flash/PSRAM against the board configuration.
esp_err_t bsp_waveshare_35_check_memory(void);
