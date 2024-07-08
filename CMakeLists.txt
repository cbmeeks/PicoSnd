###############################################################################
#   PicoSnd
#
#   Copyright (c) 2024 Cecil Meeks
#   https://github.com/cbmeeks/PicoSnd
###############################################################################
cmake_minimum_required(VERSION 3.23)

# Set environment variables
# I prefer setting this here instead of in .bashrc or other OS-specific file.  Basically, you just need to make sure
# to set your PICO_SDK_PATH environment variable to the location of the Pico SDK on your system.
set(ENV{PICO_SDK_PATH} "/home/cbmeeks/sdk/pico/pico-sdk")

# Uses the emu2149 library https://github.com/visrealm/emu2149


# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)


# Set the name of the project (as PROJECT_NAME) and C/C++ standards
project(PicoSnd C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

add_executable(PicoSnd
        main.c
        audio.c
        emu2149.c
        vgm.c
        nsf.c
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to libraries (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME}
        pico_multicore
        pico_stdlib
        pico_time_headers
        pico_cxx_options
        hardware_adc
        hardware_dma
        hardware_irq
        hardware_pio
        hardware_pwm
)

# Enable USB output, disable UART output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

