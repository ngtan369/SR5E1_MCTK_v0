
#include "r_divider_bus_voltage_sensor.h"

/* Project override of the SDK's weak RVBS_CheckFaultState. */
uint16_t RVBS_CheckFaultState(RDivider_Handle_t *pHandle)
{
    uint16_t fault;

    if (pHandle->_Super.AvBusVoltage_d > pHandle->OverVoltageThreshold)
    {
        /* fault = MC_OVER_VOLT; */
        fault = MC_NO_ERROR;
    }
    else if (pHandle->_Super.AvBusVoltage_d < pHandle->UnderVoltageThreshold)
    {
        /* fault = MC_UNDER_VOLT; */
        fault = MC_NO_ERROR;
    }
    else
    {
        fault = MC_NO_ERROR;
    }

    return fault;
}
