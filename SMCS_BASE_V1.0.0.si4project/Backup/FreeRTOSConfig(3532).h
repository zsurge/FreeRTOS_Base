/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */

//’Î∂‘≤ªÕ¨µƒ±‡“Î∆˜µ˜”√≤ªÕ¨µƒstdint.hŒƒº˛
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
    extern uint32_t SystemCoreClock;
#endif

//∂œ—‘
//#define vAssertCalled(char,int) printf("Error:%s,%d\r\n",char,int)
//#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__)
	
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }	

/***************************************************************************************************************/
/*                                        FreeRTOSª˘¥°≈‰÷√≈‰÷√—°œÓ                                              */
/***************************************************************************************************************/
#define configUSE_PREEMPTION			1   		//1 π”√«¿’º Ωƒ⁄∫À£¨0 π”√–≠≥Ã
#define configCPU_CLOCK_HZ				( SystemCoreClock ) 		//CPU∆µ¬ 
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 ) 		// ±÷”Ω⁄≈ƒ∆µ¬ £¨’‚¿Ô…Ë÷√Œ™1000£¨÷‹∆⁄æÕ «1ms
#define configMAX_PRIORITIES			( 5 )		//ø… π”√µƒ◊Ó¥Û”≈œ»º∂
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 130 )		//ø’œ–»ŒŒÒ π”√µƒ∂—’ª¥Û–°
#define configMAX_TASK_NAME_LEN			( 10 )	//»ŒŒÒ√˚◊÷◊÷∑˚¥Æ≥§∂»

#define configUSE_16_BIT_TICKS			0		//œµÕ≥Ω⁄≈ƒº∆ ˝∆˜±‰¡ø ˝æ›¿‡–Õ£¨1±Ì æŒ™16ŒªŒﬁ∑˚∫≈’˚–Œ£¨0±Ì æŒ™32ŒªŒﬁ∑˚∫≈’˚–Œ
#define configIDLE_SHOULD_YIELD			1		//Œ™1 ±ø’œ–»ŒŒÒ∑≈∆˙CPU π”√»®∏¯∆‰À˚Õ¨”≈œ»º∂µƒ”√ªß»ŒŒÒ
#define configUSE_MUTEXES				1				 //Œ™1 ± π”√ª•≥‚–≈∫≈¡ø
#define configQUEUE_REGISTRY_SIZE		8			//≤ªŒ™0 ±±Ì æ∆Ù”√∂”¡–º«¬º£¨æﬂÃÂµƒ÷µ «ø…“‘º«¬ºµƒ∂”¡–∫Õ–≈∫≈¡ø◊Ó¥Û ˝ƒø°£
#define configCHECK_FOR_STACK_OVERFLOW	0		//¥Û”⁄0 ±∆Ù”√∂—’ª“Á≥ˆºÏ≤‚π¶ƒ‹£¨»Áπ˚ π”√¥Àπ¶ƒ‹”√ªß±ÿ–ÎÃ·π©“ª∏ˆ’ª“Á≥ˆπ≥◊”∫Ø ˝£¨»Áπ˚ π”√µƒª∞¥À÷µø…“‘Œ™1ªÚ’ﬂ2£¨“ÚŒ™”–¡Ω÷÷’ª“Á≥ˆºÏ≤‚∑Ω∑®°£
#define configUSE_RECURSIVE_MUTEXES		1			//Œ™1 ± π”√µ›πÈª•≥‚–≈∫≈¡ø

#define configUSE_APPLICATION_TASK_TAG	0
#define configUSE_COUNTING_SEMAPHORES	1			//Œ™1 ± π”√º∆ ˝–≈∫≈¡ø


/***************************************************************************************************************/
/*                                FreeRTOS”Îƒ⁄¥Ê…Í«Î”–πÿ≈‰÷√—°œÓ                                                */
/***************************************************************************************************************/
//#define configSUPPORT_DYNAMIC_ALLOCATION        1                       //÷ß≥÷∂ØÃ¨ƒ⁄¥Ê…Í«Î V9.0
#define configTOTAL_HEAP_SIZE					((size_t)(20*1024))     //œµÕ≥À˘”–◊‹µƒ∂—¥Û–°

/***************************************************************************************************************/
/*                                FreeRTOS”Îπ≥◊”∫Ø ˝”–πÿµƒ≈‰÷√—°œÓ                                              */
/***************************************************************************************************************/
#define configUSE_IDLE_HOOK						0                       //1£¨ π”√ø’œ–π≥◊”£ª0£¨≤ª π”√
#define configUSE_TICK_HOOK						0                       //1£¨ π”√ ±º‰∆¨π≥◊”£ª0£¨≤ª π”√
#define configUSE_MALLOC_FAILED_HOOK	0												//1 π”√ƒ⁄¥Ê…Í«Î ß∞‹π≥◊”∫Ø ˝

/***************************************************************************************************************/
/*                                FreeRTOS”Î‘À–– ±º‰∫Õ»ŒŒÒ◊¥Ã¨ ’ºØ”–πÿµƒ≈‰÷√—°œÓ                                 */
/***************************************************************************************************************/
#define configGENERATE_RUN_TIME_STATS	        0                       //Œ™1 ±∆Ù”√‘À–– ±º‰Õ≥º∆π¶ƒ‹
#define configUSE_TRACE_FACILITY				1                       //Œ™1∆Ù”√ø… ”ªØ∏˙◊Ÿµ˜ ‘

																																				

/***************************************************************************************************************/
/*                                FreeRTOS”Î–≠≥Ã”–πÿµƒ≈‰÷√—°œÓ                                                  */
/***************************************************************************************************************/
/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0						//Œ™1 ±∆Ù”√–≠≥Ã£¨∆Ù”√–≠≥Ã“‘∫Û±ÿ–ÎÃÌº”Œƒº˛croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )	//–≠≥Ãµƒ”––ß”≈œ»º∂ ˝ƒ

/***************************************************************************************************************/
/*                                FreeRTOS”Î»Ìº˛∂® ±∆˜”–πÿµƒ≈‰÷√—°œÓ                                            */
/***************************************************************************************************************/
/* Software timer definitions. */
#define configUSE_TIMERS				1					//Œ™1 ±∆Ù”√»Ìº˛∂® ±∆˜
#define configTIMER_TASK_PRIORITY		( 2 )		//»Ìº˛∂® ±∆˜”≈œ»º∂
#define configTIMER_QUEUE_LENGTH		10			//»Ìº˛∂® ±∆˜∂”¡–≥§∂»
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )		//»Ìº˛∂® ±∆˜»ŒŒÒ∂—’ª¥Û–°

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
/***************************************************************************************************************/
/*                                FreeRTOSø…—°∫Ø ˝≈‰÷√—°œÓ                                                      */
/***************************************************************************************************************/
#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	1
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1




/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4        /* 15 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0xf

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
	

	
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
//#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */

