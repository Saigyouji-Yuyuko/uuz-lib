#pragma once
#include<stdexcept>
#include<exception>
#include<atomic>
namespace uuz
{
	namespace
	{
		template<typename T>
		class shared_ptr_impl
		{
			using destory = void(*)(T*);
		public:
		private:
			std::atomic<size_t> can;
			T* that;
			destory p = nullptr;
		};
	}
	template<typename T>
	class shared_ptr
	{
	public:
		~shared_ptr()
		{
			if (that)
				delete that;
		}
	private:
		shared_ptr_impl<T>* that = nullptr;
	};
}