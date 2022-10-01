<h1 align="center" style="color:white; background-color:black">Drivers</h1>
<h4 align="center">Centralized repository for the low level drivers developed by Zenith Aerospace</h4>

<p align="center">
	<a href="http://zenith.eesc.usp.br/">
    <img src="https://img.shields.io/badge/Zenith-Embarcados-black?style=for-the-badge"/>
    </a>
    <a href="https://eesc.usp.br/">
    <img src="https://img.shields.io/badge/Linked%20to-EESC--USP-black?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Drivers/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/zenitheesc/Drivers?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Drivers/issues">
    <img src="https://img.shields.io/github/issues/zenitheesc/Drivers?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Drivers/commits/main">
    <img src="https://img.shields.io/github/commit-activity/m/zenitheesc/Drivers?style=for-the-badge">
    </a>
    <a href="https://github.com/zenitheesc/Drivers/graphs/contributors">
    <img src="https://img.shields.io/github/contributors/zenitheesc/Drivers?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Drivers/commits/main">
    <img src="https://img.shields.io/github/last-commit/zenitheesc/Drivers?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Drivers/issues">
    <img src="https://img.shields.io/github/issues-raw/zenitheesc/Drivers?style=for-the-badge" />
    </a>
    <a href="https://github.com/zenitheesc/Drivers/pulls">
    <img src = "https://img.shields.io/github/issues-pr-raw/zenitheesc/Drivers?style=for-the-badge">
    </a>
</p>

<p align="center">
    <a href="#objective">Objective</a> •
    <a href="#environment">Environment</a> •
    <a href="#how-to-use-a-driver">How to use a Driver</a> •
    <a href="#adding-a-driver">Adding a Driver</a>
</p>

## Objective

Store and manage developed drivers.

## How to use a Driver
Each driver is organized in its own folder, where you'll find the `.c/.h` pair. Just add the files to your project, for example in a STM32CubeIDE project add the `.c` file to the `Core/Src` folder and the `.h` to the `Core/Inc`. However all drivers use the `platform` "framework", so you also need to copy the whole `platform` folder to your project as well, in STM32CubeIDE add it to the `Core/Inc` folder. All drivers should have an example of how to use it. 

### Platform
The platform framework is a set of macros and header-only libraries that 
drivers use instead of platform specific functions. For example, an I2C transaction in a STM32 project calls the ST HAL library, that means that the driver (as it is) can only be used in STM32 microcontrollers. The job of the platform is to detect in which microcontroller it is being compiled to and then choose the appropriate library, so that a driver developed to the platform can be used in other microcontrollers/processors.

#### How it works?
First the `platform.h` file sets up the common interface to the driver, common `structs` and function signatures. Then a bunch of preprocessor directives try to guess what type of project this is and include the correct platform implementation (header-only library) in the `arch` folder.

FYI, if platform can't find an implementation it defaults to a PC debug mode, where an implementation that just prints to stdout is used.

## Environment

Realistically, we mostly use the STM32CubeIDE as most of the projects we do are based on that platform. But with the platform framework you should just need a C compiler.

## Adding a Driver
Guidelines for drivers:
 - You should use C unless otherwise necessary
   - Why: to maximize compatibility (yes there are still uCs that can only use C)
 - You should add some documentation with an example
   - Why: Most users just want to copy and paste correct code
 - You should use platform functions to access hardware resources, unless necessary
   - Why: maximize compatibility, however platform doesn't cover everything so ,for now, you can add a driver that is platform specific.  

Steps in a list:
 1. Clone this repository
 2. Create a folder with the Driver name
 3. Add the `.c` and `.h` 
 4. Add a `README.md` file to the Driver folder 
    1. On the README add the Author, Date, and purpose of the library
    2. Add Documentation: Parameters and purpose of each function
    3. Add a Notes Section for any extra information of the IC
 5. (Optional) Add a folder called `Docs`
    1. Add any External Documentation including Datasheets, Application Notes, and Register Maps
 6. If there is a GitHub Issue for the Driver add a message or close it.

---

<p align="center">
    <a href="http://zenith.eesc.usp.br">
    <img src="https://img.shields.io/badge/Check%20out-Zenith's Oficial Website-black?style=for-the-badge" />
    </a> 
    <a href="https://www.facebook.com/zenitheesc">
    <img src="https://img.shields.io/badge/Like%20us%20on-facebook-blue?style=for-the-badge"/>
    </a> 
    <a href="https://www.instagram.com/zenith_eesc/">
    <img src="https://img.shields.io/badge/Follow%20us%20on-Instagram-red?style=for-the-badge"/>
    </a>

</p>
<p align = "center">
<a href="zenith.eesc@gmail.com">zenith.eesc@gmail.com</a>
</p>
