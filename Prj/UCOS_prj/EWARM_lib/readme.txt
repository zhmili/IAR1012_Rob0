/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : readme.txt
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/08/2007
* Description        : This sub-directory contains all the user-modifiable files 
*                      needed to create a new project linked with the STM32F10x  
*                      Firmware Library and working with IAR Embedded Workbench 
*                      for ARM (EWARM) software toolchain (version 4.42A)
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

Directory contents
===================
- project .ewd/.eww/.ewp: A pre-configured project file with the provided library 
                          structure that produces a debug\release executable image
                          with IAR Embedded Workbench.
                
- stm32f10x_vector.c: This file contains the vector table for STM32F10x.

- lnkarm_flash.xcl: This file is the IAR specific linking and loading file used 
                    to load in Flash and execute code and variables to target 
                    memories and regions. You can customize this file to your need.

- lnkarm_ram.xcl: This file is the IAR specific linking and loading file used to 
                  load in RAM and execute code and variables to target memories 
                  and regions. You can customize this file to your need.
                       
- cortexm3_macro.s: Instruction wrappers for special Cortex-M3 instructions. 

      
How to use it
=============
- Open the Project.eww workspace.
- In the workspace toolbar select the project config:
   - BOOT_FLASH: to load project image into FLASH
   - BOOT_RAM: to load project image into RAM
- Rebuild all files: Project->Rebuild all
- Load project image: Project->Debug
- Run program: Debug->Go(F5)    

******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE******
