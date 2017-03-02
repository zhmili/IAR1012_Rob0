;********************************************************************************************************
;                                               uC/OS-II
;                                         The Real-Time Kernel
;
;                               (c) Copyright 1992-2006, Micrium, Weston, FL
;                                          All Rights Reserved
;
;                                           Generic ARM Port
;
; File      : OS_CPU_A.ASM
; Version   : V2.84
; By        : Jean J. Labrosse
;
; For       : ARMv7M Cortex-M3
; Mode      : Thumb2
; Toolchain : RealView Development Suite
;             RealView Microcontroller Development Kit (MDK)
;             ARM Developer Suite (ADS)
;             Keil uVision
;********************************************************************************************************

                EXTERN  OSRunning                ; External references
                EXTERN  OSPrioCur
                EXTERN  OSPrioHighRdy
                EXTERN  OSTCBCur
                EXTERN  OSTCBHighRdy
                EXTERN  OSIntNesting
                EXTERN  OSIntExit
                EXTERN  OSTaskSwHook


                EXPORT  OS_CPU_SR_Save           ; Functions declared in this file
                EXPORT  OS_CPU_SR_Restore
                EXPORT  OSStartHighRdy
                EXPORT  OSCtxSw
                EXPORT  OSIntCtxSw
                EXPORT  OSPendSV
                EXPORT  CPU_IntDis
                EXPORT  CPU_IntEn

NVIC_INT_CTRL   EQU     0xE000ED04               ; interrupt control state register
NVIC_SYSPRI2    EQU     0xE000ED20               ; system priority register (2)
NVIC_PENDSV_PRI EQU     0x00FF0000               ; PendSV priority value(lowest)
NVIC_PENDSVSET  EQU     0x10000000               ; value to trigger PendSV exception


;*********************************************************************************************************
;                                   CRITICAL SECTION METHOD 3 FUNCTIONS
;
; Description: Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
;              would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
;              disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to
;              disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
;              into the CPU's status register.
;
; Prototypes :     OS_CPU_SR  OS_CPU_SR_Save(void);
;                  void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
;
;
; Note(s)    : 1) These functions are used in general like this:
;
;                 void Task (void *p_arg)
;                 {
;                 #if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
;                     OS_CPU_SR  cpu_sr;
;                 #endif
;
;                          :
;                          :
;                     OS_ENTER_CRITICAL();             /* cpu_sr = OS_CPU_SaveSR();                */
;                          :
;                          :
;                     OS_EXIT_CRITICAL();              /* OS_CPU_RestoreSR(cpu_sr);                */
;                          :
;                          :
;                 }
;
;              2) OS_CPU_SaveSR() is implemented as recommended by Atmel's application note:
;
;            (N/A for Cortex-M3)    "Disabling Interrupts at Processor Level"
;*********************************************************************************************************

        RSEG CODE:CODE:NOROOT(2)


OS_CPU_SR_Save
        ;MRS     R0, PRIMASK                 ; set prio int mask to mask all (except faults)
        ;CPSID   I
        ;BX      LR
        MRS     R0, BASEPRI	
	MOV     R1, #0x20
	MSR     BASEPRI, R1
        BX      LR

OS_CPU_SR_Restore
	;MSR     PRIMASK, R0
        ;BX      LR
	MSR     BASEPRI, R0
        BX      LR
	NOP

;*********************************************************************************************************
;                                    DISABLE and ENABLE INTERRUPTS
;
; Description : Disable/Enable interrupts.
;
; Prototypes  : void  CPU_IntDis(void);
;               void  CPU_IntEn (void);
;*********************************************************************************************************

CPU_IntDis
        CPSID   I
        BX      LR


CPU_IntEn
        CPSIE   I
        BX      LR


;*********************************************************************************************************
;                                          START MULTITASKING
;                                       void OSStartHighRdy(void)
;
; This function uses the context switcher (see below) to cause the first task to start.
; This is because for Cortex-M3 it makes most sense to implement a context switch within
; an exception handler, to take advantage of the processor's auto context save and restore
; features.
;
; Note(s) : 1) OSStartHighRdy() MUST:
;              a) Setup PendSV exception priority to lowest,
;              b) Set initial psp to 0, to tell context switcher this is first run
;              c) Set OSRunning to TRUE,
;              d) Trigger PendSV exception,
;              e) Enable interrupts (tasks will run with ints enabled)
;*********************************************************************************************************

OSStartHighRdy
	    
        LDR     R4, =NVIC_SYSPRI2       ; set the PendSV exception priority
        LDR     R5, =NVIC_PENDSV_PRI
        STR     R5, [R4]

        MOV     R4, #0                  ; set the PSP to 0 for initial context switch call
        MSR     PSP, R4

        LDR     R4, __OS_Running        ; OSRunning = TRUE
        MOV     R5, #1
        STRB    R5, [R4]

        LDR     R4, =NVIC_INT_CTRL      ; trigger the PendSV exception (causes context switch)
        LDR     R5, =NVIC_PENDSVSET
        STR     R5, [R4]

        CPSIE   I                       ; enable interrupts at processor level
OSStartHang
        B       OSStartHang             ; should never get here


;*********************************************************************************************************
;                         PERFORM A CONTEXT SWITCH (From task level) - OSCtxSw()
;
; Note(s) : 1) OSCtxSw() is called when OS wants to do a task context switch
;              This function triggers the PendSV exception which is where the
;              real work is done.
;*********************************************************************************************************

OSCtxSw
        LDR     R0, =NVIC_INT_CTRL      ; trigger the PendSV exception (causes context switch)
        LDR     R1, =NVIC_PENDSVSET
        STR     R1, [R0]
        BX      LR

;*********************************************************************************************************
;                   PERFORM A CONTEXT SWITCH (From interrupt level) - OSIntCtxSw()
;
; Notes:    1) This function is called by OSIntExit() when it determines a context
;              switch is needed as the result of an interrupt.
;
;           2) This function simply triggers a PendSV exception which will cause the
;              OSPendSV to run whenever there are no more interrupts active
;              and interrupts are enabled.
;*********************************************************************************************************

OSIntCtxSw
        LDR     R0, =NVIC_INT_CTRL      ; trigger the PendSV exception (causes context switch)
        LDR     R1, =NVIC_PENDSVSET
        STR     R1, [R0]
        BX      LR

        NOP

;*********************************************************************************************************
;                         PERFORM A CONTEXT SWITCH (From task level) - OSCtxSw()
;
; Note(s) : 1) OSPendSV is used to cause a context switch.  This is a recommended
;              method for performing context switches with Cortex-M3.  This is because
;              Cortex-M3 auto-saves half of the processor context on any exception, and
;              restores same on return from exception.  So only saving of R4-R11 is required
;              and fixing up the stack pointers.  Using the PendSV exception this way means
;              that context saving and restoring is identical method whether it is
;              initiated from a thread or occurs due to an interrupt or exception.
;              Pseudo-code is:
;              a) Get the process SP, if 0 then skip (goto d) the saving part (first context switch)
;              b) Save remaining regs r4-r11 on process stack
;              c) Save the process SP in it's TCB, OSTCBCur->OSTCBStkPtr = SP;
;              d) Call OSTaskSwHook()
;              e) Get current high priority, OSPrioCur = OSPrioHighRdy
;              f) Get current ready thread TCB, OSTCBCur = OSTCBHighRdy
;              g) Get new process SP from TCB, SP = OSTCBHighRdy->OSTCBStkPtr
;              h) Restore R4-R11 from new process stack
;              i) Perform exception return which will restore remaining context
;
;           2) On entry into OSPendSV handler:
;              a) The following have been saved on the process stack (by processor):
;                 xPSR, PC, LR, R12, R0-R3
;              b) Processor mode is switched to Handler mode (from Thread mode)
;              c) Stack is Main stack (switched from Process stack)
;              d) OSTCBCur      points to the OS_TCB of the task to suspend
;                 OSTCBHighRdy  points to the OS_TCB of the task to resume
;
;           3) Since OSPendSV is set to lowest priority in the system (by OSCtxSw() above),
;              we know that it will only be run when no other exception or interrupt is
;              active, and therefore safe to assume that context being switched out was
;              using the process stack (PSP)
;*********************************************************************************************************

OSPendSV
        MRS     R1, BASEPRI	
	MOV     R0, #0x20
	MSR     BASEPRI, R0
        ;MRS     R1, PRIMASK                 ; set prio int mask to mask all (except faults)
        ;CPSID   I
	;NOP

        MRS     R0, PSP                 ; PSP is process stack pointer
        CBZ     R0, OSPendSV_nosave     ; skip register save the first time

        SUB     R0, R0, #0x20          ; save remaining regs r4-11 on process stack
        STM     R0, {R4-R11}

        LDR     R4, __OS_TCBCur         ; OSTCBCur->OSTCBStkPtr = SP;
        LDR     R4, [R4]
        STR     R0, [R4]                ; R0 is SP of process being switched out

                                        ; at this point, entire context of process has been saved
OSPendSV_nosave
        PUSH    {R1, R14}                   ; need to save LR exc_return value
        LDR     R0, __OS_TaskSwHook     ; OSTaskSwHook();
        BLX     R0
        POP     {R1, R14}

        LDR     R4, __OS_PrioCur        ; OSPrioCur = OSPrioHighRdy
        LDR     R5, __OS_PrioHighRdy
        LDRB    R6, [R5]
        STRB    R6, [R4]

        LDR     R4, __OS_TCBCur         ; OSTCBCur  = OSTCBHighRdy;
        LDR     R6, __OS_TCBHighRdy
        LDR     R6, [R6]
        STR     R6, [R4]

        LDR     R0, [R6]                ; R0 is new process SP; SP = OSTCBHighRdy->OSTCBStkPtr;
        LDM     R0, {R4-R11}        ; restore  R1(BASEPRI), r4-11 from new process stack
        ADD     R0, R0, #0x20
        MSR     PSP, R0                 ; load PSP with new process SP

        ORR     LR, LR, #0x04           ; ensure exception return uses process stack

        ;MSR    PRIMASK, R1             ; 
	MSR     BASEPRI, R1

        BX      LR                      ; exception return will restore remaining context

        NOP

;*********************************************************************************************************
;                                     POINTERS TO VARIABLES
;*********************************************************************************************************

        DATA

__OS_TaskSwHook
        DC32     OSTaskSwHook

__OS_IntExit
        DC32     OSIntExit

__OS_IntNesting
        DC32     OSIntNesting

__OS_PrioCur
        DC32     OSPrioCur

__OS_PrioHighRdy
        DC32     OSPrioHighRdy

__OS_Running
        DC32     OSRunning

__OS_TCBCur
        DC32     OSTCBCur

__OS_TCBHighRdy
        DC32     OSTCBHighRdy


        END

