#ifndef SILBAS_WRAPPER_H
#define SILBAS_WRAPPER_H


int fxs_initialize(void);
int fxs_PCMStart(int SlicNo);
int fxs_PCMStop(int SlicNo);
 int fxs_RingStart(int SlicNo);
 int fxs_RingStop(int SlicNo);
int  fxs_ReadHookStatus(int SlicNo,  unsigned char *p_hookStat);
int IsValidSlic(int SlicNo);

#endif
