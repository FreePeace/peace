#pragma once

namespace peace
{
	namespace utils
	{
		//get it form net 
		template<typename Target, typename Source>
		inline Target brute_cast(const Source s)
		{
			//(sizeof(Target) == sizeof(Source));
			union { Target t; Source s; } u;
			u.s = s;
			return u.t;
		}
	}
}