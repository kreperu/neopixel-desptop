cmake_minimum_required(VERSION 3.13)
set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/../pico-sdk CACHE PATH "Path to the Raspberry Pi Pico SDK" FORCE)
include(../pico-sdk/external/pico_sdk_import.cmake)
project(ctrl C CXX ASM)
set(PICO_BOARD pico_w)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc)
set(CMAKE_CPP_COMPILER /usr/bin/arm-none-eabi-g++)
pico_sdk_init()
add_executable(ctrl
  main.c
  ws2812b.c
  server.c
)
pico_generate_pio_header(ctrl 
  ../pio/pwm.pio
)
pico_enable_stdio_usb(ctrl 1)
pico_enable_stdio_uart(ctrl 0)
pico_add_extra_outputs(ctrl)
#pico_set_binary_type(ctrl no_flash) #for extreme overclock

target_link_libraries(ctrl pico_stdlib hardware_gpio hardware_pio hardware_clocks  pico_cyw43_arch_lwip_threadsafe_background)

target_include_directories(ctrl PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_options(ctrl PRIVATE -Xlinker --print-memory-usage)
target_compile_options(ctrl PRIVATE -Wall -Wextra)