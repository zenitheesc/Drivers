<h1 align="center" style="color:white; background-color:black">Drivers</h1>
<h4 align="center">Collection of Low-Level Drivers and Documentation</h4>

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
    <a href="#adding-a-driver">Adding a Driver</a>
</p>

## Objective

Unify all drivers written by the group adding proper documentation.

## Environment

All our functions are based on the STM32's HAL drivers. The use of the [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) is highly recommended because it easies the process of peripheral configuration, code generation, code compilation, and debug. 

We prefer to develop drivers in C, although the STM32CubeIDE supports C++ projects. 

## Adding a Driver

 1. Clone this repository
 2. Create a folder with the Driver name
 3. Add the `Src` and `Inc` with the `.c` and `.h` 
 4. Add a `README.md` file to the Driver folder 
    1. On the README add the Author, Date, and purpose of the library
    2. Add Documentation: Parameters and purpose of each function
    3. Add a Notes Section for any extra information of the IC
 5. Add a folder called `Docs`
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
