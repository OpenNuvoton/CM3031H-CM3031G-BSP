# CM3031 Series CMSIS BSP

To experience the powerful features of CM3031 in few minutes, please select the sample code of your interest to download and execute on the CM3031 board. You can open the project files to build them with Keil® MDK, IAR or VSCode, and then download and trace them on the CM3031 board to see how it works.

## .\Document\

- CMSIS.html<br>
	Introduction of CMSIS version 6.1.0 CMSIS components included CMSIS-CORE, CMSIS-Driver, CMSIS-DSP, etc.

- NuMicro CM3031 Series CMSIS BSP Revision History.pdf<br>
	The revision history of CM3031 Series BSP.

- NuMicro CM3031 Series Driver Reference Guide.chm<br>
	The usage of drivers in CM3031 Series BSP.

## .\Library\

- CMSIS<br>
	Cortex® Microcontroller Software Interface Standard (CMSIS) V5.9.0 definitions by ARM® Corp.

- CMSIS-DSP<br>
	CMSIS DSP Library.
	
- Device<br>
	CMSIS compliant device header file.

- LlsiYcableLib<br>
	LLSI library source code.

- NuMaker<br>
	Specific libraries for CM3031 NuMaker board.

- StdDriver<br>
	All peripheral driver header and source files.

- UsbHostLib<br>
	USB host library source code.

## .\Sample Code\

- FreeRTOS<br>
	Simple FreeRTOS™ demo code.

- Hard\_Fault\_Sample<br>
	Show hard fault information when hard fault happened. The hard fault handler show some information included program counter, which is the address where the processor was executing when the hard fault occur. The listing file (or map file) can show what function and instruction that was. It also shows the Link Register (LR), which contains the return address of the last function call. It can show the status where CPU comes from to get to this point.

- ISP<br>
	Sample codes for In-System-Programming.
	
- NuMaker-CM3034HSv2<br>
	Sample codes for NuMaker-M3034HSv2 board.

- PowerManagement<br>
	Power management sample code.

- StdDriver<br>
	Demonstrate the usage of CM3031 series MCU peripheral driver APIs.

- Template<br>
	A template project file for CM3031 series.

- TrustZone<br>
	Includes the demo of secure codes and non-secure codes.

- XOM<br>
	Demonstrate how to create XOM library and use it.

## .\ThirdParty\

- FatFs<br>
	An open source FAT/exFAT filesystem library.

- FreeRTOS<br>
	A real time operating system available for free download.

- LibMAD<br>
	A MPEG audio decoder library that currently supports MPEG-1 and the MPEG-2 extension to lower sampling frequencies, as well as the de facto MPEG 2.5 format.

- emWin_light<br>
	A lightweight embedded graphics library optimized for resource-constrained systems.	An open source FAT/exFAT filesystem library.	

- LVGL<br>	
	An open-source graphics library for embedded systems, providing modern GUI components and touch support.
	
- SGL<br>	
A lightweight graphics library designed for embedded GUI and dashboard applications.

# License

**SPDX-License-Identifier: Apache-2.0**

Copyright in some of the content available in this BSP belongs to third parties.
Third parties license is specified in a file header or license file.
CM3031 BSP files are provided under the Apache-2.0 license.
