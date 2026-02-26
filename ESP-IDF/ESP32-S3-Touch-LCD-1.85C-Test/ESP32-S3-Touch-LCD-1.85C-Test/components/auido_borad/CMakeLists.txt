if(CONFIG_ESP32_S3_AUDIO_Board)
    list(APPEND BSP_BOARD_SRC "./boards/ESP32_S3_AUDIO_Board")
endif()

idf_component_register(
    SRC_DIRS
        ./
        ${BSP_BOARD_SRC}
    INCLUDE_DIRS
        "include"
        "boards/include"
    REQUIRES
        driver
        fatfs
        spiffs
        )

component_compile_options(-w)
target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
