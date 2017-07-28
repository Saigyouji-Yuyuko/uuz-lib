#pragma once
#include"prepare.h"
#include<stdexcept>
#include<exception>
#include<atomic>
namespace uuz
{
	namespace
	{
		struct store_delete_base
		{
			store_delete_base():can{1}{}
			virtual void addcan()const noexcept { ++can; }
			virtual void subcan() { --can; if (can == 0)delete this; }
			virtual void deletor() noexcept = 0;
			virtual void* get()noexcept = 0;
			virtual ~store_delete_base() {};
			void away()
			{
				if(can == 0)
					delete this;
			}
			mutable std::atomic<size_t> can;
		};

		template<typename T,typename D = nil>
		struct store_delete:public store_delete_base
		{
			store_delete(T* p, const D& d) :store_delete_base(),core{ p }, dele{ d } {}
			virtual void deletor()noexcept override
			{
				dele(core);
			}
			virtual void* get()noexcept override
			{
				return core;
			}
			virtual ~store_delete()override
			{
				deletor();
			}
			T* core;
			D dele;
		};

		template<typename T>
		struct store_nodelete :public store_delete_base
		{
			store_nodelete(T* p):core{p}{}
			virtual void deletor()noexcept override
			{
				delete core;
			}
			virtual void* get()noexcept override
			{
				return core;
			}
			virtual ~store_nodelete()override
			{
				deletor();
			}
			T* core;
		};

		template<typename T>
		struct  other_store:store_delete_base
		{
			other_store(store_delete_base* o,T* d):p{d},old{o}
			{
				old->addcan();
			}
			virtual void deletor()noexcept 
			{
				return;
			}
			virtual void* get()noexcept
			{
				return p;
			}
			virtual void addcan()const noexcept 
			{
				return	old->addcan();
			}
			virtual void subcan()
			{
				return old->subcan();
			}
			virtual ~other_store()
			{
				old->subcan();
				deletor();
			}
			T* p;
			store_delete_base* old;
		};
	}
	template<typename T>
	class shared_ptr
	{
		using self = shared_ptr;
		using value_type = T;
		template<typename U>
		friend class shared_ptr;
	public:
		constexpr shared_ptr() :that{nullptr } {};
		constexpr shared_ptr(std::nullptr_t) : that{ new store_nodelete<T>(nullptr) } {};
		template<typename Y,
				typename = std::enable_if_t<std::is_convertible<Y*, T*>::value>>
		explicit shared_ptr(Y* ptr):that{ new store_nodelete<T>(static_cast<T*>(ptr)) }{}
		template<typename Y, typename Deleter,
				typename= std::enable_if_t<std::is_convertible<Y*,T*>::value>>
		shared_ptr(Y* ptr, const Deleter& d) : that{ new store_delete<T,Deleter>(static_cast<T*>(ptr),d) } {}
		template<typename Y>
		shared_ptr(const shared_ptr<Y>& ptr,T* p):that{new other_store<T>(ptr.that,p)}{}
		template<typename Deleter>
		shared_ptr(std::nullptr_t n,const Deleter& d):that{ new store_delete<T,Deleter>(nullptr,d) }{}
		shared_ptr(const shared_ptr& t):that{t.that}
		{
			if(that)
				that->addcan();
		}
		shared_ptr(shared_ptr&& t) :that{ t.that }
		{
			t.that = nullptr;
		}
		template<typename Y,
				typename = std::enable_if_t<std::is_convertible<Y*, T*>::value>>
		shared_ptr(const shared_ptr<Y>& t):that{t.that}
		{
			if(that)
				that->addcan();
		}
		template<typename Y,
			typename = std::enable_if_t<std::is_convertible<Y*, T*>::value>>
			shared_ptr(shared_ptr<Y>&& t):that{ t.that }
		{
			t.that = nullptr;
		}


		shared_ptr& operator=(const shared_ptr& t)
		{
			that = t.that;
			that->addcan();
			return *this;
		}
		shared_ptr& operator=(shared_ptr&& t)
		{
			that = t.that;
			t.that=nullptr;
			return *this;
		}
		template<typename Y,
			typename = std::enable_if_t<std::is_convertible<Y*, T*>::value>>
		shared_ptr& operator=(const shared_ptr<Y>& t)
		{
			that = t.that;
			that->addcan();
			return *this;
		}
		template<typename Y,
			typename = std::enable_if_t<std::is_convertible<Y*, T*>::value>>
		shared_ptr& operator=(shared_ptr<Y>&& t)
		{
			that = t.that;
			t.that = nullptr;
			return *this;
		}

		void reset()noexcept
		{
			if(that)
				that->subcan();
			that = nullptr;
		}
		template<typename Y,
				typename = std::enable_if_t<std::is_convertible<Y*, T*>::value>>
		void reset(Y* ptr)
		{
			if(that)
				that->subcan();
			that = new store_nodelete<T>(static_cast<T*>(ptr));
		}
		template< class Y, class Deleter,
			typename = std::enable_if_t<std::is_convertible<Y*, T*>::value>>
		void reset(Y* ptr, Deleter d)
		{
			if(that)
				that->subcan();
			that = new store_delete<T, Deleter>(static_cast<T*>(ptr), d);
		}
	
		void swap(shared_ptr& t)noexcept
		{
			swap(that, t.that);
		}

		T* get()noexcept
		{
			if (!that)
				return nullptr;
			return (T*)that->get();
		}
		T* get()const noexcept
		{
			if (!that)
				return nullptr;
			return (T*)that->get();
		}
		T& operator*()const noexcept
		{
			return *get();
		}
		
		T* operator->()const noexcept
		{
			return get();
		}
		T* operator->()noexcept
		{
			return get();
		}

		long long use_count()const noexcept
		{
			if (!that)
				return 0;
			return that->can;
		}

		explicit operator bool()const noexcept
		{
			return get() != nullptr;
		}

		template< typename Y >
		bool owner_before(const shared_ptr<Y>& other) const noexcept
		{
			if (other.get() == get())
				return false;
			return get() < other.get();
		}



		~shared_ptr()
		{
			if (that)
			{
				--that->can;
				that->away();
			}
			
		}
	private:
		 store_delete_base* that = nullptr;
	};
	template< typename T, typename... Args >
	shared_ptr<T> make_shared(Args&&... args)
	{
		return shared_ptr<T>(new T(uuz::forward<Args>(args)...));
	}

	template<typename T, typename U>
	shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r)noexcept
	{
		return shared_ptr<T>(r, static_cast<T*>(r.get()));
	}

	template<typename T, typename U>
	shared_ptr<T> const_pointer_cast(const shared_ptr<U>& r)noexcept
	{
		return shared_ptr<T>(r, const_cast<T*>(r.get()));
	}

	template< typename T, typename U >
	shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& r) noexcept
	{
		if (auto p = dynamic_cast<T*>(r.get())) {
			return shared_ptr<T>(r, p);
		}
		else {
			return shared_ptr<T>();
		}
	}
	template <typename T, typename U, typename V>
	std::basic_ostream<U, V>& operator<<(std::basic_ostream<U, V>& os, const shared_ptr<T>& ptr)
	{
		os << ptr.get();
		return os;
	}
	template<typename _Ty1, typename _Ty2>
		bool operator==(const shared_ptr<_Ty1>& _Left, const shared_ptr<_Ty2>& _Right) noexcept
	{	// test if shared_ptr == shared_ptr
		return (_Left.get() == _Right.get());
	}

	template<typename _Ty1, typename _Ty2>
		bool operator!=(const shared_ptr<_Ty1>& _Left, const shared_ptr<_Ty2>& _Right) noexcept
	{	// test if shared_ptr != shared_ptr
		return (!(_Left == _Right));
	}

	template<class _Ty1,
		class _Ty2>
		bool operator<(const shared_ptr<_Ty1>& _Left, const shared_ptr<_Ty2>& _Right) noexcept
	{	// test if shared_ptr < shared_ptr
		return (less<decltype(_Always_false<_Ty1>::value
			? _Left.get() : _Right.get())>()(
				_Left.get(), _Right.get()));
	}

	template<class _Ty1,
		class _Ty2>
		bool operator>=(const shared_ptr<_Ty1>& _Left, const shared_ptr<_Ty2>& _Right) noexcept
	{	// shared_ptr >= shared_ptr
		return (!(_Left < _Right));
	}

	template<class _Ty1,
		class _Ty2>
		bool operator>(const shared_ptr<_Ty1>& _Left, const shared_ptr<_Ty2>& _Right) noexcept
	{	// test if shared_ptr > shared_ptr
		return (_Right < _Left);
	}

	template<class _Ty1,
		class _Ty2>
		bool operator<=(const shared_ptr<_Ty1>& _Left, const shared_ptr<_Ty2>& _Right) noexcept
	{	// test if shared_ptr <= shared_ptr
		return (!(_Right < _Left));
	}

	template<class _Ty>
	bool operator==(const shared_ptr<_Ty>& _Left, nullptr_t) noexcept
	{	// test if shared_ptr == nullptr
		return (_Left.get() == nullptr_t{});
	}

	template<class _Ty>
	bool operator==(nullptr_t, const shared_ptr<_Ty>& _Right) noexcept
	{	// test if nullptr == shared_ptr
		return (nullptr_t{} == _Right.get());
	}

	template<class _Ty>
	bool operator!=(const shared_ptr<_Ty>& _Left, nullptr_t) noexcept
	{	// test if shared_ptr != nullptr
		return (!(_Left == nullptr_t{}));
	}

	template<class _Ty>
	bool operator!=(nullptr_t, const shared_ptr<_Ty>& _Right) noexcept
	{	// test if nullptr != shared_ptr
		return (!(nullptr_t{} == _Right));
	}

	template<class _Ty>
	bool operator<(const shared_ptr<_Ty>& _Left, nullptr_t) noexcept
	{	// test if shared_ptr < nullptr
		return (less<_Ty *>()(_Left.get(), nullptr_t{}));
	}

	template<class _Ty>
	bool operator<(nullptr_t, const shared_ptr<_Ty>& _Right) noexcept
	{	// test if nullptr < shared_ptr
		return (less<_Ty *>()(nullptr_t{}, _Right.get()));
	}

	template<class _Ty>
	bool operator>=(const shared_ptr<_Ty>& _Left, nullptr_t) noexcept
	{	// test if shared_ptr >= nullptr
		return (!(_Left < nullptr_t{}));
	}

	template<class _Ty>
	bool operator>=(nullptr_t, const shared_ptr<_Ty>& _Right) noexcept
	{	// test if nullptr >= shared_ptr
		return (!(nullptr_t{} < _Right));
	}

	template<class _Ty>
	bool operator>(const shared_ptr<_Ty>& _Left, nullptr_t) noexcept
	{	// test if shared_ptr > nullptr
		return (nullptr_t{} < _Left);
	}

	template<class _Ty>
	bool operator>(nullptr_t, const shared_ptr<_Ty>& _Right) noexcept
	{	// test if nullptr > shared_ptr
		return (_Right < nullptr_t{});
	}

	template<class _Ty>
	bool operator<=(const shared_ptr<_Ty>& _Left, nullptr_t) noexcept
	{	// test if shared_ptr <= nullptr
		return (!(nullptr_t{} < _Left));
	}

	template<class _Ty>
	bool operator<=(nullptr_t, const shared_ptr<_Ty>& _Right) noexcept
	{	// test if nullptr <= shared_ptr
		return (!(_Right < nullptr_t{}));
	}

	template<typename T>
	class weak_ptr
	{
	public:

	private:

	};
}