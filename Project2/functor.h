#pragma once

namespace uuz
{
	namespace
	{
		template<typename F>
		struct getfuncname;
		template<typename R,typename ...Args>
		struct getfuncname<R(*)(Args...)>
		{
			using type = R;
		};

	}
	
	template<typename F, typename ...Args>
	class functor
	{
	public:
		explicit functor(F&& fs, Args&& ...args)
		{

		}

	private:

	};
	template<typename F,typename ...Args>
	decltype(auto) bind(F&& fs, Args&& ...args)
	{
		return functor{ forward<F>(fs),forward<Args>(...args) };
	}
}