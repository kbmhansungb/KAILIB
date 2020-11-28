/*
	Copyright (c) 2020 Kim,Bummoo
	Read KAI.h
*/
#pragma once
#include "KAI.h"
#include "KAI_Flow.h"

namespace KAI
{
	namespace NODE {
		namespace OPERATOR {
			class MatrixClass {};
			static MatrixClass matrix;

			class TransposeClass {};
			static TransposeClass transpose;

			class RowSumClass {};
			static RowSumClass rowsum;

			class ColumnSumClass {};
			static ColumnSumClass columnsum;
		}
		template <typename Type>
		class NodeOperator_Matrix
		{
		public:
			Node<Type>* TargetNode;

			//matrix process
			Node<Type>& operator + (Node<Type>& Node)
			{
				KAI::NODE::Node<Type>* NewNode = new KAI::NODE::Node<Type>(TargetNode->User, TargetNode->Row, Node.Column);
				KAI::FLOW::STACK::MatrixAdd<Type>* New = new KAI::FLOW::STACK::MatrixAdd<Type>(TargetNode->User->LastFlow, NewNode, TargetNode, &Node);
				return *NewNode;
			}
			Node<Type>& operator - (Node<Type>& Node)
			{
				KAI::NODE::Node<Type>* NewNode = new KAI::NODE::Node<Type>(TargetNode->User, TargetNode->Row, Node.Column);
				KAI::FLOW::STACK::MatrixSub<Type>* New = new KAI::FLOW::STACK::MatrixSub<Type>(TargetNode->User->LastFlow, NewNode, TargetNode, &Node);
				return *NewNode;
			}
			Node<Type>& operator * (Node<Type>& Node)
			{
				KAI::NODE::Node<Type>* NewNode = new KAI::NODE::Node<Type>(TargetNode->User, TargetNode->Row, Node.Column);
				KAI::FLOW::STACK::MatrixMul<Type>* New = new KAI::FLOW::STACK::MatrixMul<Type>(TargetNode->User->LastFlow, NewNode, TargetNode, &Node);
				return *NewNode;
			}
			Node<Type>& operator / (Node<Type>& Node)
			{
				KAI::NODE::Node<Type>* NewNode = new KAI::NODE::Node<Type>(TargetNode->User, TargetNode->Row, Node.Column);
				KAI::FLOW::STACK::MatrixDiv<Type>* New = new KAI::FLOW::STACK::MatrixDiv<Type>(TargetNode->User->LastFlow, NewNode, TargetNode, &Node);
				return *NewNode;
			}
		};
		template <typename Type>
		class Node
		{
		protected:
			KAI::Controller<Type>* Owner;
			KAI::Controller<Type>* User;
			Node(KAI::Controller<Type>* Owner, int Row, int Column)
			{
				this->Owner = Owner;
				this->User = Owner;
				this->Row = Row;
				this->Column = Column;
			}
		public:
			~Node()
			{
				Owner->Nodes.removeVal(this);
			}
			/*std::shared_ptr<std::vector<Node<Type>*>> RequiredNodes;
			void addRequiredNodes(Node<Type>* RequiredNode)
			{
				if (!RequiredNodes.get())
				{
					RequiredNodes = std::make_shared < std::vector<Node<Type>*>();
				}
				
				std::vector<Node<Type>*>& Vector = *RequiredNodes;
				Vector.push_back(RequiredNode);
			}*/
		protected:
			bool ValIsCreatedByThis = false;
			VAL::Val<Type>* ValRef = nullptr;
			int Row;
			int Column;

			//RememberFormTranse
			Node<Type>* TransposedNode = nullptr;
			Node<Type>* RowSumNode = nullptr;
			Node<Type>* ColumnSumNode = nullptr;

		public:
			virtual VAL::Val<Type>* getVal()
			{
				if (this->ValRef == nullptr)
				{
					this->ValRef = this->Owner->newVal(Row*Column);
					ValIsCreatedByThis = true;
				}
				return ValRef;
			}
			const int getSize() { return ValRef->Size; }
			const int getRow() { return Row; }
			const int getColumn() { return Column; }

			//
			//	Operator
			//

			//default process

			Node<Type>& operator + (Type Const)
			{
				KAI::NODE::Node<Type>* NewNode = new KAI::NODE::Node<Type>(User, this->Row, this->Column);
				KAI::FLOW::STACK::ArrayAddByConst<Type>* New = new KAI::FLOW::STACK::ArrayAddByConst<Type>(User->LastFlow, NewNode, this, Const);
				return *NewNode;
			}
			Node<Type>& operator - (Type Const)
			{
				KAI::NODE::Node<Type>* NewNode = new KAI::NODE::Node<Type>(User, this->Row, this->Column);
				KAI::FLOW::STACK::ArraySubByConst<Type>* New = new KAI::FLOW::STACK::ArraySubByConst<Type>(User->LastFlow, NewNode, this, Const);
				return *NewNode;
			}
			Node<Type>& operator * (Type Const)
			{
				KAI::NODE::Node<Type>* NewNode = new KAI::NODE::Node<Type>(User, this->Row, this->Column);
				KAI::FLOW::STACK::ArrayMulByConst<Type>* New = new KAI::FLOW::STACK::ArrayMulByConst<Type>(User->LastFlow, NewNode, this, Const);
				return *NewNode;
			}
			Node<Type>& operator / (Type Const)
			{
				KAI::NODE::Node<Type>* NewNode = new KAI::NODE::Node<Type>(User, this->Row, this->Column);
				KAI::FLOW::STACK::ArrayDivByConst<Type>* New = new KAI::FLOW::STACK::ArrayDivByConst<Type>(User->LastFlow, NewNode, this, Const);
				return *NewNode;
			}
			Node<Type>& operator + (Node<Type>& Node)
			{
				KAI::NODE::Node<Type>* NewNode = new KAI::NODE::Node<Type>(User, this->Row, Node.Column);
				KAI::FLOW::STACK::ArrayAdd<Type>* New = new KAI::FLOW::STACK::ArrayAdd<Type>(User->LastFlow, NewNode, this, &Node);
				return *NewNode;
			}
			Node<Type>& operator - (Node<Type>& Node)
			{
				KAI::NODE::Node<Type>* NewNode = new KAI::NODE::Node<Type>(User, this->Row, Node.Column);
				KAI::FLOW::STACK::ArraySub<Type>* New = new KAI::FLOW::STACK::ArraySub<Type>(User->LastFlow, NewNode, this, &Node);
				return *NewNode;
			}
			Node<Type>& operator * (Node<Type>& Node)
			{
				KAI::NODE::Node<Type>* NewNode = new KAI::NODE::Node<Type>(User, this->Row, Node.Column);
				KAI::FLOW::STACK::ArrayMul<Type>* New = new KAI::FLOW::STACK::ArrayMul<Type>(User->LastFlow, NewNode, this, &Node);
				return *NewNode;
			}
			Node<Type>& operator / (Node<Type>& Node)
			{
				KAI::NODE::Node<Type>* NewNode = new KAI::NODE::Node<Type>(User, this->Row, Node.Column);
				KAI::FLOW::STACK::ArrayDiv<Type>* New = new KAI::FLOW::STACK::ArrayDiv<Type>(User->LastFlow, NewNode, this, &Node);
				return *NewNode;
			}

			//assign

			void operator += (Node<Type>& Node)
			{
				KAI::FLOW::STACK::AssignAdd<Type>* New = new KAI::FLOW::STACK::AssignAdd<Type>(User->LastFlow, this, &Node);
			}
			void operator -= (Node<Type>& Node)
			{
				KAI::FLOW::STACK::AssignSub<Type>* New = new KAI::FLOW::STACK::AssignSub<Type>(User->LastFlow, this, &Node);
			}
			void operator *= (Node<Type>& Node)
			{
				KAI::FLOW::STACK::AssignMul<Type>* New = new KAI::FLOW::STACK::AssignMul<Type>(User->LastFlow, this, &Node);
			}
			void operator /= (Node<Type>& Node)
			{
				KAI::FLOW::STACK::AssignDiv <Type>* New = new KAI::FLOW::STACK::AssignDiv<Type>(User->LastFlow, this, &Node);
			}

			void operator = (Type TypeVal)
			{
				KAI::FLOW::STACK::SetByType<Type>* New = new KAI::FLOW::STACK::SetByType<Type>(User->LastFlow, this, TypeVal);
			}
			void operator = (Node<Type>& Node)
			{
				//if (Node.ValRef == nullptr)
				//{	//Node가 다이나믹인 경우
				//	Node.ValRef = this->ValRef;
				//}
				//else
				//{	//UserNode인 경우
				//	KAI::FLOW::STACK::SetByNode<Type>* New = new KAI::FLOW::STACK::SetByNode<Type>(User->LastFlow, this, &Node);
				//} removed this absolutly.
				KAI::FLOW::STACK::SetByNode<Type>* New = new KAI::FLOW::STACK::SetByNode<Type>(User->LastFlow, this, &Node);
			}
			Node<Type>& operator ^ (int Times)
			{
				KAI::NODE::Node<Type>* NewNode = new KAI::NODE::Node<Type>(User, Column, Row);
				KAI::FLOW::STACK::Times<Type>* New = new KAI::FLOW::STACK::Times<Type>(User->LastFlow, NewNode, this, Times);
				return *NewNode;
			}

			//matrix process
			NodeOperator_Matrix<Type> operator[] (OPERATOR::MatrixClass& Operator)
			{
				NodeOperator_Matrix<Type> MatrixOperator;
				MatrixOperator.TargetNode = this;
				return MatrixOperator;
			}
			Node<Type>& operator [] (OPERATOR::TransposeClass& Operator)
			{
				if (TransposedNode == nullptr)
					TransposedNode = new KAI::NODE::Node<Type>(User, Column, Row);
				KAI::FLOW::STACK::Transpose<Type>* New = new KAI::FLOW::STACK::Transpose<Type>(User->LastFlow, TransposedNode, this);
				return *TransposedNode;
			}
			Node<Type>& operator [] (OPERATOR::RowSumClass& Operator)
			{
				if (RowSumNode == nullptr)
					RowSumNode = new KAI::NODE::Node<Type>(User, 1, Column);
				KAI::FLOW::STACK::RowSum<Type>* New = new KAI::FLOW::STACK::RowSum<Type>(User->LastFlow, RowSumNode, this);
				return *RowSumNode;
			}
			Node<Type>& operator [] (OPERATOR::ColumnSumClass& Operator)
			{
				if (ColumnSumNode == nullptr)
					ColumnSumNode = new KAI::NODE::Node<Type>(User, Row, 1);
				KAI::FLOW::STACK::ColumnSum<Type>* New = new KAI::FLOW::STACK::ColumnSum<Type>(User->LastFlow, ColumnSumNode, this);
				return *ColumnSumNode;
			}
			//node process
			Node<Type>& operator() (KAI::Controller<Type>* ChangeUser)
			{
				this->User = ChangeUser;
				return *this;
			}

			void init(Type InitVal)
			{
				getVal()->calc_set_type(InitVal);
			}
			friend NodeOperator_Matrix<Type>;
			friend KAI::Controller<Type>;
		};

		template<typename Type>
		class UserNode : public Node<Type>
		{
		protected:
			UserNode(Controller<Type>* Controller, VAL::Val<Type>* ValRef, int Row, int Column) : Node<Type>(Controller, Row, Column)
			{
				this->ValRef = ValRef;
			}
		public:
			~UserNode()
			{
			}
			friend KAI::Controller<Type>;
		};
	}
}