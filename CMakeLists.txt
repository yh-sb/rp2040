cmake_minimum_required(VERSION 3.13)

set(CMAKE_TOOLCHAIN_FILE cmake/toolchain-arm-gcc-cortex.cmake)

get_filename_component(CURRENT_DIRNAME "${CMAKE_CURRENT_LIST_DIR}" NAME)
project(${CURRENT_DIRNAME} C CXX ASM)

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
#add_subdirectory(periph)

# Pico-sdk
if(CMAKE_BUILD_TYPE EQUAL Debug)
    set(PICO_DEOPTIMIZED_DEBUG 1)
endif()
set(PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party/pico-sdk)
include(third_party/pico-sdk/external/pico_sdk_import.cmake)
pico_sdk_init()

add_subdirectory(third_party/FatFs)

set(FREERTOS_PORT GCC_RP2040 CACHE STRING "")
set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "")
add_subdirectory(third_party/FreeRTOS-Kernel)

add_subdirectory(third_party/printf)

#add_subdirectory(utilities/fatfs_diskio)
add_subdirectory(utilities/freertos_wrappers)
add_subdirectory(utilities/syslog)

add_executable(${CMAKE_PROJECT_NAME} main.cpp FreeRTOSHooks.c)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    pico_stdlib
    #drivers
    #periph
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

# Add targets for flashing, erasing, resetting and debugging
set(JLINK_PARAMS -device RP2040_M0_0 -if SWD)
include(cmake/debug-probes/jlink.cmake)
