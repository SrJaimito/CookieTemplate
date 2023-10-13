# CookieTemplate

This is a basic template for working with the Cookie IoT platforms based on EFR32 microcontrollers (or similar Silicon Labs' devices). It provides some simple out-of-the-box functionalities from where to build more complex applications.

At the moment, this template has the following characteristics:

- The build process is configured with CMake, which allows for easy modification of the compilation flags and management of libraries without dealing with complex Makefiles.

- No need of copying large amounts of library and type/macro definition files related to the microcontroller for each new project, since these can be referenced from the CMake configuration file.

- The process of loading the generated binaries into the flash memory can be made directly from the terminal with simple commands defined in the CMake configuration file.

- Provides a simple structure in which the user can keep configuration macros, application code and interrupt handlers in separate files for better organization.

- Application code can be written directly inside classical `setup` and `loop` functions without worrying about extra configurations carried out in the `main` function.

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

## Using the template

Starting a project with this template is as simple as cloning this repository. For example, is you want your project to be located in a folder called `BlinkLED`, you can run

    git clone https://github.com/SrJaimito/CookieTemplate.git BlinkLED

Inside your project folder, you will find the following relevant items:

- `build` folder: where you will have all the Makefiles generated by CMake and build all your binaries.

- `app` folder: where you write your application code for the microcontroller.

- `CMakeLists.txt` file: where you write the CMake configuration.

### Writing your application code

The `app` folder has a predefined structure that can be used to organize your code. It also contains some code that performs certain configurations in the microcontroller.

The idea is that your application header and source files are located in the `inc` and `src` subdirectories of the `app` folder, though this can be customizable. These subdirectories already contain some predefined files with a given purpose:

- `app_config.h` is a general purpose header file in which you can define custom types and definitions that you plan to reuse through the entire application. For example, you could define here macros for your input/output pins or peripherals, like

```c
    #define RED_LED_PIN   10
    #define GREEN_LED_PIN 11

    #define TEMPERATURE_SENSOR_SPI USART2
```

- `app.h` contains the declarations for the `setup` and `loop` functions called from the `main` function, so there is no need of touching this file at all except for a certain situation in which you may want to add some information as it will be explained later.

- `app.c` contains the definitions of the `setup` and `loop` functions that you can fill with your application code.

    + The `loop` function is called inside an infinite loop from the `main` function, and usually carries out the entire flow and logic of the application.

    + The `setup` function is called before the `loop` function only once. You can use this function to write code for any configuration of the microcontroller or execute actions before entering to the main application loop.

- `interrupt.c` has the purpose of separating the code for handling interruptions from the main application code, keeping all the interruption request handlers organized in a dedicated file.

Separation of interrupt handlers from the main code helps keeping the code organized and clean. However, there is a price to pay for this, and it is that we cannot directly share variables between interruption handlers and your main loop (for example, updating some flags when the interruption executes that will later affect the loop's execution flow).

To bypass this caveat, you can declare your shared variables in the global scope of `app.c`, and then externalize them through `app.h` by writing

    extern int my_flag;

Then, including `app.h` in  `interrupt.c` will allow you to access these variables inside your interruption handlers.

### Building your application

Building your application should be more or less straightforward, specially if you prepared all the tools in the same way as in the [requirements section](#requirements). In case you chose different paths, remember to update `CMakeLists.txt` accordingly.

Before building anything, you will have to do at least two things in the `CMakeLists.txt` file:

- Specify your project name. By default, it is set to `CookieTemplate`, so look for it and change it to whatever you want, like

```
    project(BlinkLED)
```
- Specify your home directory. This is because the paths to the external tools and libraries are set by default relative to the home directory `~` (assuming you created the same folder structure in the [requirements section](#requirements)). Thus, you will have to look for the `HOME` variable in `CMakeLists.txt` file and update it in a way similar to

```
    set(HOME /home/[username])
```

Now you can move into the `build` folder, generate the Makefiles with CMake and compile your application

    cd build
    cmake ..
    make

Now you should see that the `build` folder has a `.hex` with the name of the project that you can flash into the device. To do so automatically, run

    make upload


