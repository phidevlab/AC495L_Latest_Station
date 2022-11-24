/******************************************************************************

  Copyright (C), 2001-2011, DCN Co., Ltd.

 ******************************************************************************
  File Name     : iphone_paging.c
  Version       : Initial Draft
  Author        : Andy.Deng
  Created       : 2017/6/20
  Last Modified :
  Description   : paging
  Function List :
              acl_create_paging_thread
  History       :
  1.Date        : 2017/6/20
    Author      : Andy.Deng
    Modification: Created file

******************************************************************************/
#include "general_defs.h"
#include <stdio.h>
#include <stdlib.h>


static PAGING_CONFIG    pConfig;
static PAGING_WORKING   pWork;


static PAGING_STREAM * paging_get_stream(unsigned int ssrc, int group)
{
    int i,total = sizeof(pWork.stream)/sizeof(PAGING_STREAM);

    for (i=0;i<total;i++)
    {
        if (pWork.stream[i].SSRC != 0 && pWork.stream[i].SSRC == ssrc)
        {
            return &pWork.stream[i];
        }
    }

    // if running here, no matched find, return a new one.
    for (i=0;i<total;i++)
    {
        if (pWork.stream[i].SSRC == 0)
        {
            pWork.stream[i].SSRC  = ssrc;
            pWork.stream[i].group = group;

            return &pWork.stream[i];
        }
    }

    // if runing here, no enough resoure.
    printf("There is no enough place to save stream, please expand the value of MAX_PAGING_STREAM");
    return NULL;
}









