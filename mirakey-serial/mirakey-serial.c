#include "mirakey-serial.h"
#include "main.h"

namespace mirakey-serial {


void MKS_Init() {
    //todo: I'll need to implement/remove these based on need
    HAL_Init();
    
    MKS_SystemClock_Config();
    MKS_GPIO_Init();
    
    DMA_Init();
}

void MKS_DMAInit() {
    DMA_InitTypeDef dma;
    DMA_DeInit(DMA1_Channel1);
    DMA_StructInit(&dma);

    dma.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dma.DMA_MemoryBaseAddr = (uint32_t)spi_tx_buffer;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dma.DMA_DIR = DMA_DIR_PeripheralDST;
    dma.DMA_BufferSize = BUFFERSIZE;   
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_M2M = DMA_M2M_Disable;

    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_Init(DMA1_Channel1, &dma);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);

    DMA_Cmd(DMA1_Channel1, ENABLE);
}

void MKS_Deinit() {}

static void MKS_GPIO_Init(void) {
    /*    
    todo: look up what the different pin modes do.
    */
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /*MKS_CLK*/
    //todo: the clock has special hardware
    //GPIO_PinAFConfig(GPIOB, AF_PIN_SOURCE, GPIO_AF);
    //GPIO_InitStructure.GPIO_Pin = GPIO_PIN_5;
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStruct.Pin = A5_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(A5_GPIO_Port, &GPIO_InitStruct);
    
    /*MKS_MOSI*/
    GPIO_InitStruct.Pin = A7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(A7_GPIO_Port, &GPIO_InitStruct);
    
    /*MKS_SHHLD*/
    //todo: see if you can sync this guy with the clock when you need it.
    GPIO_InitStruct.Pin = B6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(B6_GPIO_Port, &GPIO_InitStruct);
    
    /*MKS_RST*/
    GPIO_InitStruct.Pin = B7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(B7_GPIO_Port, &GPIO_InitStruct);
}

void MKS_Select(uint8_t slave_address) {

}



void MKS_Command(uint8_t command) {
    // cheating with HAL
    uint8_t tx[2] = {0x1, command};
    HAL_SPI_Transmit(&mirakey_serial_controller.hspi, tx, sizeof(tx), 5000);
}

    
/*
MSB vs LSB. I think it'll be MSB
mster mode
serial clo
*/
void MKS_Data(uint8_t data, uint16_t size) {
    // let's try something more intelligent
      
    /* Set the transaction information */
    hspi->State       = HAL_SPI_STATE_BUSY_TX;
    hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
    hspi->pTxBuffPtr  = (uint8_t *)pData;
    hspi->TxXferSize  = Size;
    hspi->TxXferCount = Size;

    /* Init field not used in handle to zero */
    hspi->pRxBuffPtr  = (uint8_t *)NULL;
    hspi->TxISR       = NULL;
    hspi->RxISR       = NULL;
    hspi->RxXferSize  = 0U;
    hspi->RxXferCount = 0U;
    
    /* Disable SPI Peripheral before set 1Line direction (BIDIOE bit) */
    //__HAL_SPI_DISABLE(hspi);
    //SPI_1LINE_TX(hspi);
    
    HAL_DMA_Start_IT(hspi->hdmatx, (uint32_t)hspi->pTxBuffPtr, (uint32_t)&hspi->Instance->DR,
                                 hspi->TxXferCount)
}


/*------------------------------------------------------------------------------
Copy pasted some stuff from the hal spi implementation. I want to use DMA if possible.
The difficulty will be synchrnoizing the SHHLD signal with the SPI bus.
------------------------------------------------------------------------------*/

/**
  * @brief  Tx 8-bit handler for Transmit and Receive in Interrupt mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_2linesTxISR_8BIT(struct __SPI_HandleTypeDef *hspi)
{
  *(__IO uint8_t *)&hspi->Instance->DR = (*hspi->pTxBuffPtr);
  hspi->pTxBuffPtr++;
  hspi->TxXferCount--;

  /* Check the end of the transmission */
  if (hspi->TxXferCount == 0U)
  {
#if (USE_SPI_CRC != 0U)
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
    {
      /* Set CRC Next Bit to send CRC */
      SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
      /* Disable TXE interrupt */
      __HAL_SPI_DISABLE_IT(hspi, SPI_IT_TXE);
      return;
    }
#endif /* USE_SPI_CRC */

    /* Disable TXE interrupt */
    __HAL_SPI_DISABLE_IT(hspi, SPI_IT_TXE);

    if (hspi->RxXferCount == 0U)
    {
      SPI_CloseRxTx_ISR(hspi);
    }
  }
}

/*------------------------------------------------------------------------------

//todo: enable DMA.
// It's a module that would handle the data transmission in parallel to the actual MCU
------------------------------------------------------------------------------*/

HAL_SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData,
                                              uint16_t Size)
{
  uint32_t             tmp_mode;
  HAL_SPI_StateTypeDef tmp_state;
  HAL_StatusTypeDef errorcode = HAL_OK;

  /* Check rx & tx dma handles */
  assert_param(IS_SPI_DMA_HANDLE(hspi->hdmarx));
  assert_param(IS_SPI_DMA_HANDLE(hspi->hdmatx));

  /* Check Direction parameter */
  assert_param(IS_SPI_DIRECTION_2LINES(hspi->Init.Direction));

  /* Process locked */
  __HAL_LOCK(hspi);

  /* Init temporary variables */
  tmp_state           = hspi->State;
  tmp_mode            = hspi->Init.Mode;

  if (!((tmp_state == HAL_SPI_STATE_READY) ||
        ((tmp_mode == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES) && (tmp_state == HAL_SPI_STATE_BUSY_RX))))
  {
    errorcode = HAL_BUSY;
    goto error;
  }

  if ((pTxData == NULL) || (pRxData == NULL) || (Size == 0U))
  {
    errorcode = HAL_ERROR;
    goto error;
  }

  /* Don't overwrite in case of HAL_SPI_STATE_BUSY_RX */
  if (hspi->State != HAL_SPI_STATE_BUSY_RX)
  {
    hspi->State = HAL_SPI_STATE_BUSY_TX_RX;
  }

  /* Set the transaction information */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pTxBuffPtr  = (uint8_t *)pTxData;
  hspi->TxXferSize  = Size;
  hspi->TxXferCount = Size;
  hspi->pRxBuffPtr  = (uint8_t *)pRxData;
  hspi->RxXferSize  = Size;
  hspi->RxXferCount = Size;

  /* Init field not used in handle to zero */
  hspi->RxISR       = NULL;
  hspi->TxISR       = NULL;

#if (USE_SPI_CRC != 0U)
  /* Reset CRC Calculation */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SPI_RESET_CRC(hspi);
  }
#endif /* USE_SPI_CRC */

  /* Check if we are in Rx only or in Rx/Tx Mode and configure the DMA transfer complete callback */
  if (hspi->State == HAL_SPI_STATE_BUSY_RX)
  {
    /* Set the SPI Rx DMA Half transfer complete callback */
    hspi->hdmarx->XferHalfCpltCallback = SPI_DMAHalfReceiveCplt;
    hspi->hdmarx->XferCpltCallback     = SPI_DMAReceiveCplt;
  }
  else
  {
    /* Set the SPI Tx/Rx DMA Half transfer complete callback */
    hspi->hdmarx->XferHalfCpltCallback = SPI_DMAHalfTransmitReceiveCplt;
    hspi->hdmarx->XferCpltCallback     = SPI_DMATransmitReceiveCplt;
  }

  /* Set the DMA error callback */
  hspi->hdmarx->XferErrorCallback = SPI_DMAError;

  /* Set the DMA AbortCpltCallback */
  hspi->hdmarx->XferAbortCallback = NULL;

  /* Enable the Rx DMA Stream/Channel  */
  if (HAL_OK != HAL_DMA_Start_IT(hspi->hdmarx, (uint32_t)&hspi->Instance->DR, (uint32_t)hspi->pRxBuffPtr,
                                 hspi->RxXferCount))
  {
    /* Update SPI error code */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);
    errorcode = HAL_ERROR;

    hspi->State = HAL_SPI_STATE_READY;
    goto error;
  }

  /* Enable Rx DMA Request */
  SET_BIT(hspi->Instance->CR2, SPI_CR2_RXDMAEN);

  /* Set the SPI Tx DMA transfer complete callback as NULL because the communication closing
  is performed in DMA reception complete callback  */
  hspi->hdmatx->XferHalfCpltCallback = NULL;
  hspi->hdmatx->XferCpltCallback     = NULL;
  hspi->hdmatx->XferErrorCallback    = NULL;
  hspi->hdmatx->XferAbortCallback    = NULL;

  /* Enable the Tx DMA Stream/Channel  */
  if (HAL_OK != HAL_DMA_Start_IT(hspi->hdmatx, (uint32_t)hspi->pTxBuffPtr, (uint32_t)&hspi->Instance->DR,
                                 hspi->TxXferCount))
  {
    /* Update SPI error code */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);
    errorcode = HAL_ERROR;

    hspi->State = HAL_SPI_STATE_READY;
    goto error;
  }

  /* Check if the SPI is already enabled */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);
  }
  /* Enable the SPI Error Interrupt Bit */
  __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_ERR));

  /* Enable Tx DMA Request */
  SET_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN);

error :
  /* Process Unlocked */
  __HAL_UNLOCK(hspi);
  return errorcode;
}

/**
  * @brief  Abort ongoing transfer (blocking mode).
  * @param  hspi SPI handle.
  * @note   This procedure could be used for aborting any ongoing transfer (Tx and Rx),
  *         started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable SPI Interrupts (depending of transfer direction)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
  *           - Set handle State to READY
  * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Abort(SPI_HandleTypeDef *hspi)
{
  HAL_StatusTypeDef errorcode;
  __IO uint32_t count;
  __IO uint32_t resetcount;

  /* Initialized local variable  */
  errorcode = HAL_OK;
  resetcount = SPI_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);
  count = resetcount;

  /* Clear ERRIE interrupt to avoid error interrupts generation during Abort procedure */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_ERRIE);

  /* Disable TXEIE, RXNEIE and ERRIE(mode fault event, overrun error, TI frame error) interrupts */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXEIE))
  {
    hspi->TxISR = SPI_AbortTx_ISR;
    /* Wait HAL_SPI_STATE_ABORT state */
    do
    {
      if (count == 0U)
      {
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
        break;
      }
      count--;
    } while (hspi->State != HAL_SPI_STATE_ABORT);
    /* Reset Timeout Counter */
    count = resetcount;
  }

  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXNEIE))
  {
    hspi->RxISR = SPI_AbortRx_ISR;
    /* Wait HAL_SPI_STATE_ABORT state */
    do
    {
      if (count == 0U)
      {
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
        break;
      }
      count--;
    } while (hspi->State != HAL_SPI_STATE_ABORT);
    /* Reset Timeout Counter */
    count = resetcount;
  }

  /* Disable the SPI DMA Tx request if enabled */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXDMAEN))
  {
    /* Abort the SPI DMA Tx Stream/Channel : use blocking DMA Abort API (no callback) */
    if (hspi->hdmatx != NULL)
    {
      /* Set the SPI DMA Abort callback :
      will lead to call HAL_SPI_AbortCpltCallback() at end of DMA abort procedure */
      hspi->hdmatx->XferAbortCallback = NULL;

      /* Abort DMA Tx Handle linked to SPI Peripheral */
      if (HAL_DMA_Abort(hspi->hdmatx) != HAL_OK)
      {
        hspi->ErrorCode = HAL_SPI_ERROR_ABORT;
      }

      /* Disable Tx DMA Request */
      CLEAR_BIT(hspi->Instance->CR2, (SPI_CR2_TXDMAEN));

      /* Wait until TXE flag is set */
      do
      {
        if (count == 0U)
        {
          SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
          break;
        }
        count--;
      } while ((hspi->Instance->SR & SPI_FLAG_TXE) == RESET);
    }
  }

  /* Disable the SPI DMA Rx request if enabled */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXDMAEN))
  {
    /* Abort the SPI DMA Rx Stream/Channel : use blocking DMA Abort API (no callback) */
    if (hspi->hdmarx != NULL)
    {
      /* Set the SPI DMA Abort callback :
      will lead to call HAL_SPI_AbortCpltCallback() at end of DMA abort procedure */
      hspi->hdmarx->XferAbortCallback = NULL;

      /* Abort DMA Rx Handle linked to SPI Peripheral */
      if (HAL_DMA_Abort(hspi->hdmarx) != HAL_OK)
      {
        hspi->ErrorCode = HAL_SPI_ERROR_ABORT;
      }

      /* Disable peripheral */
      __HAL_SPI_DISABLE(hspi);

      /* Disable Rx DMA Request */
      CLEAR_BIT(hspi->Instance->CR2, (SPI_CR2_RXDMAEN));
    }
  }
  /* Reset Tx and Rx transfer counters */
  hspi->RxXferCount = 0U;
  hspi->TxXferCount = 0U;

  /* Check error during Abort procedure */
  if (hspi->ErrorCode == HAL_SPI_ERROR_ABORT)
  {
    /* return HAL_Error in case of error during Abort procedure */
    errorcode = HAL_ERROR;
  }
  else
  {
    /* Reset errorCode */
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;
  }

  /* Clear the Error flags in the SR register */
  __HAL_SPI_CLEAR_OVRFLAG(hspi);
  __HAL_SPI_CLEAR_FREFLAG(hspi);

  /* Restore hspi->state to ready */
  hspi->State = HAL_SPI_STATE_READY;

  return errorcode;
}

/**
  * @brief  Abort ongoing transfer (Interrupt mode).
  * @param  hspi SPI handle.
  * @note   This procedure could be used for aborting any ongoing transfer (Tx and Rx),
  *         started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable SPI Interrupts (depending of transfer direction)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort_IT (in case of transfer in DMA mode)
  *           - Set handle State to READY
  *           - At abort completion, call user abort complete callback
  * @note   This procedure is executed in Interrupt mode, meaning that abort procedure could be
  *         considered as completed only when user abort complete callback is executed (not when exiting function).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Abort_IT(SPI_HandleTypeDef *hspi)
{
  HAL_StatusTypeDef errorcode;
  uint32_t abortcplt ;
  __IO uint32_t count;
  __IO uint32_t resetcount;

  /* Initialized local variable  */
  errorcode = HAL_OK;
  abortcplt = 1U;
  resetcount = SPI_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);
  count = resetcount;

  /* Clear ERRIE interrupt to avoid error interrupts generation during Abort procedure */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_ERRIE);

  /* Change Rx and Tx Irq Handler to Disable TXEIE, RXNEIE and ERRIE interrupts */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXEIE))
  {
    hspi->TxISR = SPI_AbortTx_ISR;
    /* Wait HAL_SPI_STATE_ABORT state */
    do
    {
      if (count == 0U)
      {
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
        break;
      }
      count--;
    } while (hspi->State != HAL_SPI_STATE_ABORT);
    /* Reset Timeout Counter */
    count = resetcount;
  }

  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXNEIE))
  {
    hspi->RxISR = SPI_AbortRx_ISR;
    /* Wait HAL_SPI_STATE_ABORT state */
    do
    {
      if (count == 0U)
      {
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
        break;
      }
      count--;
    } while (hspi->State != HAL_SPI_STATE_ABORT);
    /* Reset Timeout Counter */
    count = resetcount;
  }

  /* If DMA Tx and/or DMA Rx Handles are associated to SPI Handle, DMA Abort complete callbacks should be initialised
     before any call to DMA Abort functions */
  /* DMA Tx Handle is valid */
  if (hspi->hdmatx != NULL)
  {
    /* Set DMA Abort Complete callback if UART DMA Tx request if enabled.
       Otherwise, set it to NULL */
    if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXDMAEN))
    {
      hspi->hdmatx->XferAbortCallback = SPI_DMATxAbortCallback;
    }
    else
    {
      hspi->hdmatx->XferAbortCallback = NULL;
    }
  }
  /* DMA Rx Handle is valid */
  if (hspi->hdmarx != NULL)
  {
    /* Set DMA Abort Complete callback if UART DMA Rx request if enabled.
       Otherwise, set it to NULL */
    if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXDMAEN))
    {
      hspi->hdmarx->XferAbortCallback = SPI_DMARxAbortCallback;
    }
    else
    {
      hspi->hdmarx->XferAbortCallback = NULL;
    }
  }

  /* Disable the SPI DMA Tx request if enabled */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXDMAEN))
  {
    /* Abort the SPI DMA Tx Stream/Channel */
    if (hspi->hdmatx != NULL)
    {
      /* Abort DMA Tx Handle linked to SPI Peripheral */
      if (HAL_DMA_Abort_IT(hspi->hdmatx) != HAL_OK)
      {
        hspi->hdmatx->XferAbortCallback = NULL;
        hspi->ErrorCode = HAL_SPI_ERROR_ABORT;
      }
      else
      {
        abortcplt = 0U;
      }
    }
  }
  /* Disable the SPI DMA Rx request if enabled */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXDMAEN))
  {
    /* Abort the SPI DMA Rx Stream/Channel */
    if (hspi->hdmarx != NULL)
    {
      /* Abort DMA Rx Handle linked to SPI Peripheral */
      if (HAL_DMA_Abort_IT(hspi->hdmarx) !=  HAL_OK)
      {
        hspi->hdmarx->XferAbortCallback = NULL;
        hspi->ErrorCode = HAL_SPI_ERROR_ABORT;
      }
      else
      {
        abortcplt = 0U;
      }
    }
  }

  if (abortcplt == 1U)
  {
    /* Reset Tx and Rx transfer counters */
    hspi->RxXferCount = 0U;
    hspi->TxXferCount = 0U;

    /* Check error during Abort procedure */
    if (hspi->ErrorCode == HAL_SPI_ERROR_ABORT)
    {
      /* return HAL_Error in case of error during Abort procedure */
      errorcode = HAL_ERROR;
    }
    else
    {
      /* Reset errorCode */
      hspi->ErrorCode = HAL_SPI_ERROR_NONE;
    }

    /* Clear the Error flags in the SR register */
    __HAL_SPI_CLEAR_OVRFLAG(hspi);
    __HAL_SPI_CLEAR_FREFLAG(hspi);

    /* Restore hspi->State to Ready */
    hspi->State = HAL_SPI_STATE_READY;

    /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
    hspi->AbortCpltCallback(hspi);
#else
    HAL_SPI_AbortCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
  }

  return errorcode;
}

/**
  * @brief  Pause the DMA Transfer.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for the specified SPI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_DMAPause(SPI_HandleTypeDef *hspi)
{
  /* Process Locked */
  __HAL_LOCK(hspi);

  /* Disable the SPI DMA Tx & Rx requests */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);

  /* Process Unlocked */
  __HAL_UNLOCK(hspi);

  return HAL_OK;
}

/**
  * @brief  Resume the DMA Transfer.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for the specified SPI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_DMAResume(SPI_HandleTypeDef *hspi)
{
  /* Process Locked */
  __HAL_LOCK(hspi);

  /* Enable the SPI DMA Tx & Rx requests */
  SET_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);

  /* Process Unlocked */
  __HAL_UNLOCK(hspi);

  return HAL_OK;
}

/**
  * @brief  Stop the DMA Transfer.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for the specified SPI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_DMAStop(SPI_HandleTypeDef *hspi)
{
  HAL_StatusTypeDef errorcode = HAL_OK;
  /* The Lock is not implemented on this API to allow the user application
     to call the HAL SPI API under callbacks HAL_SPI_TxCpltCallback() or HAL_SPI_RxCpltCallback() or HAL_SPI_TxRxCpltCallback():
     when calling HAL_DMA_Abort() API the DMA TX/RX Transfer complete interrupt is generated
     and the correspond call back is executed HAL_SPI_TxCpltCallback() or HAL_SPI_RxCpltCallback() or HAL_SPI_TxRxCpltCallback()
     */

  /* Abort the SPI DMA tx Stream/Channel  */
  if (hspi->hdmatx != NULL)
  {
    if (HAL_OK != HAL_DMA_Abort(hspi->hdmatx))
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);
      errorcode = HAL_ERROR;
    }
  }
  /* Abort the SPI DMA rx Stream/Channel  */
  if (hspi->hdmarx != NULL)
  {
    if (HAL_OK != HAL_DMA_Abort(hspi->hdmarx))
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);
      errorcode = HAL_ERROR;
    }
  }


}
