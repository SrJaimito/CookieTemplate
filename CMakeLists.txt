cmake_minimum_required(VERSION 3.22.1)
project(CookieTemplate C ASM)

set(PROJECT_INC_DIRS
    app/inc
)

file(GLOB_RECURSE PROJECT_ASM_FILES app/*.[sS])
file(GLOB_RECURSE PROJECT_SRC_FILES app/*.c)
file(GLOB_RECURSE PROJECT_INC_FILES app/*.h)

set(PROJECT_FILES
    ${PROJECT_ASM_FILES}
    ${PROJECT_SRC_FILES}
    ${PROJECT_INC_FILES}
)

set(HOME /home/srjaimito)

set(SDK_DIR ${HOME}/opt/sdks/gecko-sdk)
set(GECKO_COMMON_INC_DIR ${SDK_DIR}/platform/common/inc)

set(CMSIS_INC_DIR ${SDK_DIR}/platform/CMSIS/Core/Include)

set(DEVICE_DIR ${SDK_DIR}/platform/Device/SiliconLabs/EFR32BG12P)
set(DEVICE_INC_DIR ${DEVICE_DIR}/Include)

set(STARTUP_FILE ${DEVICE_DIR}/Source/GCC/startup_efr32bg12p.S)
set(SYSTEM_FILE ${DEVICE_DIR}/Source/system_efr32bg12p.c)
set(LD_FILE ${DEVICE_DIR}/Source/GCC/efr32bg12p.ld)

set(EMLIB_DIR ${SDK_DIR}/platform/emlib)
set(EMLIB_INC_DIR ${EMLIB_DIR}/inc)
file(GLOB_RECURSE EMLIB_SRC_FILES ${EMLIB_DIR}/src/*.c)

include_directories(
    ${PROJECT_INC_DIRS}
    ${GECKO_COMMON_INC_DIR}
    ${CMSIS_INC_DIR}
    ${DEVICE_INC_DIR}
    ${EMLIB_INC_DIR}
)

set(TOOLCHAIN_DIR ${HOME}/opt/toolchains/gcc-arm-none-eabi-10.3-2021.10/bin)

set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/arm-none-eabi-gcc)
set(OBJCOPY ${TOOLCHAIN_DIR}/arm-none-eabi-objcopy)

set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS S)

set(CMAKE_C_STANDARD 99)

add_compile_definitions(EFR32BG12P EFR32BG12P132F1024GL125)

set(CMAKE_C_FLAGS " \
    -mcpu=cortex-m4 \
    -mfpu=fpv4-sp-d16 \
    -mfloat-abi=hard \
    -O2 \
    -ffunction-sections \
    -fdata-sections \
")

set(CMAKE_EXE_LINKER_FLAGS " \
    -T${LD_FILE} \
    --specs=nosys.specs \
    -Wl,--gc-sections \
")

add_executable(${PROJECT_NAME}.elf
    ${PROJECT_FILES}
    ${STARTUP_FILE}
    ${SYSTEM_FILE}
    ${EMLIB_SRC_FILES}
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
)

set(COMMANDER ${HOME}/opt/commander/commander)

add_custom_target(upload
    DEPENDS commander-cmd
)

add_custom_command(OUTPUT commander-cmd
    COMMAND ${COMMANDER} flash ${PROJECT_NAME}.hex --address 0x0 --device EFR32BG12P132F1024GL125
)
