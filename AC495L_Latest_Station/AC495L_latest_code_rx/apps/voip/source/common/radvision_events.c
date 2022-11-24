#include <rvmdmobjects.h>
#include <rvccprovidermdm.h>
#include <rvccterminalmdm.h>
#include <RvSipOtherHeader.h>
#include <rvcctext.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "acl_call_mngr.h"
#include "acl_line_connection.h"
//#include "ipphone.h"
#include "voip_main.h"
#include "rvphone_common.h"
#include "voip_lcd.h"
#include "acl_ui_commands.h"
#include <rvlog.h>
#include "acl_rv_sip_api.h"
#include "led.h"
#include "voip_status_handler.h"
#include "iphone_fxs.h"

#include <rvMdmControlApi.h>
#include "acl_voip_conf.h"//Nirp 060719
#include <acl_telephony_adv_sip_features.h> /* ACL NB 060927 */
#include <rvIppCfwApi.h> /* ACL NB 061115 */
#include <rvmdmtermevent.h>


int send_event_to_radvision(int event)
{
    RvIppTerminalHandle connHndl;
    RvIppTerminalHandle termHndl = rvIppMdmConnGetTerminal(connHndl);
	RvMdmTerm*          mdmTerm = rvIppMdmTerminalGetMdmTerm(termHndl);
   	RvCCConnection *pActiveConn;
   	RvCCConnState state;

   	RvMdmParameterList paramsList;
    RvMdmPackageItem pkgItem;

    RvIppTerminalHandle t = rvIppMdmConnGetTerminal(connHndl);
    switch(event)
    {
        case RINGING_OFF_HOOK_EVENT:
              /*Construct local objects*/
        rvMdmParameterListConstruct(&paramsList);
        rvMdmPackageItemConstruct(&pkgItem, "", "keyid");
/* Build an Off-Hook event */
        rvMdmParameterListOr(&paramsList, &pkgItem, "kh");
/*Send Off-Hook event */
        mdmTerm = rvIppMdmTerminalGetMdmTerm(t);
        rvMdmTermProcessEvent(mdmTerm, "kf", "kd", NULL,&paramsList);
/*Destruct local objects*/
        rvMdmPackageItemDestruct(&pkgItem);
        rvMdmParameterListDestruct(&paramsList);
        break;
    }
}
