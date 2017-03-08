/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FREERTOS_H
#define __FREERTOS_H

// List of semaphore
osSemaphoreId sem_usb_irq;

// List of threads
osThreadId thread_motor_0;
osThreadId thread_motor_1;
osThreadId thread_usb_cmd;
#ifdef TEST_USB_SPEED
osThreadId thread_usb_test;
#endif

#endif /* __FREERTOS_H */