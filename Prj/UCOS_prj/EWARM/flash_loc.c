#include"includes.h"

void vector_loc()
{
#ifdef  VECT_TAB_RAM  
    // Set the Vector Table base location at 0x20000000 
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  
    // Set the Vector Table base location at 0x08040000  
    //修正过来了
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);   
#endif
}
