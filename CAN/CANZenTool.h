/**
 * CANZenTool.h
 *
 * Author: Carlos Craveiro (@CarlosCraveiro)
 * Created On: September 25, 2021
 *
 * Brief Description:
 * */

#include "stm32f1xx_hal_can.h"


/**
 * @brief:
 *
 * @param
 * @param
 * @param
 *
 * */

void CANZenTool_setFilter(CAN_HandleTypeDef* hcan, CAN_FilterTypeDef* canfilterconfig , uint8_t isActive, uint32_t filterBank, uint32_t filterId, uint32_t filterMaskId);


/**
 * @brief:
 *
 * @param:
 * @param:
 * @param:
 *
 * @return:
 * */

 CAN_TxHeaderTypeDef CANZenTool_writeStdCanFrame(uint32_t dlc, uint32_t id, uint8_t isData);


/**
 * @brief:
 *
 * @param:
 * @param:
 *
 * */

void CANZenTool_sendCanFrameMsg(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *TxHeader, uint8_t TxData[], uint32_t *pTxMailbox);

