/*
	Copyright (c) 2020 Kim,Bummoo
	Read KAI.h
*/
#pragma once
#include <amp.h>
#include <vector>
#include <map>

namespace KAI
{
	template <typename Type>
	class Controller;

	namespace VAL {
		template <typename Type>
		class Val;
	}
	namespace FLOW {
		namespace STACK {
			template <typename Type>
			class Stack;
		}
		template <typename Type>
		class Flow;
	}
	namespace NODE {
		template <typename Type>
		class Node;
	}
}