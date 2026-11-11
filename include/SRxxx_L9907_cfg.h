/*
 ******************************************************************************
 * @attention
 *
  * <h2><center>&copy; COPYRIGHT M4_COPYRIGHT_YEAR STMicroelectronics</center></h2>
 *
 * Licensed under ADG License Agreement, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
  *        M4_COPYRIGHT_WEBLINK
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. EVALUATION ONLY – NOT FOR USE IN PRODUCTION
 *
 ******************************************************************************
*/

/*
*   @file    SRxxx_L9907_cfg.h
*   @version M4_VERSION
*   @brief   L9907 configuration macros and function
*          
*   @details L9907 configuration macros and function.
*
*/

 
#ifndef __SRxxx_L9907_CFG_H__
#define __SRxxx_L9907_CFG_H__


 
/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/


#define L9907_CMD0_CONFIGURATION             0x04C9U    /* 0b0000010011001001 */
#define L9907_CMD1_CONFIGURATION             0x3305U    /* 0b0011001100000101 */
#define L9907_CMD2_CONFIGURATION             0x4200U    /* 0b0100001000000000 */
#define L9907_CMD3_CONFIGURATION             0x6000U    /* 0b0110000000000000 */
#define L9907_CMD4_CONFIGURATION             0x83FFU    /* 0b1000001111111111 */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern void L9907_faultRecovery(void);

#endif /* __SRxxx_L9907_CFG_H__ */
