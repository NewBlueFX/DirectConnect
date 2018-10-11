

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Sat Sep 29 13:46:07 2018
 */
/* Compiler settings for ..\..\IDLProject\DirectConnectionTLHostInterface.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __DirectConnectionTLHostInterface_h_h__
#define __DirectConnectionTLHostInterface_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

/* header files for imported files */
#include "DirectConnectionTLTypes.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_DirectConnectionTLHostInterface_0000_0000 */
/* [local] */ 

#pragma pack( push, 4 )


extern RPC_IF_HANDLE __MIDL_itf_DirectConnectionTLHostInterface_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DirectConnectionTLHostInterface_0000_0000_v0_0_s_ifspec;

#ifndef __DirectConnectionTLHostInterface_INTERFACE_DEFINED__
#define __DirectConnectionTLHostInterface_INTERFACE_DEFINED__

/* interface DirectConnectionTLHostInterface */
/* [version][uuid] */ 

void DirectConnectionClientConnected( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ int nServerPort,
    /* [in] */ DCTLProcessID processID,
    /* [out] */ DCTLProcessID *pHostProcessId);

int DirectConnectionGetHostFrame( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ DCTLInstanceID instanceID,
    /* [in] */ int nVideoFrameType,
    /* [out][in] */ DCTLVideoRenderInfoIDL *pRenderInfo,
    /* [out][in] */ DCTLVideoFrameParamsIDL *pFrame);



extern RPC_IF_HANDLE DirectConnectionTLHostInterface_v1_0_c_ifspec;
extern RPC_IF_HANDLE DirectConnectionTLHostInterface_v1_0_s_ifspec;
#endif /* __DirectConnectionTLHostInterface_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_DirectConnectionTLHostInterface_0000_0001 */
/* [local] */ 

#pragma pack( pop )


extern RPC_IF_HANDLE __MIDL_itf_DirectConnectionTLHostInterface_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DirectConnectionTLHostInterface_0000_0001_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


