cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE cmake/toolchain-arm-gcc-cortex.cmake)

project(rp2040 C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(
    -mcpu=cortex-m0plus
    -mthumb
    -ffunction-sections
    -fdata-sections
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
)

#add_subdirectory(drivers)
add_subdirectory(periph)

# Pico-sdk
set(PICO_DEOPTIMIZED_DEBUG $<CONFIG:Debug>)
set(PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party/pico-sdk)
include(third_party/pico-sdk/external/pico_sdk_import.cmake)
pico_sdk_init()

add_subdirectory(third_party/FatFs)

set(FREERTOS_PORT GCC_RP2040 CACHE STRING "")
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE .)
add_subdirectory(third_party/FreeRTOS-Kernel)

add_subdirectory(third_party/printf)

#add_subdirectory(utilities/fatfs_diskio)
add_subdirectory(utilities/freertos_wrappers)
add_subdirectory(utilities/syslog)

add_executable(${CMAKE_PROJECT_NAME} main.cpp FreeRTOSHooks.c)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    pico_stdlib
    #drivers
    periph
    #fatfs_diskio
    freertos_kernel
    fatfs
    printf
    syslog
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -mcpu=cortex-m0plus
    -Wl,--gc-sections
    -specs=nano.specs
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map,--cref
)

set(CMAKE_EXECUTABLE_SUFFIX .elf)
# Generate .uf2, .elf using pico-sdk
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX ".elf")
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} ${CMAKE_PROJECT_NAME}.elf
)

# Flash .uf2 file to RPI-RP2 disk using RP2040 bootloader.
# If you have JLink connected. Use "flash" target from cmake/debug-probes/jlink.cmake
add_custom_target(flash2
    COMMENT "Copy .uf2 file to the bootloader RPI-RP2 disk"
    COMMAND powershell -Command "$rp2040Drive = (Get-WmiObject -Query 'Select * From Win32_LogicalDisk Where VolumeName=''RPI-RP2''').DeviceID; if (-not $rp2040Drive) { Write-Error 'RPI-RP2 disk not found'; exit 1; }; Copy-Item -Path ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.uf2 -Destination $rp2040Drive"
    VERBATIM USES_TERMINAL
)

# Add targets for flashing, erasing, resetting and debugging
set(JLINK_PARAMS -device RP2040_M0_0 -if SWD)
include(cmake/debug-probes/jlink.cmake)
