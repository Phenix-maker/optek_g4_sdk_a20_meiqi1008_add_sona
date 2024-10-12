#ifndef PROX_REPORTER_H
#define PROX_REPORTER_H


// Proximity Profile Services bit fields
#define PP_LINK_LOSS_SERVICE            0x00000001 // Link Loss Service
#define PP_IM_ALETR_SERVICE             0x00000002 // Immediate Alert Service
#define PP_TX_PWR_LEVEL_SERVICE         0x00000004 // Tx Power Level Service

extern bStatus_t ProxReporter_AddService( uint32_t services );

#endif