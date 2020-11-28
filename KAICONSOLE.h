#pragma once
#include <iostream>
#include "KAI__CORE.h"

template <typename Type>
void printNodeVal(KAI::NODE::Node<Type>& Node, bool endlEveryRowEnd = false)
{
	int Row = Node.getRow();
	int Column = Node.getColumn();
	KAI::VAL::Val<Type>* Val = Node.getVal();

	std::cout << "[";
	for (int r = 0; r < Row; r++)
	{
		std::cout << "[ ";
		for (int c = 0; c < Column; c++)
		{
			std::cout << (*Val->Ref)[r*Column + c] << " ";
		}
		std::cout << "]";
		if (r < Row - 1) std::cout << (endlEveryRowEnd ? "\n" : " ");
	}
	std::cout << "]" << std::endl;
}

template <typename Type>
void printFrameStacks(KAI::FLOW::Flow<Type>& Frames, std::string front = "")
{
	int index = 0;
	for (auto Elum : Frames.Stacks)
	{
		std::cout << front << index++ << " " << typeid(*Elum).name() << "	" << Elum << std::endl;
	}
}