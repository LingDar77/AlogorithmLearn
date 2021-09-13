#pragma once
#include <iostream>
namespace LD
{
	using namespace std;
	
	template<class Type>
	class __function{ };
	
	template<class RetType, class... ArgTypes>
	class __function<RetType(ArgTypes...)>
	{
	private:
		struct CallableBase
		{
			virtual ~CallableBase(){ }
			virtual RetType operator()(ArgTypes...) = 0;
			virtual CallableBase* Clone() const = 0;
		};

		template<class AnyType>
		struct Callable : CallableBase
		{
			AnyType functor;
			Callable(AnyType func)
				:functor(func)
			{

			}

			virtual RetType operator()(ArgTypes...) override
			{
				return functor(ArgTypes...);
			}

			virtual CallableBase* Clone() const override
			{
				return new Callable<AnyType>(functor);
			}

		};
		CallableBase* pCallable;
	public:

		template<class AnyType>
		__function(AnyType func)
			:pCallable(new Callable<AnyType>(func))
		{

		}

		__function(const __function& rhs)
			:pCallable(rhs.pCallable->Clone())
		{

		}

		~__function()
		{
			delete pCallable;
		}

		__function& operator=(const __function & rhs)
		{
			delete pCallable;
			pCallable = rhs.pCallable->Clone();
			return *this;
		}

// 		bool operator==(const function& rhs)
// 		{
// 			auto p1 = dynamic_cast<Callable<RetType(ArgTypes...)>*>(pCallable);
// 			auto p2 = dynamic_cast<Callable<RetType(ArgTypes...)>*>(rhs.pCallable);
// 			return p1->functor == p2->functor;
// 		}//Hard to implement

		RetType operator()(ArgTypes...)
		{
			return (*pCallable)(ArgTypes...);
		}
	};


	template<class T>
	class _function { };

	template<class RetType, class... ArgTypes>
	class _function<RetType(ArgTypes...)>
	{
	private:
		RetType(*Operator_)(_function*, ArgTypes...);
		void(*Destructor_)(_function*);
		void*(*Clone_)(const _function*);
		void* ptr;

		template<class functor>
		static void* Clone(const _function* rhs)
		{
			return new functor(*static_cast<const functor*>(rhs->ptr));
		}

		template<class functor>
		static RetType Operator(_function* self, ArgTypes...)
		{
			return (*static_cast<functor*>(self->ptr))(ArgTypes...);
		}

		template<class functor>
		static void Destructor(_function* self)
		{
			delete static_cast<functor*>(self->ptr);
		}

	public:
		template<class T>
		_function(T functor)
			:Operator_(Operator<T>),
			 Destructor_(Destructor<T>),
			 Clone_(Clone<T>),
			 ptr(new T(functor))
		{

		}

		_function(const _function& rhs)
			:Operator_(rhs.Operator_),
			 Destructor_(rhs.Destructor_),
			 Clone_(rhs.Clone_),
			 ptr(Clone_(&rhs))
		{

		}

		~_function()
		{
			Destructor_(this);
		}

		_function& operator=(const _function& rhs)
		{
			Destructor_(this);
			ptr = rhs.Clone_(&rhs);
			Destructor_ = rhs.Destructor_;
			Clone_ = rhs.Clone_;
			Operator_ = rhs.Operator_;
			return *this;
		}

		RetType operator()(ArgTypes...)
		{
			return Operator_(this, ArgTypes...);
		}

	};


	/*
	 * a sloppy imitation of std::function,
	 * which seems not to support reference type
	 */
	template<class Type>
	class Function{ };

	template<class RetType, class... Args>
	class Function<RetType(Args...)> 
	{
	private:
		void*	pCallable;
		void	(*fDestructor)(Function* self);
		RetType (*fCall)(Function* self, Args...);
		void	(*fClone)(Function* dest, const Function* self);

		template<class FuncType>
		static void Destructor(Function* self)
		{
			delete static_cast<FuncType*>(self->pCallable);
		}

		template<class FuncType>
		static void DestructorPlacement(Function* self)
		{
			static_cast<FuncType*>(self->pCallable)->~FuncType();
		}

		template<class FuncType>
		static RetType Call(Function* self, Args...)
		{
			return (*static_cast<FuncType*>(self->pCallable))(Args...);
		}

		template<class FuncType>
		static RetType CallPlacement(Function* self, Args...)
		{
			return (*static_cast<FuncType*>(static_cast<void*>(&(self->pCallable))))(Args...);
		}

		template<class FuncType>
		static void Clone(Function* dest, const Function* self)
		{
			dest->pCallable = new FuncType(*static_cast<const FuncType*>(self->pCallable));
		}

		template<class FuncType>
		static void ClonePlacement(Function* dest, const Function* self)
		{
			new(&dest->pCallable) FuncType(*static_cast<const FuncType*>(static_cast<const void *>(&self->pCallable)));
		}

		template<bool placement>
		struct Placement { };

		template<class FuncType>
		void Init(FuncType func, Placement<false>)
		{
			fCall = Call<FuncType>;
			fClone = Clone<FuncType>;
			fDestructor = Destructor<FuncType>;
			pCallable = new FuncType(func);
		}

		template<class FuncType>
		void Init(FuncType func, Placement<true>)
		{
			fCall = CallPlacement<FuncType>;
			fClone = ClonePlacement<FuncType>;
			fDestructor = DestructorPlacement<FuncType>;
			new(&pCallable) FuncType(func);
		}

	public:
		
		template<class FuncType>
		Function(FuncType func)
		{
			Init<FuncType>(func, Placement<(sizeof(pCallable) >= sizeof(FuncType))>());
		}

		Function(const Function& rhs)
			:fDestructor(rhs.fDestructor),
			 fCall(rhs.fCall),
			 fClone(rhs.fClone),
			 fClone(this, &rhs)
		{

		}

		~Function()
		{
			fDestructor(this);
		}

		Function& operator=(const Function& rhs)
		{
			fDestructor(this);
			fDestructor = rhs.fDestructor;
			fCall = rhs.fCall;
			fClone = rhs.fClone;
			fClone(this, &rhs);
			return *this;
		}

		RetType operator()(Args...)
		{
			return fCall(this, Args...);
		}

	};

	
}