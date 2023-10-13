#ifndef CORE_BT_SWITCH_H
#define CORE_BT_SWITCH_H

// Include any necessary includes from HOJA backend
#include "hoja_includes.h"

extern ns_subcore_t _ns_subcore;
extern uint8_t ns_hostAddress[6];
extern bool ns_connected;

hoja_err_t core_ns_set_subcore(ns_subcore_t subcore);

// Start the Nintendo Switch controller core
hoja_err_t core_ns_start(void);

// Stop the Nintendo Switch controller core
void core_ns_stop(void);

hoja_err_t ns_savepairing(uint8_t* host_addr);

hoja_err_t ns_startpairing(void);

#endif