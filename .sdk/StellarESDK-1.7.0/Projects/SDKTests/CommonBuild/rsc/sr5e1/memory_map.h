/****************************************************************************
 *
 * Copyright (c) 2023 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com.
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *****************************************************************************/

/* SR5E1 memory map for ARMCLANG */

/* core1 nvm base address */
#define core1_nvm               0x08000000
/* core1 nvm size (960k) */
#define core1_nvm_size          0xF0000
/* core2 nvm base address */
#define core2_nvm               0x080F0000
/* core2 nvm size (960k) */
#define core2_nvm_size          0xF0000

/* core1 ram base address */
#define core1_ram               0x24000000
/* core1 ram size (128k) */
#define core1_ram_size          0x20000
/* core2 ram base address */
#define core2_ram               0x24020000
/* core2 ram size (128k) */
#define core2_ram_size          0x20000

/* itcm1 direct access base address */
#define core1_itcm_dir          0x00000000
/* itcm1 indirect access base address */
#define core1_itcm_ind          0x5A000000
/* itcm1 size (32k) */
#define core1_itcm_size         0x8000

/* dtcm1 direct access base address */
#define core1_dtcm_dir          0x20000000
/* dtcm1 indirect access base address */
#define core1_dtcm_ind          0x5C000000
/* dtcm1 size (64k) */
#define core1_dtcm_size         0x10000

/* itcm2 direct access base address */
#define core2_itcm_dir          0x00000000
/* itcm2 indirect access base address */
#define core2_itcm_ind          0x5A040000
/* itcm2 size (32k) */
#define core2_itcm_size         0x8000

/* dtcm2 direct access base address */
#define core2_dtcm_dir          0x20000000
/* dtcm2 indirect access base address */
#define core2_dtcm_ind          0x5C040000
/* dtcm2 size (64k) */
#define core2_dtcm_size         0x10000