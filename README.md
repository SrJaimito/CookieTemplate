# CookieTemplate

This is a basic template for working with the Cookie IoT platforms based on EFR32 microcontrollers (or similar Silicon Labs' devices). It provides some simple out-of-the-box functionalities from where to build more complex applications.

At the moment, this template has the following characteristics:

- The build process is configured with CMake, which allows for easy modification of the compilation flags and management of libraries without dealing with complex Makefiles.

- No need of copying large amounts of library and type/macro definition files related to the microcontroller for each new project, since these can be referenced from the CMake configuration file.

- The process of loading the generated binaries into the flash memory can be made directly from the terminal with simple commands defined in the CMake configuration file.

- Provides a simple structure in which the user can keep configuration macros, application code and interrupt handlers in separate files for better organization.

- Application code can be writen directly inside classical `setup` and `loop` functions without worrying about extra configurations carried out in the `main` function.

- The external 38,4 MHz crystal oscillator is configured as the main clock source after the startup sequence, and the default internal 19 MHz RC oscillator is later disabled to save energy.

- The standard `printf` function is retargeted to the default debugging UART for simple communication with the PC through the USB connector or the debugger's interface.

## Requirements

The following is a list of dependencies and tools needed to work with this template.

**Note:** The build process and commands have been tested in Linux only. It should not be hard to port it to other operating systems, but it is not guaranteed to work directly without making additional modifications to the way different paths and commands are defined. 

#### CMake

CMake is needed to generate the Makefiles that will be used later to build the binaries taking into account the desired configurations. In a Debian/Ubuntu-like Linux distribution, the installation of CMake should be as easy as running

    sudo apt install cmake

For other Linux distributions or operating systems, check the corresponding installation alternatives.

At this moment, the minimum required version of CMake is 3.22.1.

#### GNU ARM Embedded Toolchain

The template relies on different tools provided in the GNU ARM Embedded Toolchain, like the `arm-none-eabi-gcc` compiler to build binaries for ARM Cortex-M devices. Though this toolchain is usually available in commonly used repositories, it is recommended to download it directly from the [ARM developer site](https://developer.arm.com/downloads/-/gnu-rm) for more up-to-date versions.

In particular, the last version tested with this template is 10.3-2021.10.

Once the toolchain has been downloaded, if you want it to work with this template without doing several modifications, you can extract it from the compressed file and rename the root folder to `gcc-arm-none-eabi.10.3-2021.10` and place it in your home directory, inside a `~/opt/toolchains` folder. You should end up with the following folder structure:

    ~/opt
    └─── toolchains
         └─── gcc-arm-none-eabi.10.3-2021.10
              └─── arm-none-eabi
              └─── bin
              |        ...
              |        arm-none-eabi-gcc
              |        ...
              └─── lib
              └─── share

Placing the toolchain in a different location is possible, but then the corresponding paths in the `CMakeLists.txt` configuration file must be properly updated.

#### Silicon Labs Gecko SDK

The Gecko SDK contains different files needed to work with compatible Silicon Labs' devices. Among other things, it provides header and source files for working with CMSIS, higher-level abstraction layers of the hardware, and default startup code and symbol definitions for different microcontrollers.

The SDK can be cloned directly from its GitHub repository or downloaded from the corresponding [releases page](https://github.com/SiliconLabs/gecko_sdk/releases). This template has been tested with version 4.1.6.

Once downloaded, the path to the SDK in the `CMakeLists.txt` file is expected to be `~/opt/sdks/gecko-sdk`, so you should have a folder structure like the following:

    ~/opt
    └─── sdks
         └─── gecko-sdk
              └─── app
              |    ...
              └─── platform
              |    ...
              └─── util

Again, you can rename the root folders of the SDK as you want, or place it in any other location, while modifying the paths defined in the `CMakeLists.txt` file accordingly.

#### Simplicity Commander and J-Link

Simplicity Commander is a tool provided by Silicon Labs that can be used to flash binaries into their devices. It can be downloaded from the *Standalone Installation* section [here](https://community.silabs.com/s/article/simplicity-commander?language=en_US).

Though it provides a graphical user interface to work with it, the `CMakeLists.txt` file provided in this template defines an additional target that allows to quickly flash the generated binary via the command line. To do so, it expects to find the Simplicity Commander executable in `~/opt/commander/commander`. If you want to place it in other location, you must update the corresponding path in `CMakeLists.txt`.

Simplicity Commander will use the J-Link software under the hood, and this one must be installed separately. The recommended option is to simply download the corresponding DEB installer [here](https://www.segger.com/downloads/jlink/) and run

    sudo dpkg -i [path to J-Link DEB file]

Check your alternatives if your Linux distribution or operating system cannot perform this installation process through DEB files.
