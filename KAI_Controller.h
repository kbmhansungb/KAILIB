#pragma once
/*
	Copyright (c) 2020 Kim,Bummoo
	Read KAI.h
*/
#include "KAI_Val.h"
#include "KAI_Flow.h"
#include "KAI_Node.h"

namespace KAI
{
	template <typename ConType, typename ValType>
	class RefController
	{
	protected:
		KAI::Controller<ValType>* Owner;
		std::map<ConType*, int> Map;

	public:
		RefController() {};
		~RefController()
		{
			clear();
		}

		void addVal(ConType* Target)
		{
			Map.insert_or_assign(Target, 0);
		}
		//		REFCONTROLLER
		void removeVal(ConType* Target)
		{
			Map.erase(Target);
		}
		void clear()
		{
			while(Map.begin()!=Map.end())
			{
				delete Map.begin()->first;
			}
		}

		friend Controller<ValType>;
	};
	template <typename Type>
	class Controller
	{
	public:
		Controller()
		{
			Vals.Owner = this;
			Flows.Owner = this;
			Nodes.Owner = this;
			MainFlow = newFlow();
		}
		~Controller()
		{
		}

		RefController<VAL::Val<Type>, Type> Vals;
		RefController<FLOW::Flow<Type>, Type> Flows;
		RefController<NODE::Node<Type>, Type> Nodes;

		bool IsMakeStackMemory = true;
		virtual void rememberStack(KAI::FLOW::STACK::Stack<Type>* RequiredStack) 
		{
			if (LastFlow == nullptr)
			{
				LastFlow = newFlow();
			}
			LastFlow->Stacks.push_back(RequiredStack);
		}
		FLOW::Flow<Type>* LastFlow = nullptr;
		FLOW::Flow<Type>* MainFlow = nullptr;

	public:
		VAL::Val<Type>* newVal(int Size)
		{
			VAL::Val<Type>* New = new VAL::Val<Type>(this);
			New->SetSize(Size);
			Vals.addVal(New);
			return New;
		}
		FLOW::Flow<Type>* newFlow()
		{
			FLOW::Flow<Type>* New = new FLOW::Flow<Type>(this);
			Flows.addVal(New);
			LastFlow = New;
			return New;
		}
		NODE::Node<Type>* newNode(int Row, int Column)
		{
			NODE::Node<Type>* New = new NODE::Node<Type>(this, Row, Column);
			New->ValIsCreatedByThis = true;
			Nodes.addVal(New);
			return New;
		}
		NODE::Node<Type>* newNodeVal(int Row, int Column)
		{
			NODE::UserNode<Type>* New = new NODE::UserNode<Type>(this, newVal(Row*Column), Row, Column);
			New->ValIsCreatedByThis = true;
			Nodes.addVal(New);
			return New;
		}
	};
}