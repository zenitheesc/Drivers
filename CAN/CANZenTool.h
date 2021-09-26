/**
 * CANZenTool.h
 *
 * Author: Carlos Craveiro (@CarlosCraveiro)
 * Created On: September 25, 2021
 *
 * Brief Description:	A header for the library to abstract some overcomplicated functionalities 
 * 						of "stm32f1xx_hal_can.h" to the standard projects developed by the 
 * 						Zenith's Low Level Programming Team.
 * */

#include "stm32f1xx_hal_can.h"
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
void CANZenTool_setFilter(CAN_HandleTypeDef* hcan, CAN_FilterTypeDef* canFilterConfig , bool isActive, uint32_t filterBank, uint32_t filterId, uint32_t filterMaskId);


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
 CAN_TxHeaderTypeDef CANZenTool_writeStdCanFrame(uint32_t dlc, uint32_t id, bool isData);


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
void CANZenTool_sendCanFrameMsg(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *TxHeader, uint8_t TxData[], uint32_t *pTxMailbox);

