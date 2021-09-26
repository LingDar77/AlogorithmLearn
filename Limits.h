#pragma once
#ifndef Limits_h__
#define Limits_h__




template<bool Expression, class RetType = void>
struct EnableTemplate
{
};

template<class RetType>
struct EnableTemplate<true, RetType>
{
	using Result = typename RetType;
};

#define EnableTemplateWhen(BoolExpression) class = typename EnableTemplate<(BoolExpression)>::Result

struct TrueType
{
	const static bool Value = true ;
};

struct FalseType
{
	const static bool Value = false;
};





#endif // Limits_h__