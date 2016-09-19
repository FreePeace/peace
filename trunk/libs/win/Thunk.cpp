
#include "Thunk.h"


namespace peace {

	namespace win {

		HANDLE __thunkHeap = nullptr;

		void UninitThunk()
		{
			if (nullptr != __thunkHeap)
			{
				::HeapDestroy(__thunkHeap);
				__thunkHeap = nullptr;
			}
		}
		void InitThunk()
		{
			UninitThunk();
			__thunkHeap = ::HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
		}
#if defined(_M_IX86) || defined(_M_AMD64) || defined (_M_THUMB) || defined(_M_ARM64) || defined (_M_ARM64)
		PVOID __stdcall __AllocStdCallThunk(size_t sz)
		{
			return ::HeapAlloc(__thunkHeap, HEAP_ZERO_MEMORY,sz);
		}

		VOID __stdcall __FreeStdCallThunk(PVOID Thunk)
		{
			::HeapFree(__thunkHeap,0, Thunk);
		}
#endif
	}

} // namespace peace  