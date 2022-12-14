/* vp_hal.h
 *
 * This file defines the interface between the VoicePath API and the Hardware
 * Abstraction Layer (HAL).  The types and functions declared in this file are
 * platform-dependent.  The functions are defined in ve_hal.c.  Porting the
 * VoicePath API to a new host processor consists of modifying the typedefs
 * in this file, setting the HBI_PINCONFIG define below, and modifying the
 * function implementations in vp_hal.c.
 *
 * Copyright (c) 2004, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef _VP_HAL_H
#define _VP_HAL_H

#include "vp_api_types.h"
#include "hbi_hal.h"
#include "mpi_hal.h"
#include "sys_service.h"

/* The following definitions have been added to redirect the output of
 * debug printf statements from the application and the API */
#define term_printf printk
#define debug_printf printk

#endif /* _VP_HAL_H */

