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
			virtual ~store_delete_base() = 0;
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
			store_nodelete(T* p, const D& d) :core{ p }, dele{ d } {}
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
			virtual ~store_delete()override
			{
				deletor();
			}
			T* core;
		};

		template<typename T>
		struct  other_store
		{
			other_store(store_delete_base* o,T* d):p{d},old{o}
			{
				old->addcan();
			}
			virtual void deletor()noexcept override
			{
				return;
			}
			virtual void* get()noexcept override
			{
				return p;
			}
			virtual void addcan()const noexcept override
			{
				return	old->addcan();
			}
			virtual void subcan()override
			{
				return old->subcan();
			}
			virtual ~store_delete()override
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
	public:
		constexpr shared_ptr() :that{ new store_nodelete<T>(new T()) } {};
		constexpr shared_ptr(std::nullptr_t) :that{ new store_nodelete<T>(nullptr) } {};
		template<typename Y,
				typename = enable_if_t<std::is_convertible<Y*, T*>::value>>
		explicit shared_ptr(Y* ptr):that{ new store_nodelete<T>(static_cast<T*>(ptr)) }{}
		template<typename Y, typename Deleter,
				typename= enable_if_t<std::is_convertible<Y*,T*>::value>>
		shared_ptr(Y* ptr, const Deleter& d) : that{ new store_delete<T,Deleter>(static_cast<T*>(ptr),d) } {}
		template<typename Y>
		shared_ptr(const shared_ptr<Y>& ptr,T* p):that{new other_store<T>(ptr->that,p)}{}
		template<typename Deleter>
		shared_ptr(std::nullptr_t n,const Deleter& d):that{ new store_delete<T,Deleter>(nullptr,d) }{}
		shared_ptr(const shared_ptr& t):that{t.that}
		{
			that->addcan();
		}
		shared_ptr(shared_ptr&& t) :that{ t.that }
		{
			t.that = nullptr;
		}
		template<typename Y,
				typename = enable_if_t<std::is_convertible<Y*, T*>::value>>
		shared_ptr(const shared_ptr<Y>& t):that{t.that}
		{
			that->addcan();
		}
		template<typename Y,
			typename = enable_if_t<std::is_convertible<Y*, T*>::value>>
			shared_ptr(shared_ptr<Y>&& t):that{ t.that }
		{
			t.that = nullptr;
		}


		shared_ptr& operator=(const shared_ptr& t)
		{
			that = t.that;
			that->addcan();
		}
		shared_ptr& operator=(shared_ptr&& t)
		{
			that = t.that;
			t.that=nullptr;
		}
		template<typename Y,
			typename = enable_if_t<std::is_convertible<Y*, T*>::value>>
		shared_ptr& operator=(const shared_ptr<Y>& t)
		{
			that = t.that;
			that->addcan();
		}
		template<typename Y,
			typename = enable_if_t<std::is_convertible<Y*, T*>::value>>
		shared_ptr& operator=(shared_ptr<Y>&& t)
		{
			that = t.that;
			t.that = nullptr;
		}

		void reset()noexcept
		{
			that->subcan();
			that = new store_nodelete<T>(nullptr);
		}
		template<typename Y,
				typename = enable_if_t<std::is_convertible<Y*, T*>::value>>
		void reset(Y* ptr)
		{
			that->subcan();
			that = new store_nodelete<T>(static_cast<T*>(ptr));
		}
		template< class Y, class Deleter,
			typename = enable_if_t<std::is_convertible<Y*, T*>::value>>
		void reset(Y* ptr, Deleter d)
		{
			that->subcan();
			that = new store_delete<T, Deleter>(static_cast<T*>(ptr), d);
		}
	
		void swap(shared_ptr& t)noexcept
		{
			swap(that, t.that);
		}

		T* get()noexcept
		{
			return (T*)that->get();
		}

		T& operator*()noexcept
		{
			return *get();
		}
		T* operator->()noexcept
		{
			return get();
		}

		long long use_count()const noexcept
		{
			return that->can;
		}

		explicit operator bool()const noexcept
		{
			reutrn get() != nullptr;
		}

		template< typename Y >
		bool owner_before(const shared_ptr<Y>& other) const noexcept
		{
			if (other.get() == get())
				return false;
			return get() < other.get();
		}


		template< typename T, typename... Args >
		friend shared_ptr<T> make_shared(Args&&... args)
		{
			return shared_ptr<T>(new T(std::forward<Args>(args...)));
		}

		template<typename T,typename U>
		friend shared_ptr<T> static_pointer_cast(const std::shared_ptr<U>& r)
		{

		}


		~shared_ptr()
		{
			--that->can;
			that->away();
		}
	private:
		store_delete_base* that = nullptr;
	};
}