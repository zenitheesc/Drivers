/**
 * CANZenTool.c
 *
 * Author: Carlos Craveiro (@CarlosCraveiro)
 * Created On: September 25, 2021
 *
 * Brief Description:
 * */

#include "CANZenTool.h"

void CANZenTool_setFilter(CAN_HandleTypeDef* hcan, CAN_FilterTypeDef* canfilterconfig , uint8_t isActive, uint32_t filterBank, uint32_t filterId, uint32_t filterMaskId) {
	
	if(isActive) {
		
		canfilterconfig->FilterActivation = CAN_FILTER_ENABLE;
	} else {
		
		canfilterconfig->FilterActivation = CAN_FILTER_DISABLE;
	}
	canfilterconfig->FilterBank = filterBank;
	canfilterconfig->FilterFIFOAssignment = CAN_RX_FIFO0;
	canfilterconfig->FilterIdHigh = filterId << 5;
	canfilterconfig->FilterIdLow = 0x0000;
	canfilterconfig->FilterMaskIdHigh = filterMaskId << 5;
	canfilterconfig->FilterMaskIdLow = 0x0000;
	canfilterconfig->FilterMode = CAN_FILTERMODE_IDMASK;
	canfilterconfig->SlaveStartFilterBank = 0;
	
	HAL_CAN_ConfigFilter(&hcan, &canfilterconfig);
}


CAN_TxHeaderTypeDef CANZenTool_writeStdCanFrame(uint32_t dlc, uint32_t id, uint8_t isData) {

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

