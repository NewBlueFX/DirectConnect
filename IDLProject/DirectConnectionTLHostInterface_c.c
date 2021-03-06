

/* this ALWAYS GENERATED file contains the RPC client stubs */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Oct 08 12:18:24 2018
 */
/* Compiler settings for DirectConnectionTLHostInterface.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/

#include <string.h>

#include "DirectConnectionTLHostInterface_h.h"

#define TYPE_FORMAT_STRING_SIZE   51                                
#define PROC_FORMAT_STRING_SIZE   121                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _DirectConnectionTLHostInterface_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } DirectConnectionTLHostInterface_MIDL_TYPE_FORMAT_STRING;

typedef struct _DirectConnectionTLHostInterface_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } DirectConnectionTLHostInterface_MIDL_PROC_FORMAT_STRING;

typedef struct _DirectConnectionTLHostInterface_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } DirectConnectionTLHostInterface_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const DirectConnectionTLHostInterface_MIDL_TYPE_FORMAT_STRING DirectConnectionTLHostInterface__MIDL_TypeFormatString;
extern const DirectConnectionTLHostInterface_MIDL_PROC_FORMAT_STRING DirectConnectionTLHostInterface__MIDL_ProcFormatString;
extern const DirectConnectionTLHostInterface_MIDL_EXPR_FORMAT_STRING DirectConnectionTLHostInterface__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: __MIDL_itf_DirectConnectionTLHostInterface_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: DirectConnectionTLHostInterface, ver. 1.0,
   GUID={0xF6A7AAF5,0xE634,0x44A4,{0xBA,0xF5,0x8D,0x8F,0x7A,0xF2,0xFB,0x40}} */



static const RPC_CLIENT_INTERFACE DirectConnectionTLHostInterface___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0xF6A7AAF5,0xE634,0x44A4,{0xBA,0xF5,0x8D,0x8F,0x7A,0xF2,0xFB,0x40}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0x00000000
    };
RPC_IF_HANDLE DirectConnectionTLHostInterface_v1_0_c_ifspec = (RPC_IF_HANDLE)& DirectConnectionTLHostInterface___RpcClientInterface;

extern const MIDL_STUB_DESC DirectConnectionTLHostInterface_StubDesc;

static RPC_BINDING_HANDLE DirectConnectionTLHostInterface__MIDL_AutoBindHandle;


void DirectConnectionClientConnected( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ int nServerPort,
    /* [in] */ DCTLProcessID processID,
    /* [out] */ DCTLProcessID *pHostProcessId)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&DirectConnectionTLHostInterface_StubDesc,
                  (PFORMAT_STRING) &DirectConnectionTLHostInterface__MIDL_ProcFormatString.Format[0],
                  IDL_handle,
                  clientID,
                  nServerPort,
                  processID,
                  pHostProcessId);
    
}


int DirectConnectionGetHostFrame( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ DCTLClientID clientID,
    /* [in] */ DCTLInstanceID instanceID,
    /* [in] */ int nVideoFrameType,
    /* [out][in] */ DCTLVideoRenderInfoIDL *pRenderInfo,
    /* [out][in] */ DCTLVideoFrameParamsIDL *pFrame)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&DirectConnectionTLHostInterface_StubDesc,
                  (PFORMAT_STRING) &DirectConnectionTLHostInterface__MIDL_ProcFormatString.Format[54],
                  IDL_handle,
                  clientID,
                  instanceID,
                  nVideoFrameType,
                  pRenderInfo,
                  pFrame);
    return ( int  )_RetVal.Simple;
    
}


/* Standard interface: __MIDL_itf_DirectConnectionTLHostInterface_0000_0001, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const DirectConnectionTLHostInterface_MIDL_PROC_FORMAT_STRING DirectConnectionTLHostInterface__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure DirectConnectionClientConnected */

			0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 14 */	NdrFcShort( 0x20 ),	/* 32 */
/* 16 */	NdrFcShort( 0x1c ),	/* 28 */
/* 18 */	0x40,		/* Oi2 Flags:  has ext, */
			0x4,		/* 4 */
/* 20 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */
/* 28 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 30 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 34 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter clientID */

/* 36 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 38 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 40 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nServerPort */

/* 42 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 44 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 46 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter processID */

/* 48 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 50 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 52 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DirectConnectionGetHostFrame */


	/* Parameter pHostProcessId */

/* 54 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 56 */	NdrFcLong( 0x0 ),	/* 0 */
/* 60 */	NdrFcShort( 0x1 ),	/* 1 */
/* 62 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 64 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 66 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 68 */	NdrFcShort( 0x8c ),	/* 140 */
/* 70 */	NdrFcShort( 0x6c ),	/* 108 */
/* 72 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 74 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 76 */	NdrFcShort( 0x0 ),	/* 0 */
/* 78 */	NdrFcShort( 0x0 ),	/* 0 */
/* 80 */	NdrFcShort( 0x0 ),	/* 0 */
/* 82 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 84 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 86 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 88 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter clientID */

/* 90 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 92 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 94 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter instanceID */

/* 96 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 98 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nVideoFrameType */

/* 102 */	NdrFcShort( 0x11a ),	/* Flags:  must free, in, out, simple ref, */
/* 104 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 106 */	NdrFcShort( 0xa ),	/* Type Offset=10 */

	/* Parameter pRenderInfo */

/* 108 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 110 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 112 */	NdrFcShort( 0x1e ),	/* Type Offset=30 */

	/* Parameter pFrame */

/* 114 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 116 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 118 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const DirectConnectionTLHostInterface_MIDL_TYPE_FORMAT_STRING DirectConnectionTLHostInterface__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  4 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0x0,	/* FC_RP */
/*  8 */	NdrFcShort( 0x2 ),	/* Offset= 2 (10) */
/* 10 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 12 */	NdrFcShort( 0x40 ),	/* 64 */
/* 14 */	0x8,		/* FC_LONG */
			0x40,		/* FC_STRUCTPAD4 */
/* 16 */	0xc,		/* FC_DOUBLE */
			0xc,		/* FC_DOUBLE */
/* 18 */	0xc,		/* FC_DOUBLE */
			0xc,		/* FC_DOUBLE */
/* 20 */	0xc,		/* FC_DOUBLE */
			0x8,		/* FC_LONG */
/* 22 */	0x8,		/* FC_LONG */
			0xc,		/* FC_DOUBLE */
/* 24 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 26 */	
			0x11, 0x0,	/* FC_RP */
/* 28 */	NdrFcShort( 0x2 ),	/* Offset= 2 (30) */
/* 30 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 32 */	NdrFcShort( 0x20 ),	/* 32 */
/* 34 */	NdrFcShort( 0x0 ),	/* 0 */
/* 36 */	NdrFcShort( 0x0 ),	/* Offset= 0 (36) */
/* 38 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 40 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 42 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 44 */	0xd,		/* FC_ENUM16 */
			0x3,		/* FC_SMALL */
/* 46 */	0x3,		/* FC_SMALL */
			0x3e,		/* FC_STRUCTPAD2 */
/* 48 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */

			0x0
        }
    };

static const unsigned short DirectConnectionTLHostInterface_FormatStringOffsetTable[] =
    {
    0,
    54
    };


static const MIDL_STUB_DESC DirectConnectionTLHostInterface_StubDesc = 
    {
    (void *)& DirectConnectionTLHostInterface___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &DirectConnectionTLHostInterface__MIDL_AutoBindHandle,
    0,
    0,
    0,
    0,
    DirectConnectionTLHostInterface__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x800025b, /* MIDL Version 8.0.603 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

