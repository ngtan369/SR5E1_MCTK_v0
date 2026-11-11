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

/* Memory map */
#include <memory_map.h>

LR_ROM LR_ROM_START_ADDR LR_ROM_MAX_SIZE
{
    STARTUP +0 ALIGNALL 1024
    {
        vectors.o (.vectors, +FIRST)
        vectors.o (.reset_handler)
    }

    ER_ROM +0 ALIGN 16
    {
        *(+RO)
    }

    ER_ITCM ER_ITCM_START_ADDR ALIGN 16 ER_ITCM_MAX_SIZE
    {
        *(.itcm)
    }

    ER_DTCM ER_DTCM_START_ADDR ALIGN 4 ER_DTCM_MAX_SIZE
    {
        *(.dtcm)
    }

    ER_DATA LR_RAM_START_ADDR ALIGN 4 NOCOMPRESS
    {
        *(+RW)
    }

    ER_BSS +0 ALIGN 8
    {
        *(+ZI)
    }

    ER_MSTACK AlignExpr(+0, 16) EMPTY ER_MSTACK_MAX_SIZE
    {
    }

    ER_PSTACK AlignExpr(+0, 16) EMPTY ER_PSTACK_MAX_SIZE
    {
    }

    ER_HEAP AlignExpr(+0, 16) EMPTY (LR_RAM_START_ADDR + LR_RAM_MAX_SIZE - (ImageLimit(ER_PSTACK)))
    {
    }
}
