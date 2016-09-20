
#include "CallThunk.h"


namespace peace {

	namespace win {

#if defined(_M_IX86) || defined(_M_AMD64) || defined (_M_THUMB) || defined(_M_ARM64) || defined (_M_ARM64)
		PVOID __stdcall __AllocStdCallThunk(size_t sz)
		{
			return ::VirtualAlloc(NULL, sz, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		}

		VOID __stdcall __FreeStdCallThunk(PVOID thunkData,size_t sz)
		{
			::VirtualFree(thunkData, sz, MEM_RELEASE);
		}
#endif
	}

} // namespace peace  