add_library(periph STATIC
    src/gpio.cpp
)

target_include_directories(periph
    PUBLIC
        include
)

target_link_libraries(periph
    PRIVATE
        hardware_gpio
)
