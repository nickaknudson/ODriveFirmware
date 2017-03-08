/**
  ******************************************************************************
  * @file    usbd_mc.h
  * @brief   
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_MC_H
#define __USB_MC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup usbd_cdc
  * @brief This file is the Header file for usbd_cdc.c
  * @{
  */


/** @defgroup usbd_cdc_Exported_Defines
  * @{
  */
#define MC_IN_EP                                   0x91  /* EP1 for data IN */
#define MC_OUT_EP                                  0x11  /* EP1 for data OUT */

/* MC Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#define MC_DATA_HS_MAX_PACKET_SIZE                 512  /* Endpoint IN & OUT Packet size */
#define MC_DATA_FS_MAX_PACKET_SIZE                 64  /* Endpoint IN & OUT Packet size */
#define MC_CMD_PACKET_SIZE                         8  /* Control Endpoint Packet size */

#define MC_DATA_HS_IN_PACKET_SIZE                  MC_DATA_HS_MAX_PACKET_SIZE
#define MC_DATA_HS_OUT_PACKET_SIZE                 MC_DATA_HS_MAX_PACKET_SIZE

#define MC_DATA_FS_IN_PACKET_SIZE                  MC_DATA_FS_MAX_PACKET_SIZE
#define MC_DATA_FS_OUT_PACKET_SIZE                 MC_DATA_FS_MAX_PACKET_SIZE

/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

/**
  * @}
  */
typedef struct _USBD_MC_Itf
{
  int8_t (* Init)          (void);
  int8_t (* DeInit)        (void);
  int8_t (* Control)       (uint8_t, uint8_t * , uint16_t);
  int8_t (* Receive)       (uint8_t *, uint32_t *);

}USBD_MC_ItfTypeDef;


typedef struct
{
  uint32_t data[MC_DATA_HS_MAX_PACKET_SIZE/4];      /* Force 32bits alignment */
  uint8_t  CmdOpCode;
  uint8_t  CmdLength;
  uint8_t  *RxBuffer;
  uint8_t  *TxBuffer;
  uint32_t RxLength;
  uint32_t TxLength;

  __IO uint32_t TxState;
  __IO uint32_t RxState;
}
USBD_MC_HandleTypeDef;



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef  USBD_MC;
#define USBD_MC_CLASS    &USBD_MC
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t  USBD_MC_RegisterInterface  (USBD_HandleTypeDef   *pdev,
                                      USBD_MC_ItfTypeDef *fops);

uint8_t  USBD_MC_SetTxBuffer        (USBD_HandleTypeDef   *pdev,
                                      uint8_t  *pbuff,
                                      uint16_t length);

uint8_t  USBD_MC_SetRxBuffer        (USBD_HandleTypeDef   *pdev,
                                      uint8_t  *pbuff);

uint8_t  USBD_MC_ReceivePacket      (USBD_HandleTypeDef *pdev);

uint8_t  USBD_MC_TransmitPacket     (USBD_HandleTypeDef *pdev);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_MC_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
