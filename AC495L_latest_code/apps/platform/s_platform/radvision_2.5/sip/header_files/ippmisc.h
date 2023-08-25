#ifndef _RV_IPPMISC_H
#define _RV_IPPMISC_H
#include "ipp_inc_std.h"
#include "rvaddress.h"

/*Length of the string, representing IP, used by Stack*/
#define RV_IPP_LEN_STRING_IP 51 /* sizeof("[ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255]%dd")+'\0' */
#include "rvtypes.h"
/*Number of bytes in ipv6 addess*/
#define RV_IPP_TRANSPORT_LEN_BYTES_IPV6 16 


#define RV_IPP_VALUE_NONE	-1

/************************************************************************
 * IppUtilIsNumberValid
 * purpose: check if the string is from a valid number
 * input  : tel		- the string that we are going to check
 * output :	none
 * return : rvTrue for number, rvFalse for string
 *         
 ************************************************************************/
RvBool	IppUtilIsNumberValid(IN const char *tel);



RvBool IppUtilIsIpAddress(char* str);
/************************************************************************************
 * IppUtilIsIpV4
 * ----------------------------------------------------------------------------------
 * General: check if the string is an ip4 address
 *
 * Return Value: RV_TRUE - ip4 address, RV_FALSE - not ip4 address
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   StringIp - the source string
 * Output:  (-)
 ***********************************************************************************/
RVAPI RvBool IppUtilIsIpV4(const RvChar* StringIp);

/***************************************************************************************
 * IppUtilIsIpV6
 * -------------------------------------------------------------------------------------
 * General:   checks if an ip address is of ipv6 class or ipv4 class
 * Return Value: RV_TRUE - ipv6
 *               RV_FALSE - ipv4
 * -------------------------------------------------------------------------------------
 * Arguments:
 * Input:    szAddress  - a NULL terminated the  address.
 * Output:   none
 **************************************************************************************/
RvBool IppUtilIsIpV6(IN const RvChar* szAddress);

/************************************************************************************
 * IppUtilConvertString2Address
 * ----------------------------------------------------------------------------------
 * General: make RV_LI_AddressType from string
 *
 * Return Value: none
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   StringIp - the source string
 *          bIsIpV4   - Defines if the address is ip4 or ip6
 *      
 * Output:  address - the address in RV_LI_AddressType format
 ***********************************************************************************/
RvStatus IppUtilConvertString2Address(IN  RvChar*                StringIp,
                                      OUT RvAddress*			 address,
                                      IN  RvBool                 bIsIpV4);

/************************************************************************************
 * IppUtilRemoveScopeIdFromIpv6String
 * ----------------------------------------------------------------------------------
 * General: 
 *
 * Return Value: none
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   StringIp - the source string IPv6
 *          
 *      
 * Output:  string - input parameter without scope id
 ***********************************************************************************/
void IppUtilRemoveScopeIdFromIpv6String(OUT RvChar* StringIp);

/************************************************************************************
 * IppUtilRemoveBracketsFromIpv6String
 * ----------------------------------------------------------------------------------
 * General: 
 *
 * Return Value: input parameter without square brackets
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   StringIp - the source string IPv6
 *          
 *      
 * Output:  string - none
 ***********************************************************************************/
void IppUtilRemoveBracketsFromIpv6String(IN  RvChar*  StringIp);

/************************************************************************************
 * RvAddressConstructFromString
 * ----------------------------------------------------------------------------------
 * General: gets address in string format (ipv6/ipv4) and construct RvAddress
 *
 * Return Value: none
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   ipString - address in string format
 *          addr - the address in RV_LI_AddressType format
 *      
 * Output:  addr - the address in RV_LI_AddressType format
 ***********************************************************************************/
RVAPI RvStatus RvAddressConstructFromString(IN RvChar* ipString, INOUT RvAddress *addr);

/************************************************************************************
 * RvAddressConstructCopy
 * ----------------------------------------------------------------------------------
 * General: construct source address and copy address from source address
 *
 * Return Value: RvStatus
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   addrSrc  - pointer to the source address
 *          addrDest - pointer to the destination address
 *			addrtype - address type (IPv4 or IPv6)
 *      
 * Output:  addr - the source address in RV_LI_AddressType format
 ***********************************************************************************/
RVAPI RvStatus RvAddressConstructCopy(IN RvAddress* addrSrc,OUT RvAddress* addrDest);

/************************************************************************************
 * IppUtilPrintMemoryAllocations
 * ----------------------------------------------------------------------------------
 * General: This function prints to log (Core - Memory module, Debug level) all
 *			memory allocations made so far and the file name who called them.
 *
 * Return Value: RvStatus
 * ----------------------------------------------------------------------------------
 * Arguments:
 * Input:   None
 *      
 * Output:  None
 ***********************************************************************************/
RVAPI RvStatus IppUtilPrintMemoryAllocations(void);

/***************************************************************************
 * IppPrintSdp
 * ------------------------------------------------------------------------
 * General: The function prints SDP message as text.
 * Return Value: True if SDP was successfuly parsed, False if not.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: sdp - a pointer to SDP message
 *        title	 - a string to be printed before the SDP message.
 *		  
 ***************************************************************************/
RVAPI RvBool	IppPrintSdp( RvSdpMsg *sdp, const char* title);

/***************************************************************************
 * IppUtil_itoa
 * ------------------------------------------------------------------------
 * General: The function convert integer to string.
 * Return Value: None.
 * ------------------------------------------------------------------------
 * Arguments:
 * Input: number - The integer value.
 *        str	 - The buffer that will contain the string of the integer.
 *		  
 ***************************************************************************/
RVAPI char *IppUtil_itoa(int number, char *str);

#endif

