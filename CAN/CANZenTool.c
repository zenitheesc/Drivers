/**
 * CANZenTool.c
 *
 * Author: Carlos Craveiro (@CarlosCraveiro)
 * Created On: September 25, 2021
 *
 * Brief Description:	The code of the library to abstract some overcomplicated functionalities 
 * 						of "stm32f1xx_hal_can.h" to the standard projects developed by the 
 * 						Zenith's Low Level Programming Team.
 */

#include "CANZenTool.h"
#include <stdbool.h>

void CANZenTool_setFilter(CAN_HandleTypeDef* hcan, CAN_FilterTypeDef* canFilterConfig , bool isActive, uint32_t filterBank, uint32_t filterId, uint32_t filterMaskId) {
	
	if(isActive) {
		
		canfilterconfig->FilterActivation = CAN_FILTER_ENABLE;
	} else {
		
		canfilterconfig->FilterActivation = CAN_FILTER_DISABLE;
	}
	canFilterConfig->FilterBank = filterBank;
	canFilterConfig->FilterFIFOAssignment = CAN_RX_FIFO0;
	canFilterConfig->FilterIdHigh = filterId << 5;
	canFilterConfig->FilterIdLow = 0x0000;
	canFilterConfig->FilterMaskIdHigh = filterMaskId << 5;
	canFilterConfig->FilterMaskIdLow = 0x0000;
	canFilterConfig->FilterMode = CAN_FILTERMODE_IDMASK;
	canFilterConfig->SlaveStartFilterBank = 0;
	
	HAL_CAN_ConfigFilter(&hcan, &canFilterConfig);
}


CAN_TxHeaderTypeDef CANZenTool_writeStdCanFrame(uint32_t dlc, uint32_t id, bool isData) {

	CAN_TxHeaderTypeDef TxHeader;

	TxHeader.DLC = dlc;
	TxHeader.ExtId = 0;
	TxHeader.IDE = CAN_ID_STD;
	
	if (isData) {
		
		TxHeader.RTR = CAN_RTR_DATA;
	} else {
		
		TxHeader.RTR = CAN_RTR_REMOTE;
	}
	
	TxHeader.StdId = id;
	TxHeader.TransmitGlobalTime = DISABLE;

	return TxHearder;
}


void CANZenTool_sendCanFrameMsg(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *TxHeader, uint8_t TxData[], uint32_t *pTxMailbox) {

	 if(HAL_CAN_AddTxMessage(hcan, TxHeader, TxData, TxMailbox) != HAL_OK) {
	  Error_Handler();
  	}
}

