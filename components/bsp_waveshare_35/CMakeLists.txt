idf_component_register(
    SRCS "bsp_waveshare_35.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_common
    PRIV_REQUIRES esp_hw_support esp_psram esp_system log spi_flash
)
