idf_component_register(
    SRCS "app_core.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES bsp_waveshare_35 esp_app_format log
)
