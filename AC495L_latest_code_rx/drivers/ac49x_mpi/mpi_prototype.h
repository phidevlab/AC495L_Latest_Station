#ifndef __P_MPI_DRV_H__
#define __P_MPI_DRV_H__
void VpMpiCmd(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char *);
#ifdef WINBOND_SLIC_EN
void VpPxMpiRead(unsigned char deviceId, unsigned char address, unsigned char paramLen, unsigned char *paramPtr);
void VpPxMpiWrite(unsigned char deviceId, unsigned char address, unsigned char paramLen, unsigned char *paramPtr);
void VpPxMpiBurstWrite(unsigned char deviceId, unsigned char address, unsigned char paramLen, unsigned char *paramPtr);
#endif

#endif