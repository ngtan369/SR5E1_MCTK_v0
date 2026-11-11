/****************************************************************************
 * @file    can_manager.c
 * @brief   CAN Manager wrapper source file complying with MISRA-C rules.
 *****************************************************************************/


#include <stdint.h>
#include <stdbool.h>

/* External references to ST driver instances */
extern can_driver_t DRV_CAN1;
extern can_driver_t DRV_CAN2;
extern can_driver_t DRV_CAN3;
extern can_driver_t DRV_CAN4;

static can_driver_t * const Can_Driver_Map[SYS_CAN_PORT_MAX] =
{
    &DRV_CAN1,
    &DRV_CAN2,
    &DRV_CAN3,
    &DRV_CAN4
};

static bool Port_Is_Open[SYS_CAN_PORT_MAX] =
{
    false, false, false, false
};

/*===========================================================================*/
/* MODULE EXPORTED FUNCTIONS                                                 */
/*===========================================================================*/

void System_CAN_Init_All(void)
{
    uint8_t i;

    for (i = 0U; i < (uint8_t)SYS_CAN_PORT_MAX; i++)
    {
        can_init(Can_Driver_Map[i]);
        can_rx_config(Can_Driver_Map[i]);
        can_tx_config(Can_Driver_Map[i]);
        (void)can_set_irq_mask(Can_Driver_Map[i], CAN_IRQ_ALL_ENABLED);
        Port_Is_Open[i] = false;
    }
}

sys_can_status_t System_CAN_Open_Port(sys_can_port_t port)
{
    sys_can_status_t status = SYS_CAN_STATUS_ERR;

    if (port < SYS_CAN_PORT_MAX)
    {
        can_start(Can_Driver_Map[port]);
        Port_Is_Open[port] = true;
        status = SYS_CAN_STATUS_OK;
    }

    return status;
}

sys_can_status_t System_CAN_Send(sys_can_port_t port, uint32_t id, const uint8_t *payload, uint8_t dlc)
{
    sys_can_status_t status = SYS_CAN_STATUS_PORT_CLOSED;
    can_frame_t tx_frame;
    bool hw_status;
    uint8_t safe_dlc;

    if (port < SYS_CAN_PORT_MAX)
    {
        if (Port_Is_Open[port] == true)
        {
            safe_dlc = (dlc > 8U) ? 8U : dlc;

            tx_frame.TYPE = CAN_ID_STD;
            tx_frame.ID = id;
            tx_frame.DLC = safe_dlc;
            tx_frame.OPERATION = CAN_OPERATION_NORMAL;

            (void)memset(tx_frame.data8, 0, sizeof(tx_frame.data8));

            if ((payload != NULL) && (safe_dlc > 0U))
            {
                (void)memcpy((void *)tx_frame.data8, (const void *)payload, (size_t)safe_dlc);
            }

            hw_status = can_transmit(Can_Driver_Map[port], CAN_TX_ANY_BUFFER, &tx_frame);

            if (hw_status == true)
            {
                status = SYS_CAN_STATUS_OK;
            }
            else
            {
                status = SYS_CAN_STATUS_ERR;
            }
        }
    }
    else
    {
        status = SYS_CAN_STATUS_ERR;
    }

    return status;
}
