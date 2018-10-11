

/* this ALWAYS GENERATED file contains the RPC server stubs */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Sat Sep 29 13:46:07 2018
 */
/* Compiler settings for ..\..\IDLProject\DirectConnectionTLInterface.idl:
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
#include "DirectConnectionTLInterface_h.h"

#define TYPE_FORMAT_STRING_SIZE   75                                
#define PROC_FORMAT_STRING_SIZE   781                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _DirectConnectionTLInterface_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } DirectConnectionTLInterface_MIDL_TYPE_FORMAT_STRING;

typedef struct _DirectConnectionTLInterface_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } DirectConnectionTLInterface_MIDL_PROC_FORMAT_STRING;

typedef struct _DirectConnectionTLInterface_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } DirectConnectionTLInterface_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

extern const DirectConnectionTLInterface_MIDL_TYPE_FORMAT_STRING DirectConnectionTLInterface__MIDL_TypeFormatString;
extern const DirectConnectionTLInterface_MIDL_PROC_FORMAT_STRING DirectConnectionTLInterface__MIDL_ProcFormatString;
extern const DirectConnectionTLInterface_MIDL_EXPR_FORMAT_STRING DirectConnectionTLInterface__MIDL_ExprFormatString;

/* Standard interface: __MIDL_itf_DirectConnectionTLInterface_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: DirectConnectionTLInterface, ver. 1.0,
   GUID={0xE04F87AC,0x4493,0x4155,{0x90,0xA5,0xF4,0x5F,0x41,0x6C,0x80,0x08}} */


extern const MIDL_SERVER_INFO DirectConnectionTLInterface_ServerInfo;

extern const RPC_DISPATCH_TABLE DirectConnectionTLInterface_v1_0_DispatchTable;

static const RPC_SERVER_INTERFACE DirectConnectionTLInterface___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0xE04F87AC,0x4493,0x4155,{0x90,0xA5,0xF4,0x5F,0x41,0x6C,0x80,0x08}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    (RPC_DISPATCH_TABLE*)&DirectConnectionTLInterface_v1_0_DispatchTable,
    0,
    0,
    0,
    &DirectConnectionTLInterface_ServerInfo,
    0x04000000
    };
RPC_IF_HANDLE DirectConnectionTLInterface_v1_0_s_ifspec = (RPC_IF_HANDLE)& DirectConnectionTLInterface___RpcServerInterface;

extern const MIDL_STUB_DESC DirectConnectionTLInterface_StubDesc;


/* Standard interface: __MIDL_itf_DirectConnectionTLInterface_0000_0001, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const DirectConnectionTLInterface_MIDL_PROC_FORMAT_STRING DirectConnectionTLInterface__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure DirectConnectionInitialize */

			0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x68 ),	/* X64 Stack size/offset = 104 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 14 */	NdrFcShort( 0x6d ),	/* 109 */
/* 16 */	NdrFcShort( 0x2c ),	/* 44 */
/* 18 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0xc,		/* 12 */
/* 20 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */
/* 28 */	NdrFcShort( 0xa000 ),	/* -24576 */

	/* Parameter IDL_handle */

/* 30 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 34 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter clientID */

/* 36 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 38 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 40 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter instanceID */

/* 42 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 44 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 46 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nDCChannel */

/* 48 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 50 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 52 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nWidth */

/* 54 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 56 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 58 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nHeight */

/* 60 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 62 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 64 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter dFrameRate */

/* 66 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 68 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 70 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter dPixelAspectRatio */

/* 72 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 74 */	NdrFcShort( 0x40 ),	/* X64 Stack size/offset = 64 */
/* 76 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Parameter bInterlaced */

/* 78 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 80 */	NdrFcShort( 0x48 ),	/* X64 Stack size/offset = 72 */
/* 82 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nSampleRate */

/* 84 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 86 */	NdrFcShort( 0x50 ),	/* X64 Stack size/offset = 80 */
/* 88 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nNumChannels */

/* 90 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 92 */	NdrFcShort( 0x58 ),	/* X64 Stack size/offset = 88 */
/* 94 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pdStartTime */

/* 96 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 98 */	NdrFcShort( 0x60 ),	/* X64 Stack size/offset = 96 */
/* 100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DirectConnectionGetVideoFrame */


	/* Return value */

/* 102 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 104 */	NdrFcLong( 0x0 ),	/* 0 */
/* 108 */	NdrFcShort( 0x1 ),	/* 1 */
/* 110 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 112 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 114 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 116 */	NdrFcShort( 0x38 ),	/* 56 */
/* 118 */	NdrFcShort( 0x8 ),	/* 8 */
/* 120 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 122 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 124 */	NdrFcShort( 0x0 ),	/* 0 */
/* 126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x800 ),	/* 2048 */

	/* Parameter IDL_handle */

/* 132 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 134 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 136 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter clientID */

/* 138 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 140 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 142 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter instanceID */

/* 144 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 146 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 148 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nDCChannel */

/* 150 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 152 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 154 */	NdrFcShort( 0xa ),	/* Type Offset=10 */

	/* Parameter pFrame */

/* 156 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 158 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 160 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter dTime */

/* 162 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 164 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 166 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DirectConnectionGetAudioFrame */


	/* Return value */

/* 168 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 170 */	NdrFcLong( 0x0 ),	/* 0 */
/* 174 */	NdrFcShort( 0x2 ),	/* 2 */
/* 176 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 178 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 180 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 182 */	NdrFcShort( 0x68 ),	/* 104 */
/* 184 */	NdrFcShort( 0x38 ),	/* 56 */
/* 186 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x6,		/* 6 */
/* 188 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 194 */	NdrFcShort( 0x0 ),	/* 0 */
/* 196 */	NdrFcShort( 0x800 ),	/* 2048 */

	/* Parameter IDL_handle */

/* 198 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 200 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 202 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter clientID */

/* 204 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 206 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 208 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter instanceID */

/* 210 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 212 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nDCChannel */

/* 216 */	NdrFcShort( 0x11a ),	/* Flags:  must free, in, out, simple ref, */
/* 218 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 220 */	NdrFcShort( 0x22 ),	/* Type Offset=34 */

	/* Parameter pFrame */

/* 222 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 224 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 226 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter dTime */

/* 228 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 230 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 232 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DirectConnectionGetVideoSettings */


	/* Return value */

/* 234 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 236 */	NdrFcLong( 0x0 ),	/* 0 */
/* 240 */	NdrFcShort( 0x3 ),	/* 3 */
/* 242 */	NdrFcShort( 0x50 ),	/* X64 Stack size/offset = 80 */
/* 244 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 246 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 248 */	NdrFcShort( 0x28 ),	/* 40 */
/* 250 */	NdrFcShort( 0xa1 ),	/* 161 */
/* 252 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x9,		/* 9 */
/* 254 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 256 */	NdrFcShort( 0x0 ),	/* 0 */
/* 258 */	NdrFcShort( 0x0 ),	/* 0 */
/* 260 */	NdrFcShort( 0x0 ),	/* 0 */
/* 262 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 264 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 266 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 268 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter clientID */

/* 270 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 272 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 274 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter instanceID */

/* 276 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 278 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 280 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nDCChannel */

/* 282 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 284 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 286 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pnWidth */

/* 288 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 290 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 292 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pnHeight */

/* 294 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 296 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 298 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pdFrameRate */

/* 300 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 302 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 304 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter pdPixelAspectRatio */

/* 306 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 308 */	NdrFcShort( 0x40 ),	/* X64 Stack size/offset = 64 */
/* 310 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Parameter pbInterlaced */

/* 312 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 314 */	NdrFcShort( 0x48 ),	/* X64 Stack size/offset = 72 */
/* 316 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DirectConnectionGetAudioSettings */


	/* Return value */

/* 318 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 320 */	NdrFcLong( 0x0 ),	/* 0 */
/* 324 */	NdrFcShort( 0x4 ),	/* 4 */
/* 326 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 328 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 330 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 332 */	NdrFcShort( 0x28 ),	/* 40 */
/* 334 */	NdrFcShort( 0x40 ),	/* 64 */
/* 336 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x6,		/* 6 */
/* 338 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 340 */	NdrFcShort( 0x0 ),	/* 0 */
/* 342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 344 */	NdrFcShort( 0x0 ),	/* 0 */
/* 346 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 348 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 350 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 352 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter clientID */

/* 354 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 356 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 358 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter instanceID */

/* 360 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 362 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 364 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nDCChannel */

/* 366 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 368 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 370 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pnSampleRate */

/* 372 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 374 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 376 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pnNumChannels */

/* 378 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 380 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 382 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DirectConnectionGetMetadataFrame */


	/* Return value */

/* 384 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 386 */	NdrFcLong( 0x0 ),	/* 0 */
/* 390 */	NdrFcShort( 0x5 ),	/* 5 */
/* 392 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 394 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 396 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 398 */	NdrFcShort( 0x28 ),	/* 40 */
/* 400 */	NdrFcShort( 0x24 ),	/* 36 */
/* 402 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x6,		/* 6 */
/* 404 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 406 */	NdrFcShort( 0x1 ),	/* 1 */
/* 408 */	NdrFcShort( 0x0 ),	/* 0 */
/* 410 */	NdrFcShort( 0x0 ),	/* 0 */
/* 412 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 414 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 416 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 418 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter clientID */

/* 420 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 422 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 424 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter instanceID */

/* 426 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 428 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 430 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nDCChannel */

/* 432 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 434 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 436 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plBufferSize */

/* 438 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 440 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 442 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Parameter ppOutBuffer */

/* 444 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 446 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 448 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DirectConnectionSendMetadataFrame */


	/* Return value */

/* 450 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 452 */	NdrFcLong( 0x0 ),	/* 0 */
/* 456 */	NdrFcShort( 0x6 ),	/* 6 */
/* 458 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 460 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 462 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 464 */	NdrFcShort( 0x28 ),	/* 40 */
/* 466 */	NdrFcShort( 0x8 ),	/* 8 */
/* 468 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 470 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 472 */	NdrFcShort( 0x0 ),	/* 0 */
/* 474 */	NdrFcShort( 0x0 ),	/* 0 */
/* 476 */	NdrFcShort( 0x0 ),	/* 0 */
/* 478 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 480 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 482 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 484 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter clientID */

/* 486 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 488 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 490 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter instanceID */

/* 492 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 494 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 496 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nDCChannel */

/* 498 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 500 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 502 */	NdrFcShort( 0x48 ),	/* Type Offset=72 */

	/* Parameter pszData */

/* 504 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 506 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 508 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DirectConnectionClientConnect */


	/* Return value */

/* 510 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 512 */	NdrFcLong( 0x0 ),	/* 0 */
/* 516 */	NdrFcShort( 0x7 ),	/* 7 */
/* 518 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 520 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 522 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 524 */	NdrFcShort( 0x18 ),	/* 24 */
/* 526 */	NdrFcShort( 0x0 ),	/* 0 */
/* 528 */	0x40,		/* Oi2 Flags:  has ext, */
			0x2,		/* 2 */
/* 530 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 532 */	NdrFcShort( 0x0 ),	/* 0 */
/* 534 */	NdrFcShort( 0x0 ),	/* 0 */
/* 536 */	NdrFcShort( 0x0 ),	/* 0 */
/* 538 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 540 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 542 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 544 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter clientID */

/* 546 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 548 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 550 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DirectConnectionClientDisconnect */


	/* Parameter nServerPort */

/* 552 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 554 */	NdrFcLong( 0x0 ),	/* 0 */
/* 558 */	NdrFcShort( 0x8 ),	/* 8 */
/* 560 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 562 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 564 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 566 */	NdrFcShort( 0x18 ),	/* 24 */
/* 568 */	NdrFcShort( 0x0 ),	/* 0 */
/* 570 */	0x40,		/* Oi2 Flags:  has ext, */
			0x2,		/* 2 */
/* 572 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 574 */	NdrFcShort( 0x0 ),	/* 0 */
/* 576 */	NdrFcShort( 0x0 ),	/* 0 */
/* 578 */	NdrFcShort( 0x0 ),	/* 0 */
/* 580 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 582 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 584 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 586 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter clientID */

/* 588 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 590 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 592 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DirectConnectionCreateInstance */


	/* Parameter nServerPort */

/* 594 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 596 */	NdrFcLong( 0x0 ),	/* 0 */
/* 600 */	NdrFcShort( 0x9 ),	/* 9 */
/* 602 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 604 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 606 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 608 */	NdrFcShort( 0x20 ),	/* 32 */
/* 610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 612 */	0x40,		/* Oi2 Flags:  has ext, */
			0x2,		/* 2 */
/* 614 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 616 */	NdrFcShort( 0x0 ),	/* 0 */
/* 618 */	NdrFcShort( 0x0 ),	/* 0 */
/* 620 */	NdrFcShort( 0x0 ),	/* 0 */
/* 622 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 624 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 626 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 628 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter clientID */

/* 630 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 632 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 634 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Procedure DirectConnectionDestroyInstance */


	/* Parameter instanceID */

/* 636 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 638 */	NdrFcLong( 0x0 ),	/* 0 */
/* 642 */	NdrFcShort( 0xa ),	/* 10 */
/* 644 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 646 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 648 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 650 */	NdrFcShort( 0x20 ),	/* 32 */
/* 652 */	NdrFcShort( 0x0 ),	/* 0 */
/* 654 */	0x40,		/* Oi2 Flags:  has ext, */
			0x2,		/* 2 */
/* 656 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 658 */	NdrFcShort( 0x0 ),	/* 0 */
/* 660 */	NdrFcShort( 0x0 ),	/* 0 */
/* 662 */	NdrFcShort( 0x0 ),	/* 0 */
/* 664 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 666 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 668 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 670 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter clientID */

/* 672 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 674 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 676 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Procedure DirectConnectionFreeSharedMemory */


	/* Parameter instanceID */

/* 678 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 680 */	NdrFcLong( 0x0 ),	/* 0 */
/* 684 */	NdrFcShort( 0xb ),	/* 11 */
/* 686 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 688 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 690 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 692 */	NdrFcShort( 0x26 ),	/* 38 */
/* 694 */	NdrFcShort( 0x8 ),	/* 8 */
/* 696 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 698 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 700 */	NdrFcShort( 0x0 ),	/* 0 */
/* 702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 704 */	NdrFcShort( 0x0 ),	/* 0 */
/* 706 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 708 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 710 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 712 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter clientID */

/* 714 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 716 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 718 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter instanceID */

/* 720 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 722 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 724 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter type */

/* 726 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 728 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 730 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DirectConnectionSendGlobalMetadataFrame */


	/* Return value */

/* 732 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 734 */	NdrFcLong( 0x0 ),	/* 0 */
/* 738 */	NdrFcShort( 0xc ),	/* 12 */
/* 740 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 742 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 744 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 746 */	NdrFcShort( 0x8 ),	/* 8 */
/* 748 */	NdrFcShort( 0x8 ),	/* 8 */
/* 750 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 752 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 754 */	NdrFcShort( 0x0 ),	/* 0 */
/* 756 */	NdrFcShort( 0x0 ),	/* 0 */
/* 758 */	NdrFcShort( 0x0 ),	/* 0 */
/* 760 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 762 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 764 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 766 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nDCChannel */

/* 768 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 770 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 772 */	NdrFcShort( 0x48 ),	/* Type Offset=72 */

	/* Parameter pszData */

/* 774 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 776 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 778 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const DirectConnectionTLInterface_MIDL_TYPE_FORMAT_STRING DirectConnectionTLInterface__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  4 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0x0,	/* FC_RP */
/*  8 */	NdrFcShort( 0x2 ),	/* Offset= 2 (10) */
/* 10 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 12 */	NdrFcShort( 0x20 ),	/* 32 */
/* 14 */	NdrFcShort( 0x0 ),	/* 0 */
/* 16 */	NdrFcShort( 0x0 ),	/* Offset= 0 (16) */
/* 18 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 20 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 22 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 24 */	0xd,		/* FC_ENUM16 */
			0x3,		/* FC_SMALL */
/* 26 */	0x3,		/* FC_SMALL */
			0x3e,		/* FC_STRUCTPAD2 */
/* 28 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 30 */	
			0x11, 0x0,	/* FC_RP */
/* 32 */	NdrFcShort( 0x2 ),	/* Offset= 2 (34) */
/* 34 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 36 */	NdrFcShort( 0xc ),	/* 12 */
/* 38 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 40 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 42 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 44 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 46 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 48 */	0x3,		/* FC_SMALL */
			0x5c,		/* FC_PAD */
/* 50 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 52 */	NdrFcShort( 0x2 ),	/* Offset= 2 (54) */
/* 54 */	
			0x12, 0x0,	/* FC_UP */
/* 56 */	NdrFcShort( 0x2 ),	/* Offset= 2 (58) */
/* 58 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 60 */	NdrFcShort( 0x1 ),	/* 1 */
/* 62 */	0x28,		/* Corr desc:  parameter, FC_LONG */
			0x54,		/* FC_DEREFERENCE */
/* 64 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 66 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 68 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 70 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 72 */	
			0x22,		/* FC_C_CSTRING */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const unsigned short DirectConnectionTLInterface_FormatStringOffsetTable[] =
    {
    0,
    102,
    168,
    234,
    318,
    384,
    450,
    510,
    552,
    594,
    636,
    678,
    732
    };


static const MIDL_STUB_DESC DirectConnectionTLInterface_StubDesc = 
    {
    (void *)& DirectConnectionTLInterface___RpcServerInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    0,
    0,
    0,
    0,
    0,
    DirectConnectionTLInterface__MIDL_TypeFormatString.Format,
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

static const RPC_DISPATCH_FUNCTION DirectConnectionTLInterface_table[] =
    {
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    0
    };
static const RPC_DISPATCH_TABLE DirectConnectionTLInterface_v1_0_DispatchTable = 
    {
    13,
    (RPC_DISPATCH_FUNCTION*)DirectConnectionTLInterface_table
    };

static const SERVER_ROUTINE DirectConnectionTLInterface_ServerRoutineTable[] = 
    {
    (SERVER_ROUTINE)DirectConnectionInitialize,
    (SERVER_ROUTINE)DirectConnectionGetVideoFrame,
    (SERVER_ROUTINE)DirectConnectionGetAudioFrame,
    (SERVER_ROUTINE)DirectConnectionGetVideoSettings,
    (SERVER_ROUTINE)DirectConnectionGetAudioSettings,
    (SERVER_ROUTINE)DirectConnectionGetMetadataFrame,
    (SERVER_ROUTINE)DirectConnectionSendMetadataFrame,
    (SERVER_ROUTINE)DirectConnectionClientConnect,
    (SERVER_ROUTINE)DirectConnectionClientDisconnect,
    (SERVER_ROUTINE)DirectConnectionCreateInstance,
    (SERVER_ROUTINE)DirectConnectionDestroyInstance,
    (SERVER_ROUTINE)DirectConnectionFreeSharedMemory,
    (SERVER_ROUTINE)DirectConnectionSendGlobalMetadataFrame
    };

static const MIDL_SERVER_INFO DirectConnectionTLInterface_ServerInfo = 
    {
    &DirectConnectionTLInterface_StubDesc,
    DirectConnectionTLInterface_ServerRoutineTable,
    DirectConnectionTLInterface__MIDL_ProcFormatString.Format,
    DirectConnectionTLInterface_FormatStringOffsetTable,
    0,
    0,
    0,
    0};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

