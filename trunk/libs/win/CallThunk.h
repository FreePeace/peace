//使用场所
//1，在调用操作系统的api函数或C函数时，参数为回调函数。这个回调函数要么是全局要么是静态，这时把函数与this邦到一起一定会解决很对对象的问题
//2，想把成员函数当作全局或静态成员使用时（与第一条很像）
//缺点
//32位64位要不同处理，cpu指令不同也会有变化
//
//注：c#中有delegate非常好的解决了这些问题， 但不知道它是通过什么方式来实现的，它的实现有可能是通过类似于c++的函数对象方式实现，但这样做的性能就有一定的差异了，也可能是像atl那样的处理，不同cpu作不同处理
//
//网上的参考实现
//1， http ://www.cppblog.com/proguru/archive/2008/08/24/59831.html
//2， http ://www.codeproject.com/Articles/16785/Thunking-in-Win-Simplifying-Callbacks-to-Non-sta
//3，http ://www.cppblog.com/Streamlet/archive/2010/10/24/131064.html 
//	4， http ://www.cnblogs.com/tekkaman/archive/2011/05/17/2049170.html
//5，http ://www.codeproject.com/Articles/5717/Better-way-to-use-member-function-for-C-style-call
//
//	atlstdthunk这个的实现比较全的，建议详细研究，不过wtl的也应该比较全，只是没有看过，就不建议了。

// This is a part of the Active Template Library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.

#pragma once
#include <Windows.h>
#include <atlstdthunk.h>


#if !defined(USE_ATL_THUNK2) && (defined(_M_IX86) || defined(_M_AMD64) || defined(_M_ARM) || defined(_M_ARM64))
#define USE_ATL_THUNK2
#endif


namespace peace
{
	namespace win
	{
		void UninitThunk();
		void InitThunk();
		
		/////////////////////////////////////////////////////////////////////////////
		// Thunks for __stdcall member functions

#if defined(_M_IX86)
		PVOID __stdcall __AllocStdCallThunk(size_t sz);
		VOID  __stdcall __FreeStdCallThunk(PVOID thunkData, size_t sz);
#pragma pack(push,1)
		struct CallThunk
		{
			DWORD   m_mov;          // mov dword ptr [esp+0x4], pThis (esp+0x4 is hWnd)
			DWORD   m_this;         //
			BYTE    m_jmp;          // jmp WndProc
			DWORD   m_relproc;      // relative jmp
			BOOL Init(
				_In_ DWORD_PTR proc,
				_In_opt_ void* pThis)
			{
				m_mov = 0x042444C7;  //C7 44 24 0C
				m_this = PtrToUlong(pThis);
				m_jmp = 0xe9;
				m_relproc = DWORD((INT_PTR)proc - ((INT_PTR)this + sizeof(CallThunk)));
				// write block from data cache and
				//  flush from instruction cache
				FlushInstructionCache(GetCurrentProcess(), this, sizeof(CallThunk));
				return TRUE;
			}
			//some thunks will dynamically allocate the memory for the code
			void* GetCodeAddress()
			{
				return this;
			}
			_Ret_maybenull_ _Post_writable_byte_size_(sizeof(CallThunk)) void* operator new(size_t sz)
			{
				return __AllocStdCallThunk(sz);
			}
			void operator delete(_In_opt_ void* pThunk)
			{
				__FreeStdCallThunk(pThunk, sizeof(CallThunk));
			}
		};
#pragma pack(pop)

#elif defined(_M_AMD64)
		PVOID __stdcall __AllocStdCallThunk(size_t);
		VOID  __stdcall __FreeStdCallThunk(PVOID thunkData, size_t sz);
#pragma pack(push,2)
		struct CallThunk
		{
			USHORT  RcxMov;         // mov rcx, pThis
			ULONG64 RcxImm;         //
			USHORT  RaxMov;         // mov rax, target
			ULONG64 RaxImm;         //
			USHORT  RaxJmp;         // jmp target
			BOOL Init(
				_In_ DWORD_PTR proc,
				_In_opt_ void *pThis)
			{
				RcxMov = 0xb948;          // mov rcx, pThis
				RcxImm = (ULONG64)pThis;  //
				RaxMov = 0xb848;          // mov rax, target
				RaxImm = (ULONG64)proc;   //
				RaxJmp = 0xe0ff;          // jmp rax
				FlushInstructionCache(GetCurrentProcess(), this, sizeof(CallThunk));
				return TRUE;
			}
			//some thunks will dynamically allocate the memory for the code
			void* GetCodeAddress()
			{
				return this;
			}
			_Ret_maybenull_ _Post_writable_byte_size_(sizeof(CallThunk)) void* operator new(size_t sz)
			{
				return __AllocStdCallThunk(sz);
			}
			void operator delete(_In_opt_ void* pThunk)
			{
				__FreeStdCallThunk(pThunk,sizeof(CallThunk));
			}
		};
#pragma pack(pop)
#elif defined(_SH3_)
#pragma pack(push,4)
		struct CallThunk // this should come out to 16 bytes
		{
			WORD	m_mov_r0;		// mov.l	pFunc,r0
			WORD	m_mov_r1;		// mov.l	pThis,r1
			WORD	m_jmp;			// jmp		@r0
			WORD	m_nop;			// nop
			DWORD	m_pFunc;
			DWORD	m_pThis;
			BOOL Init(
				_In_ DWORD_PTR proc,
				_In_opt_ void* pThis)
			{
				m_mov_r0 = 0xd001;
				m_mov_r1 = 0xd402;
				m_jmp = 0x402b;
				m_nop = 0x0009;
				m_pFunc = (DWORD)proc;
				m_pThis = (DWORD)pThis;
				// write block from data cache and
				//  flush from instruction cache
				FlushInstructionCache(GetCurrentProcess(), this, sizeof(CallThunk));
				return TRUE;
			}
			void* GetCodeAddress()
			{
				return this;
			}
		};
#pragma pack(pop)
#elif defined(_MIPS_)
#pragma pack(push,4)
		struct CallThunk
		{
			WORD	m_pFuncHi;
			WORD	m_lui_t0;		// lui		t0,PFUNC_HIGH
			WORD	m_pFuncLo;
			WORD	m_ori_t0;		// ori		t0,t0,PFUNC_LOW
			WORD	m_pThisHi;
			WORD	m_lui_a0;		// lui		a0,PTHIS_HIGH
			DWORD	m_jr_t0;		// jr		t0
			WORD	m_pThisLo;
			WORD	m_ori_a0;		// ori		a0,PTHIS_LOW
			BOOL Init(
				_In_ DWORD_PTR proc,
				_In_opt_ void* pThis)
			{
				m_pFuncHi = HIWORD(proc);
				m_lui_t0 = 0x3c08;
				m_pFuncLo = LOWORD(proc);
				m_ori_t0 = 0x3508;
				m_pThisHi = HIWORD(pThis);
				m_lui_a0 = 0x3c04;
				m_jr_t0 = 0x01000008;
				m_pThisLo = LOWORD(pThis);
				m_ori_a0 = 0x3484;
				// write block from data cache and
				//  flush from instruction cache
				FlushInstructionCache(GetCurrentProcess(), this, sizeof(CallThunk));
				return TRUE;
			}
			void* GetCodeAddress()
			{
				return this;
			}
		};
#pragma pack(pop)
#elif defined (_M_THUMB)
	// note this case must be before _M_ARM because _M_ARM is also defined
		PVOID __stdcall __AllocStdCallThunk(size_t);
		VOID  __stdcall __FreeStdCallThunk(PVOID thunkData, size_t sz);
#pragma pack(push,2)
		struct CallThunk
		{
			USHORT	m_mov_r0[2];	// mov	r0, pThis
			USHORT	m_mov_pc[2];	// mov	pc, pFunc
			DWORD	m_pThis;
			DWORD	m_pFunc;
			BOOL Init(DWORD_PTR proc, void* pThis)
			{
				m_mov_r0[0] = 0xF8DF;
				m_mov_r0[1] = 0x0004;
				m_mov_pc[0] = 0xF8DF;
				m_mov_pc[1] = 0xF004;
				m_pThis = (DWORD)pThis;
				m_pFunc = (DWORD)proc;
				// write block from data cache and
				//  flush from instruction cache
				FlushInstructionCache(GetCurrentProcess(), this, sizeof(CallThunk));
				return TRUE;
			}
			void* GetCodeAddress()
			{
				return (void *)((ULONG_PTR)this | 1);
			}
			void* operator new(size_t sz)
			{
				return __AllocStdCallThunk(sz);
			}
			void operator delete(void* pThunk)
			{
				__FreeStdCallThunk(pThunk,sizeof(CallThunk));
			}
		};
#pragma pack(pop)
#elif defined(_M_ARM64)
		PVOID __stdcall __AllocStdCallThunk(size_t);
		VOID  __stdcall __FreeStdCallThunk(PVOID thunkData, size_t sz);
#pragma pack(push,4)  
		struct CallThunk {
			ULONG   m_ldr_r16;      // ldr  x16, [pc, #24]
			ULONG   m_ldr_r0;       // ldr  x0, [pc, #12]
			ULONG   m_br;           // br   x16      
			ULONG   m_pad;
			ULONG64	m_pThis;
			ULONG64	m_pFunc;
			BOOL Init(DWORD_PTR proc, void* pThis) {
				m_ldr_r16 = 0x580000D0;
				m_ldr_r0 = 0x58000060;
				m_br = 0xd61f0200;
				m_pThis = (ULONG64)pThis;
				m_pFunc = (ULONG64)proc;
				// write block from data cache and  		
				//  flush from instruction cache  		
				FlushInstructionCache(GetCurrentProcess(), this, sizeof(CallThunk));
				return TRUE;
			}
			void* GetCodeAddress() {
				return (void *)((ULONG_PTR)this | 1);
			}
			void* operator new(size_t sz)
			{
				return __AllocStdCallThunk(sz);
			}
			void operator delete(void* pThunk) {
				__FreeStdCallThunk(pThunk,sizeof(CallThunk));
			}
		};
#pragma pack(pop)
#elif defined(_ARM_)
#pragma pack(push,4)
		struct CallThunk // this should come out to 16 bytes
		{
			DWORD	m_mov_r0;		// mov	r0, pThis
			DWORD	m_mov_pc;		// mov	pc, pFunc
			DWORD	m_pThis;
			DWORD	m_pFunc;
			BOOL Init(
				_In_ DWORD_PTR proc,
				_In_opt_ void* pThis)
			{
				m_mov_r0 = 0xE59F0000;
				m_mov_pc = 0xE59FF000;
				m_pThis = (DWORD)pThis;
				m_pFunc = (DWORD)proc;
				// write block from data cache and
				//  flush from instruction cache
				FlushInstructionCache(GetCurrentProcess(), this, sizeof(CallThunk));
				return TRUE;
			}
			void* GetCodeAddress()
			{
				return this;
			}
		};
#pragma pack(pop)
#elif defined(_M_IA64)
#pragma pack(push,8)
		extern "C" void _StdCallThunkProcProc(void);
		struct _FuncDesc
		{
			void* pfn;
			void* gp;
		};
		struct CallThunk
		{
			_FuncDesc m_funcdesc;
			void* m_pFunc;
			void* m_pThis;

			BOOL Init(
				_In_ DWORD_PTR proc,
				_In_opt_ void* pThis)
			{
				m_funcdesc.pfn = ((_FuncDesc*)(&_StdCallThunkProcProc))->pfn;  // Pointer to actual beginning of StdCallThunkProc
				m_funcdesc.gp = &m_pFunc;
				m_pFunc = reinterpret_cast<void*>(proc);
				m_pThis = pThis;
				::FlushInstructionCache(GetCurrentProcess(), this, sizeof(CallThunk));
				return TRUE;
			}
			void* GetCodeAddress()
			{
				return(&m_funcdesc);
			}
		};
#pragma pack(pop)
		//IA64 thunks do not currently use the atlhunk.cpp allocator.
#elif defined (_M_ARM64)  
		PVOID __stdcall __AllocStdCallThunk(size_t);
		VOID  __stdcall __FreeStdCallThunk(PVOID thunkData, size_t sz);
#pragma pack(push,4)  
		struct CallThunk {
			ULONG   m_ldr_r16;      // ldr  x16, [pc, #24]
			ULONG   m_ldr_r0;       // ldr  x0, [pc, #12]
			ULONG   m_br;           // br   x16      
			ULONG   m_pad;
			ULONG64	m_pThis;
			ULONG64	m_pFunc;
			BOOL Init(DWORD_PTR proc, void* pThis) {
				m_ldr_r16 = 0x580000D0;
				m_ldr_r0 = 0x58000060;
				m_br = 0xd61f0200;
				m_pThis = (ULONG64)pThis;
				m_pFunc = (ULONG64)proc;
				// write block from data cache and  		
				//  flush from instruction cache  		
				FlushInstructionCache(GetCurrentProcess(), this, sizeof(CallThunk));
				return TRUE;
			}
			void* GetCodeAddress() {
				return (void *)((ULONG_PTR)this | 1);
			}
			void* operator new(size_t sz)
			{
				return __AllocStdCallThunk(sz);
			}
			void operator delete(void* pThunk) {
				__FreeStdCallThunk(pThunk,sizeof(CallThunk));
			}
		};
#pragma pack(pop)  
#else
#error Only ARM, ARM64, ALPHA, SH3, MIPS, IA64, AMD64 and X86 supported
#endif
	}//namespace win
}   // namespace peace



