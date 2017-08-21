#pragma once
namespace uuz
{
	template<typename T>
	struct iterator_traits;

	template<typename T>
	class reverse_iterator
	{
		using value_type = typename T::value_type;
		using iterator_type = typename T::iterator_type;
	};


}