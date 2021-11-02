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

/**
 * @brief	Sets the configurations parameters passed in the function call 
 * 			and  other standard settings in a CAN_FilterTypeDef 
 * 			named canFilterConfigs.
 *	
 * @param	hcan pointer to a CAN_HandleTypeDef structure that contains
 *			the configuration information for the specified CAN.
 *
 * @param	canFilterConfig pointer to a CAN_FilterTypeDef structure that
 *			is going to recieve the filter configurations setted in 
 *			this function.
 *
 * @param	isActive bool that specifies either the Filter will be active 
 * 			or not.
 *
 * @param	filterBank unsigned int of 32 bits that specifies the filter 
 * 			bank which will be initialized.For single CAN instance
 * 			(14 dedicated filter banks), this parameter must be a number 
 * 			between Min_Data = 0 and Max_Data = 13.
 *
 * @param	filterId specifies the filter identification number. 
 * 			This parameter must be a number between 
 * 			Min_Data = 0x000 and Max_Data = 0x7FF.
 *
 * @param	filterMaskId specifies the filter mask number.
 * 			This parameter must be a number between 
 * 			Min_Data = 0x000 and Max_Data = 0x7FF.
 */
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
    canFilterConfig->FilterScale = CAN_FILTERSCALE_32BIT;
	canFilterConfig->SlaveStartFilterBank = 0;

	HAL_CAN_ConfigFilter(&hcan, &canFilterConfig);
}


/**
 * @brief   "adds" a standard CAN Id to the "builder" refered as "this". The addition "re-calculates"
 *          the new Id and Mask inside the "builder" structure based in the income Id.
 *          Note: this function can be both used as a "method" of the "builder" structure 
 *          or as a stand-alone function.
 *
 * @param   this pointer to a CANZenTool_IdNdMaskBuilder_t structure (proto-class). 
 *
 * @param   newId unsigned integer of 32 bits that represents a standard CAN Id following a Big-endian convention.
 */
void CANZenTool_addId(CANZenTool_IdNdMaskBuilder_t* this, uint32_t newId) {
    
    this->m_idBuffer &= newId;
    this->m_maskBuffer = (this->m_rawIds == 0)? this->m_maskBuffer : (this->m_maskBuffer & (~(this->m_idBuffer ^ newId)));
    this->m_rawIds++;

}


/**
 * @brief   "adds" a list of standard CAN Ids to the "builder" refered as "this". 
 *          The addition "re-calculates" the new Id and Mask inside the "builder" structure 
 *          based in the income Ids. Note: this function can be both used as a "method" of 
 *          the "builder" structure or as a stand-alone function.
 *
 * @param   idListLength unsigned integer of 32 bits that represents the length of the income idList. 
 *
 * @param   idList (pointer to uint32_t) array of standard CAN Ids following the Big-endian convention. 
 */
void CANZenTool_addIdList(CANZenTool_IdNdMaskBuilder_t* this,  uint32_t idListLength, uint32_t idList[]) {
   
    for(int i = 0; i < idListLength; i++) {
       
       this->addId(this, idList[i]);
   } 
}


/**
 * @brief   gets the result Mask constructed in the "builder" structure (proto-class).
 *
 * @param   this pointer to a CANZenTool_IdNdMaskBuilder_t structure (proto-class). 
 *
 * @return  the result Mask (unsigned int of 32 bits) constructed in the "builder" structure.
 */
uint32_t CANZenTool_getResultMask(CANZenTool_IdNdMaskBuilder_t* this) {

    return this->m_maskBuffer;
}


/**
 * @brief   gets the result Id constructed in the "builder" structure (proto-class).
 *
 * @param   this pointer to a CANZenTool_IdNdMaskBuilder_t structure (proto-class).
 *
 * @return  the result Id (unsigned int of 32 bits) constructed in the "builder" structure.
 */
uint32_t CANZenTool_getResultId(CANZenTool_IdNdMaskBuilder_t* this) {

    return this->m_idBuffer;
}


/**
 * @brief   creates a new "builder" structure (proto-class) of type @ref CANZenTool_IdNdMaskBuilder_t.
 *
 * @return  a new "builder" structure (proto-class) @ref CANZenTool_IdNdMaskBuilder_t.
 */
CANZenTool_IdNdMaskBuilder_t CANZenTool_newFilterBuilder() {

    CANZenTool_IdNdMaskBuilder_t newBuilder;
    newBuilder.addId = &CANZenTool_addId;
    newBuilder.addIdList = &CANZenTool_addIdList;
    newBuilder.getResultId = &CANZenTool_getResultId;
    newBuilder.getResultMask = &CANZenTool_getResultMask;
    newBuilder.m_maskBuffer = 0xFFFFFFFF;
    newBuilder.m_idBuffer = 0xFFFFFFFF;
    newBuilder.m_rawIds = 0;

    return newBuilder;
}


/**
 * @brief	Writes a CAN Standard Frame with the parameters passed in the function call.
 *
 * @param	dlc specifies the length of the frame that will be transmitted. 
 * 			This parameter must be a number between Min_Data = 0 and Max_Data = 8.
 * 
 * @param	id specifies the standard identifier. This parameter must be 
 * 			a number between Min_Data = 0 and Max_Data = 0x7FF.
 * 
 * @param	isData bool that specifies either the CanFrame will be 
 * 			a data frame or not (remote frame).
 *
 * @return 	The writed CAN Frame.
 */
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


/**
 * @brief	Sends the CANFrame specified in the function call (value of pointer TxHeader) 
 * 			and call Error_Handler if some error occurred during the transmission.
 *
 * @param	hcan pointer to an CAN_HandleTypeDef structure that contains
 * 			the configuration information for the specified CAN.
 *
 * @param	TxHeader pointer to a CAN_TxHeaderTypeDef structure.
 *
 * @param	TxData array containing the payload of the Tx frame.
 *
 * @param	pTxMailbox pointer to a variable where the function will 
 * 			return the TxMailbox used to store the Tx message.
 */
void CANZenTool_sendCanFrameMsg(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *TxHeader, uint8_t TxData[], uint32_t *pTxMailbox) {

	 if(HAL_CAN_AddTxMessage(hcan, TxHeader, TxData, TxMailbox) != HAL_OK) {
	  Error_Handler();
  	}
}

