/**
  ******************************************************************************
  * @file    usbd_mc.c
  * @brief
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc.h"


/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_MC
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_MC_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_MC_Private_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_MC_Private_Macros
  * @{
  */

/**
  * @}
  */


/** @defgroup USBD_MC_Private_FunctionPrototypes
  * @{
  */


static uint8_t  USBD_MC_Init (USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx);

static uint8_t  USBD_MC_DeInit (USBD_HandleTypeDef *pdev,
                                 uint8_t cfgidx);

static uint8_t  USBD_MC_Setup (USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req);

static uint8_t  USBD_MC_DataIn (USBD_HandleTypeDef *pdev,
                                 uint8_t epnum);

static uint8_t  USBD_MC_DataOut (USBD_HandleTypeDef *pdev,
                                 uint8_t epnum);

static uint8_t  USBD_MC_EP0_RxReady (USBD_HandleTypeDef *pdev);

static uint8_t  *USBD_MC_GetFSCfgDesc (uint16_t *length);

static uint8_t  *USBD_MC_GetHSCfgDesc (uint16_t *length);

static uint8_t  *USBD_MC_GetOtherSpeedCfgDesc (uint16_t *length);

static uint8_t  *USBD_MC_GetOtherSpeedCfgDesc (uint16_t *length);

uint8_t  *USBD_MC_GetDeviceQualifierDescriptor (uint16_t *length);

/**
  * @}
  */

/** @defgroup USBD_MC_Private_Variables
  * @{
  */


/* MC interface class callbacks structure */
USBD_ClassTypeDef  USBD_MC =
{
  USBD_MC_Init,
  USBD_MC_DeInit,
  USBD_MC_Setup,
  NULL,                 /* EP0_TxSent, */
  USBD_MC_EP0_RxReady,
  USBD_MC_DataIn,
  USBD_MC_DataOut,
  NULL,
  NULL,
  NULL,
  USBD_MC_GetHSCfgDesc,
  USBD_MC_GetFSCfgDesc,
  USBD_MC_GetOtherSpeedCfgDesc,
  USBD_MC_GetDeviceQualifierDescriptor,
};

/**
  * @}
  */

/** @defgroup USBD_MC_Private_Functions
  * @{
  */

/**
  * @brief  USBD_MC_Init
  *         Initialize the MC interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_MC_Init (USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx)
{
  uint8_t ret = 0;
  USBD_MC_HandleTypeDef   *hmc;

  if(pdev->dev_speed == USBD_SPEED_HIGH  )
  {
    /* Open EP IN */
    USBD_LL_OpenEP(pdev,
                   MC_IN_EP,
                   USBD_EP_TYPE_BULK,
                   MC_DATA_HS_IN_PACKET_SIZE);

    /* Open EP OUT */
    USBD_LL_OpenEP(pdev,
                   MC_OUT_EP,
                   USBD_EP_TYPE_BULK,
                   MC_DATA_HS_OUT_PACKET_SIZE);

  }
  else
  {
    /* Open EP IN */
    USBD_LL_OpenEP(pdev,
                   MC_IN_EP,
                   USBD_EP_TYPE_BULK,
                   MC_DATA_FS_IN_PACKET_SIZE);

    /* Open EP OUT */
    USBD_LL_OpenEP(pdev,
                   MC_OUT_EP,
                   USBD_EP_TYPE_BULK,
                   MC_DATA_FS_OUT_PACKET_SIZE);
  }

  pdev->pClassDataMC = USBD_malloc(sizeof (USBD_MC_HandleTypeDef));//FIXME

  if(pdev->pClassDataMC == NULL)//FIXME
  {
    ret = 1;
  }
  else
  {
    hmc = (USBD_MC_HandleTypeDef*) pdev->pClassDataMC;//FIXME

    /* Init  physical Interface components */
    ((USBD_MC_ItfTypeDef *)pdev->pUserDataMC)->Init();//FIXME

    /* Init Xfer states */
    hmc->TxState =0;
    hmc->RxState =0;

    if(pdev->dev_speed == USBD_SPEED_HIGH  )//FIXME
    {
      /* Prepare Out endpoint to receive next packet */
      USBD_LL_PrepareReceive(pdev,
                             MC_OUT_EP,
                             hmc->RxBuffer,
                             MC_DATA_HS_OUT_PACKET_SIZE);
    }
    else
    {
      /* Prepare Out endpoint to receive next packet */
      USBD_LL_PrepareReceive(pdev,
                             MC_OUT_EP,
                             hmc->RxBuffer,
                             MC_DATA_FS_OUT_PACKET_SIZE);
    }


  }
  return ret;
}

/**
  * @brief  USBD_MC_Init
  *         DeInitialize the MC layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_MC_DeInit (USBD_HandleTypeDef *pdev,
                                 uint8_t cfgidx)
{
  uint8_t ret = 0;

  /* Open EP IN */
  USBD_LL_CloseEP(pdev,
              MC_IN_EP);

  /* Open EP OUT */
  USBD_LL_CloseEP(pdev,
              MC_OUT_EP);


  /* DeInit  physical Interface components */
  if(pdev->pClassDataMC != NULL)//FIXME
  {
    ((USBD_MC_ItfTypeDef *)pdev->pUserDataMC)->DeInit();//FIXME
    USBD_free(pdev->pClassDataMC);//FIXME
    pdev->pClassDataMC = NULL;//FIXME
  }

  return ret;
}

/**
  * @brief  USBD_MC_Setup
  *         Handle the MC specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_MC_Setup (USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req)
{
  USBD_MC_HandleTypeDef   *hmc = (USBD_MC_HandleTypeDef*) pdev->pClassDataMC;//FIXME
  static uint8_t ifalt = 0;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :
    if (req->wLength)
    {
      if (req->bmRequest & 0x80)
      {
        ((USBD_MC_ItfTypeDef *)pdev->pUserDataMC)->Control(req->bRequest,
                                                          (uint8_t *)hmc->data,
                                                          req->wLength);//FIXME
          USBD_CtlSendData (pdev,
                            (uint8_t *)hmc->data,
                            req->wLength);
      }
      else
      {
        hmc->CmdOpCode = req->bRequest;
        hmc->CmdLength = req->wLength;

        USBD_CtlPrepareRx (pdev,
                           (uint8_t *)hmc->data,
                           req->wLength);
      }

    }
    else
    {
      ((USBD_MC_ItfTypeDef *)pdev->pUserDataMC)->Control(req->bRequest,
                                                        (uint8_t*)req,
                                                        0);//FIXME
    }
    break;

  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_INTERFACE :
      USBD_CtlSendData (pdev,
                        &ifalt,
                        1);
      break;

    case USB_REQ_SET_INTERFACE :
      break;
    }

  default:
    break;
  }
  return USBD_OK;
}

/**
  * @brief  USBD_MC_DataIn
  *         Data sent on non-control IN endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  USBD_MC_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  USBD_MC_HandleTypeDef   *hmc = (USBD_MC_HandleTypeDef*) pdev->pClassDataMC;//FIXME

  if(pdev->pClassDataMC != NULL)//FIXME
  {

    hmc->TxState = 0;

    return USBD_OK;
  }
  else
  {
    return USBD_FAIL;
  }
}

/**
  * @brief  USBD_MC_DataOut
  *         Data received on non-control Out endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  USBD_MC_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  USBD_MC_HandleTypeDef   *hmc = (USBD_MC_HandleTypeDef*) pdev->pClassDataMC;//FIXME

  /* Get the received data length */
  hmc->RxLength = USBD_LL_GetRxDataSize (pdev, epnum);

  /* USB data will be immediately processed, this allow next USB traffic being
  NAKed till the end of the application Xfer */
  if(pdev->pClassDataMC != NULL)//FIXME
  {
    ((USBD_MC_ItfTypeDef *)pdev->pUserDataMC)->Receive(hmc->RxBuffer, &hmc->RxLength);//FIXME

    return USBD_OK;
  }
  else
  {
    return USBD_FAIL;
  }
}



/**
  * @brief  USBD_MC_DataOut
  *         Data received on non-control Out endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  USBD_MC_EP0_RxReady (USBD_HandleTypeDef *pdev)
{
  USBD_MC_HandleTypeDef   *hmc = (USBD_MC_HandleTypeDef*) pdev->pClassDataMC;//FIXME

  if((pdev->pUserDataMC != NULL) && (hmc->CmdOpCode != 0xFF))
  {
    ((USBD_MC_ItfTypeDef *)pdev->pUserDataMC)->Control(hmc->CmdOpCode,
                                                      (uint8_t *)hmc->data,
                                                      hmc->CmdLength);//FIXME
      hmc->CmdOpCode = 0xFF;

  }
  return USBD_OK;
}

/**
* @brief  USBD_MC_RegisterInterface
  * @param  pdev: device instance
  * @param  fops: MC  Interface callback
  * @retval status
  */
uint8_t  USBD_MC_RegisterInterface  (USBD_HandleTypeDef   *pdev,
                                      USBD_MC_ItfTypeDef *fops)
{
  uint8_t  ret = USBD_FAIL;

  if(fops != NULL)
  {
    pdev->pUserDataMC= fops;//FIXME
    ret = USBD_OK;
  }

  return ret;
}

/**
  * @brief  USBD_MC_SetTxBuffer
  * @param  pdev: device instance
  * @param  pbuff: Tx Buffer
  * @retval status
  */
uint8_t  USBD_MC_SetTxBuffer  (USBD_HandleTypeDef   *pdev,
                                uint8_t  *pbuff,
                                uint16_t length)
{
  USBD_MC_HandleTypeDef   *hmc = (USBD_MC_HandleTypeDef*) pdev->pClassDataMC;//FIXME

  hmc->TxBuffer = pbuff;
  hmc->TxLength = length;

  return USBD_OK;
}


/**
  * @brief  USBD_MC_SetRxBuffer
  * @param  pdev: device instance
  * @param  pbuff: Rx Buffer
  * @retval status
  */
uint8_t  USBD_MC_SetRxBuffer  (USBD_HandleTypeDef   *pdev,
                                   uint8_t  *pbuff)
{
  USBD_MC_HandleTypeDef   *hmc = (USBD_MC_HandleTypeDef*) pdev->pClassDataMC;//FIXME

  hmc->RxBuffer = pbuff;

  return USBD_OK;
}

/**
  * @brief  USBD_MC_DataOut
  *         Data received on non-control Out endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
uint8_t  USBD_MC_TransmitPacket(USBD_HandleTypeDef *pdev)
{
  USBD_MC_HandleTypeDef   *hmc = (USBD_MC_HandleTypeDef*) pdev->pClassDataMC;//FIXME

  if(pdev->pClassDataMC != NULL)//FIXME
  {
    if(hmc->TxState == 0)
    {
      /* Tx Transfer in progress */
      hmc->TxState = 1;

      /* Transmit next packet */
      USBD_LL_Transmit(pdev,
                       MC_IN_EP,
                       hmc->TxBuffer,
                       hmc->TxLength);

      return USBD_OK;
    }
    else
    {
      return USBD_BUSY;
    }
  }
  else
  {
    return USBD_FAIL;
  }
}


/**
  * @brief  USBD_MC_ReceivePacket
  *         prepare OUT Endpoint for reception
  * @param  pdev: device instance
  * @retval status
  */
uint8_t  USBD_MC_ReceivePacket(USBD_HandleTypeDef *pdev)
{
  USBD_MC_HandleTypeDef   *hmc = (USBD_MC_HandleTypeDef*) pdev->pClassDataMC;//FIXME

  /* Suspend or Resume USB Out process */
  if(pdev->pClassDataMC != NULL)//FIXME
  {
    if(pdev->dev_speed == USBD_SPEED_HIGH  )
    {
      /* Prepare Out endpoint to receive next packet */
      USBD_LL_PrepareReceive(pdev,
                             MC_OUT_EP,
                             hmc->RxBuffer,
                             MC_DATA_HS_OUT_PACKET_SIZE);
    }
    else
    {
      /* Prepare Out endpoint to receive next packet */
      USBD_LL_PrepareReceive(pdev,
                             MC_OUT_EP,
                             hmc->RxBuffer,
                             MC_DATA_FS_OUT_PACKET_SIZE);
    }
    return USBD_OK;
  }
  else
  {
    return USBD_FAIL;
  }
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
