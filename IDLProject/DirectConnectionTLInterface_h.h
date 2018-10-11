

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Oct 08 12:18:26 2018
 */
/* Compiler settings for DirectConnectionTLInterface.idl:
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


#ifndef __DirectConnectionTLInterface_h_h__
#define __DirectConnectionTLInterface_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

/* header files for imported files */
#include "DirectConnectionTLTypes.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_DirectConnectionTLInterface_0000_0000 */
/* [local] */ 

#pragma pack( push, 4 )


extern RPC_IF_HANDLE __MIDL_itf_DirectConnectionTLInterface_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DirectConnectionTLInterface_0000_0000_v0_0_s_ifspec;

#ifndef __DirectConnectionTLInterface_INTERFACE_DEFINED__
#define __DirectConnectionTLInterface_INTERFACE_DEFINED__

/* interface DirectConnectionTLInterface */
/* [version][uuid] */ 

int DirectConnectionInitialize( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ DCTLInstanceID instanceID,
    /* [in] */ int nDCChannel,
    /* [in] */ int nWidth,
    /* [in] */ int nHeight,
    /* [in] */ double dFrameRate,
    /* [in] */ double dPixelAspectRatio,
    /* [in] */ boolean bInterlaced,
    /* [in] */ int nSampleRate,
    /* [in] */ int nNumChannels,
    /* [out] */ double *pdStartTime);

int DirectConnectionGetVideoFrame( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ DCTLInstanceID instanceID,
    /* [in] */ int nDCChannel,
    /* [out][in] */ DCTLVideoFrameParamsIDL *pFrame,
    /* [in] */ double dTime);

int DirectConnectionGetAudioFrame( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ DCTLInstanceID instanceID,
    /* [in] */ int nDCChannel,
    /* [out][in] */ DCTLAudioFrameParamsIDL *pFrame,
    /* [in] */ double dTime);

int DirectConnectionGetVideoSettings( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ DCTLInstanceID instanceID,
    /* [in] */ int nDCChannel,
    /* [out] */ int *pnWidth,
    /* [out] */ int *pnHeight,
    /* [out] */ double *pdFrameRate,
    /* [out] */ double *pdPixelAspectRatio,
    /* [out] */ boolean *pbInterlaced);

int DirectConnectionGetAudioSettings( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ DCTLInstanceID instanceID,
    /* [in] */ int nDCChannel,
    /* [out] */ int *pnSampleRate,
    /* [out] */ int *pnNumChannels);

int DirectConnectionGetMetadataFrame( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ DCTLInstanceID instanceID,
    /* [in] */ int nDCChannel,
    /* [out] */ long *plBufferSize,
    /* [size_is][size_is][out] */ unsigned char **ppOutBuffer);

int DirectConnectionSendMetadataFrame( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ DCTLInstanceID instanceID,
    /* [in] */ int nDCChannel,
    /* [string][in] */ const unsigned char *pszData);

void DirectConnectionClientConnect( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ int nServerPort);

void DirectConnectionClientDisconnect( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ int nServerPort);

void DirectConnectionCreateInstance( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ DCTLInstanceID instanceID);

void DirectConnectionDestroyInstance( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ DCTLInstanceID instanceID);

int DirectConnectionFreeSharedMemory( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ DCTLInstanceID instanceID,
    /* [in] */ DCTLSharedMemoryType type);

int DirectConnectionSendGlobalMetadataFrame( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ int nDCChannel,
    /* [string][in] */ const unsigned char *pszData);



extern RPC_IF_HANDLE DirectConnectionTLInterface_v1_0_c_ifspec;
extern RPC_IF_HANDLE DirectConnectionTLInterface_v1_0_s_ifspec;
#endif /* __DirectConnectionTLInterface_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_DirectConnectionTLInterface_0000_0001 */
/* [local] */ 

#pragma pack( pop )


extern RPC_IF_HANDLE __MIDL_itf_DirectConnectionTLInterface_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DirectConnectionTLInterface_0000_0001_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


