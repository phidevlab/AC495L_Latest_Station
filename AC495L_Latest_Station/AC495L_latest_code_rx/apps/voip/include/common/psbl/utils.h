/*******************************************************************************
 * FILE NAME:       utils.h
 *
 * DESCRIPTION:     utilities definition
 *
 * (C) Copyright 2010, AudioCodes, Inc
 ******************************************************************************/

#ifndef __UTILS_H__
#define __UTILS_H__

#define SET_BIT(reg, mask)		((reg) |= (mask))
#define CLEAR_BIT(reg, mask)		((reg) &=~(mask))
#define TEST_BIT(reg, mask)		(((reg) & (mask)) == (mask))
 
#define SET_REG(reg, idx)		SET_BIT(reg, (1 << idx))
#define CLEAR_REG(reg, idx)		CLEAR_BIT(reg, (1 << idx))
 
#define TEST_REG(reg, idx)  		TEST_BIT(reg, (1 << idx))

#if 0
/* why gcc-3.3.6  no macro  __FUNCTION__ and do not cause error?   HanseH, 20100421 */
#define PRINTF(fmt, arg...)		sys_printf("[%s:%d: %s()] "fmt,  __FILE__, __LINE__, __FUNCTION__,  ##arg)
#else 
#define PRINTF(fmt, arg...)		sys_printf("[%s:%d] "fmt,  __FILE__, __LINE__,  ##arg)
#endif
  
#endif /* __UTILS_H__ */
