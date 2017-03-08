/**
  ******************************************************************************
  * @file           : usbd_mc_if.c
  * @brief          :
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "usbd_mc_if.h"
/* USER CODE BEGIN INCLUDE */
#include "low_level.h"
/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_MC
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_MC_Private_TypesDefinitions
  * @{
  */
/* USER CODE BEGIN PRIVATE_TYPES */
/* USER CODE END PRIVATE_TYPES */
/**
  * @}
  */

/** @defgroup USBD_MC_Private_Defines
  * @{
  */
/* USER CODE BEGIN PRIVATE_DEFINES */
/* Define size for the receive and transmit buffer over MC */
/* It's up to user to redefine and/or remove those define */
#define APP_RX_DATA_SIZE  64
#define APP_TX_DATA_SIZE  64
/* USER CODE END PRIVATE_DEFINES */
/**
  * @}
  */

/** @defgroup USBD_MC_Private_Macros
  * @{
  */
/* USER CODE BEGIN PRIVATE_MACRO */
/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_MC_Private_Variables
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/* Received Data over USB are stored in this buffer       */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/* Send Data over USB MC are stored in this buffer       */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_MC_IF_Exported_Variables
  * @{
  */
  extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE BEGIN EXPORTED_VARIABLES */
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_MC_Private_FunctionPrototypes
  * @{
  */
static int8_t MC_Init_FS     (void);
static int8_t MC_DeInit_FS   (void);
static int8_t MC_Control_FS  (uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t MC_Receive_FS  (uint8_t* pbuf, uint32_t *Len);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_MC_ItfTypeDef USBD_Interface_fops_FS =
{
  MC_Init_FS,
  MC_DeInit_FS,
  MC_Control_FS,
  MC_Receive_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  MC_Init_FS
  *         Initializes the MC media low layer over the FS USB IP
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
  /* Set Application Buffers */
  USBD_MC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_MC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  MC_DeInit_FS
  *         DeInitializes the MC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  MC_Control_FS
  *         Manage the MC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MC_Control_FS  (uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch (cmd)
  {
  case MC_SEND_ENCAPSULATED_COMMAND:

    break;

  case MC_GET_ENCAPSULATED_RESPONSE:

    break;

  case MC_SET_COMM_FEATURE:

    break;

  case MC_GET_COMM_FEATURE:

    break;

  case MC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
  case MC_SET_LINE_CODING:

    break;

  case MC_GET_LINE_CODING:

    break;

  case MC_SET_CONTROL_LINE_STATE:

    break;

  case MC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  MC_Receive_FS
  *         Data received over USB OUT endpoint are sent over MC interface
  *         through this function.
  *
  *         @note
  *         This function will block any OUT packet reception on USB endpoint
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on MC interface (ie. using DMA controller) it will result
  *         in receiving more data while previous ones are still not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MC_Receive_FS (uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
  USBD_MC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
  USBD_MC_ReceivePacket(&hUsbDeviceFS);

  motor_parse_cmd(Buf, *Len);

  return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  MC_Transmit_FS
  *         Data send over USB IN endpoint are sent over MC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be send
  * @param  Len: Number of data to be send (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t MC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */
  USBD_MC_HandleTypeDef *hmc = (USBD_MC_HandleTypeDef*)hUsbDeviceFS.pClassDataMC;
  if (hmc->TxState != 0){
    return USBD_BUSY;
  }
  USBD_MC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_MC_TransmitPacket(&hUsbDeviceFS);
  /* USER CODE END 7 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
