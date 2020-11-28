/*
	Copyright (c) 2020 Kim,Bummoo
	Read KAI.h
*/
#pragma once
#include "KAI.h"
#include "KAI_Controller.h"
#include "KAI_Val.h"

namespace KAI
{
	//
	namespace FLOW {
		template <typename Type>
		class Flow;
		template <typename Type>
		class DiffResult;


		namespace STACK {
			template <typename Type>
			class Stack
			{
			public:
				//Stack is created in node operator
				KAI::FLOW::Flow<Type>* Owner;
				Stack(KAI::FLOW::Flow<Type>* Owner)
				{
					this->Owner = Owner;
					Owner->Stacks.push_back(this);
				}
				~Stack()
				{
				}
				virtual void calc() {}
				virtual void diff(DiffResult<Type>& Con) {}
			};
			//
			//	TwoParmCalc
			//	O1 = I1 ? I2	Done
			//
			template<typename Type>
			class TwoParmCalc : public Stack<Type>
			{
			public:
				TwoParmCalc(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, KAI::NODE::Node<Type>* I2) : Stack<Type>(Owner)
				{
					this->O1 = O1;
					this->I1 = I1;
					this->I2 = I2;
				}
			public:
				KAI::NODE::Node<Type>* O1;
				KAI::NODE::Node<Type>* I1;
				KAI::NODE::Node<Type>* I2;
				friend KAI::NODE::Node<Type>;
			};
			//
			//	OneParmCalc
			//	O1 ?= I1	Done
			//
			template<typename Type>
			class OneParmCalc : public Stack<Type>
			{
			public:
				OneParmCalc(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1) : Stack<Type>(Owner)
				{
					this->O1 = O1;
					this->I1 = I1;
				}
			public:
				KAI::NODE::Node<Type>* O1;
				KAI::NODE::Node<Type>* I1;
			};
			//
			//	OneParmOneTypeCalc
			//	O1 ?= I1	Done
			//
			template<typename Type>
			class OneParmOneTypeCalc : public Stack<Type>
			{
			public:
				OneParmOneTypeCalc(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, Type Const1) : Stack<Type>(Owner)
				{
					this->O1 = O1;
					this->I1 = I1;
					this->Const1 = Const1;
				}
			public:
				KAI::NODE::Node<Type>* O1;
				KAI::NODE::Node<Type>* I1;
				Type Const1;
			};
			//
			//	OneTypeCalc
			//	O1 ?= Const1
			//
			template<typename Type>
			class OneTypeCalc : public Stack<Type>
			{
			public:
				OneTypeCalc(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, Type Const1) : Stack<Type>(Owner)
				{
					this->O1 = O1;
					this->Const1 = Const1;
				}
			public:
				KAI::NODE::Node<Type>* O1;
				Type Const1;
			};

			// Assign
			// 

			template<typename Type>
			class AssignAddByType : public OneTypeCalc<Type>
			{
			public:
				AssignAddByType(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, Type TypeVal) :
					OneTypeCalc<Type>(Owner, O1, TypeVal) {}
				virtual void calc() override
				{
					O1->getVal()->calc_assign_add(O1->getVal(), Const1);
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					//dO1 *= (Type)1;
				}
			};
			template<typename Type>
			class AssignSubByType : public OneTypeCalc<Type>
			{
			public:
				AssignSubByType(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, Type TypeVal) :
					OneTypeCalc<Type>(Owner, O1, TypeVal) {}
				virtual void calc() override
				{
					O1->getVal()->calc_assign_sub(O1->getVal(), TypeVal);
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					//dO1 *= (Type)1;
				}
			};
			template<typename Type>
			class AssignMulByType : public OneTypeCalc<Type>
			{
			public:
				AssignMulByType(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, Type TypeVal) :
					OneTypeCalc<Type>(Owner, O1, TypeVal) {}
				virtual void calc() override
				{
					O1->getVal()->calc_assign_sub(O1->getVal(), TypeVal);
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);

					dO1 *= (Type)Const1;
				}
			};
			template<typename Type>
			class AssignDivByType : public OneTypeCalc<Type>
			{
			public:
				AssignDivByType(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, Type TypeVal) :
					OneTypeCalc<Type>(Owner, O1, TypeVal) {}
				virtual void calc() override
				{
					O1->getVal()->calc_assign_sub(O1->getVal(), TypeVal);
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);

					dO1 /= (Type)Const1;
				}
			};

			template<typename Type>
			class AssignAdd : public OneParmCalc<Type>
			{
			public:
				AssignAdd(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1)
					: OneParmCalc<Type>(Owner, O1, I1) {}
				virtual void calc() override
				{
					O1->getVal()->calc_assign_add(I1->getVal());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);

					//dO1 *= (Type)1;
					dI1 += dO1;
				}
			};
			template<typename Type>
			class AssignSub : public OneParmCalc<Type>
			{
			public:
				AssignSub(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1)
					: OneParmCalc<Type>(Owner, O1, I1) {}
				virtual void calc() override
				{
					O1->getVal()->calc_assign_sub(I1->getVal());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);

					//dO1 *= (Type)1;
					dI1 -= dO1;// * (Type)-1;
				}
			};
			template<typename Type>
			class AssignMul : public OneParmCalc<Type>
			{
			public:
				AssignMul(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1)
					: OneParmCalc<Type>(Owner, O1, I1) {}
				virtual void calc() override
				{
					O1->getVal()->calc_assign_mul(I1->getVal());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);

					dO1 *= I1;
					dI1 += dO1 * O1;
				}
			};
			template<typename Type>
			class AssignDiv : public OneParmCalc<Type>
			{
			public:
				AssignDiv(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1)
					: OneParmCalc<Type>(Owner, O1, I1) {}
				virtual void calc() override
				{
					O1->getVal()->calc_assign_div(I1->getVal());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);

					dO1 /= I1;
					//dI1 += dO1 * O1;
				}
			};

			// Array

			template<typename Type>
			class ArrayAddByConst : public OneParmOneTypeCalc<Type>
			{
			public:
				ArrayAddByConst(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, Type Const1) :
					OneParmOneTypeCalc<Type>(Owner, O1, I1, Const1) {}
				virtual void calc() override
				{
					O1->getVal()->calc_array_add(I1->getVal(), Const1);
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);
					dI1 += dO1;
				}
			};
			template<typename Type>
			class ArraySubByConst : public OneParmOneTypeCalc<Type>
			{
			public:
				ArraySubByConst(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, Type Const1) :
					OneParmOneTypeCalc<Type>(Owner, O1, I1, Const1) {}
				virtual void calc() override
				{
					O1->getVal()->calc_array_sub(I1->getVal(), Const1);
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);
					dI1 += dO1;
				}
			};
			template<typename Type>
			class ArrayMulByConst : public OneParmOneTypeCalc<Type>
			{
			public:
				ArrayMulByConst(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, Type Const1) :
					OneParmOneTypeCalc<Type>(Owner, O1, I1, Const1) {}
				virtual void calc() override
				{
					O1->getVal()->calc_array_mul(I1->getVal(), Const1);
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);
					dI1 += dO1 * Const1;
				}
			};
			template<typename Type>
			class ArrayDivByConst : public OneParmOneTypeCalc<Type>
			{
			public:
				ArrayDivByConst(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, Type Const1) :
					OneParmOneTypeCalc<Type>(Owner, O1, I1, Const1) {}
				virtual void calc() override
				{
					O1->getVal()->calc_array_div(I1->getVal(), Const1);
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);
					dI1 += dO1 / Const1;
				}
			};

			template<typename Type>
			class ArrayAdd : public TwoParmCalc<Type>
			{
			public:
				ArrayAdd(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, KAI::NODE::Node<Type>* I2) : 
					TwoParmCalc<Type>(Owner, O1, I1, I2) {}	
				virtual void calc() override
				{
					O1->getVal()->calc_array_add_looparrange(I1->getVal(), I2->getVal(), I1->getSize(), I2->getSize());
				}	
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);
					KAI::NODE::Node<Type>& dI2 = Con(*I2);

					dI1 += dO1;// *(Type)1;
					dI2 += dO1;// +(*I1);
				}
			};
			template<typename Type>
			class ArraySub : public TwoParmCalc<Type>
			{
			public:
				ArraySub(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, KAI::NODE::Node<Type>* I2)
					: TwoParmCalc<Type>(Owner, O1, I1, I2) {}
				virtual void calc() override
				{
					O1->getVal()->calc_array_sub_looparrange(I1->getVal(), I2->getVal(), I1->getSize(), I2->getSize());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);
					KAI::NODE::Node<Type>& dI2 = Con(*I2);

					dI1 += dO1;// - (*I2);
					dI2 -= dO1;;
				}
			};
			template<typename Type>
			class ArrayMul : public TwoParmCalc<Type>
			{
			public:
				ArrayMul(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, KAI::NODE::Node<Type>* I2)
					: TwoParmCalc<Type>(Owner, O1, I1, I2) {}
				virtual void calc() override
				{
					O1->getVal()->calc_array_mul_looparrange(I1->getVal(), I2->getVal(), I1->getSize(), I2->getSize());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);
					KAI::NODE::Node<Type>& dI2 = Con(*I2);

					dI1 += dO1 * (*I2);
					dI2 += dO1 * (*I1);
				}
			};
			template<typename Type>
			class ArrayDiv : public TwoParmCalc<Type>
			{
			public:
				ArrayDiv(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, KAI::NODE::Node<Type>* I2)
					: TwoParmCalc<Type>(Owner, O1, I1, I2) {}
				virtual void calc() override
				{
					O1->getVal()->calc_array_div_looparrange(I1->getVal(), I2->getVal(), I1->getSize(), I2->getSize());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);
					KAI::NODE::Node<Type>& dI2 = Con(*I2);

					dI1 += dO1 / (*I2);
					dI2 += dO1 * (*I1);
				}
			};

			//
			//MatrixCalc
			//

			template<typename Type>
			class MatrixAdd : public TwoParmCalc<Type>
			{
			public:
				MatrixAdd(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, KAI::NODE::Node<Type>* I2)
					: TwoParmCalc<Type>(Owner, O1, I1, I2) {}
				virtual void calc() override
				{
					O1->getVal()->calc_matrix_add(I1->getVal(), I2->getVal(), I1->getRow(), I2->getColumn(), I1->getColumn());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);
					KAI::NODE::Node<Type>& dI2 = Con(*I2);

					dI1 = dI1 + dO1[columnsum]; // temporary
					dI2 = dI2 + dO1[rowsum]; // temporary
				}
			};
			template<typename Type>
			class MatrixSub : public TwoParmCalc<Type>
			{
			public:
				MatrixSub(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, KAI::NODE::Node<Type>* I2)
					: TwoParmCalc<Type>(Owner, O1, I1, I2) {}
				virtual void calc() override
				{
					O1->getVal()->calc_matrix_sub(I1->getVal(), I2->getVal(), I1->getRow(), I2->getColumn(), I1->getColumn());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);
					KAI::NODE::Node<Type>& dI2 = Con(*I2);

					dI1 = dI1 + dO1[columnsum]; // temporary
					dI2 = dI2 - dO1[rowsum]; // temporary
				}
			};
			template<typename Type>
			class MatrixMul : public TwoParmCalc<Type>
			{
			public:
				MatrixMul(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, KAI::NODE::Node<Type>* I2)
					: TwoParmCalc<Type>(Owner, O1, I1, I2) {}
				virtual void calc() override
				{
					O1->getVal()->calc_matrix_mul(I1->getVal(), I2->getVal(), I1->getRow(), I2->getColumn(), I1->getColumn());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);
					KAI::NODE::Node<Type>& dI2 = Con(*I2);

					dI1 += dO1 * (*I2)[KAI::NODE::OPERATOR::transpose];
					KAI::NODE::Node<Type>& cI1 = *Con.Based->newNode(I1->getRow(), I1->getColumn());
					dI2 += (*I1)[KAI::NODE::OPERATOR::transpose](Con.Based) * dO1;
				}
			};
			template<typename Type>
			class MatrixDiv : public TwoParmCalc<Type>
			{
			public:
				MatrixDiv(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, KAI::NODE::Node<Type>* I2)
					: TwoParmCalc<Type>(Owner, O1, I1, I2) {}
				virtual void calc() override
				{
					O1->getVal()->calc_matrix_div(I1->getVal(), I2->getVal(), I1->getRow(), I2->getColumn(), I1->getColumn());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);
					KAI::NODE::Node<Type>& dI2 = Con(*I2);

					dI1 += dO1 * (*I2)[KAI::NODE::OPERATOR::transpose];
					KAI::NODE::Node<Type>& cI1 = *Con.Based->newNode(I1->getRow(), I1->getColumn());
					dI2 += (*I1)[KAI::NODE::OPERATOR::transpose](Con.Based) * dO1;
				}
			};

			//OtherCalc


			template<typename Type>
			class SetByType : public OneTypeCalc<Type>
			{
			public:
				SetByType(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, Type Const1)
					: OneTypeCalc<Type>(Owner, O1, Const1) {}
				virtual void calc() override
				{
					O1->getVal()->calc_set_type(Const1);
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
				}
			};
			template<typename Type>
			class SetByNode : public OneParmCalc<Type>
			{
			public:
				SetByNode(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1)
					: OneParmCalc<Type>(Owner, O1, I1) {}
				virtual void calc() override
				{
					O1->getVal()->calc_set_looparrange(I1->getVal(), I1->getSize());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);

					dI1 = dO1;
				}
			};
			template<typename Type>
			class Times : public OneParmCalc<Type>
			{
			public:
				Times(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1, int Times)
					: OneParmCalc<Type>(Owner, O1, I1), Loop(Times) {}
				int Loop;
				virtual void calc() override
				{
					O1->getVal()->calc_times(I1->getVal(), Loop);
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);

					dI1 += dO1*((*I1)(Con.Based)^(Loop-1));
				}
			};
			template<typename Type>
			class Transpose : public OneParmCalc<Type>
			{
			public:
				Transpose(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1)
					: OneParmCalc<Type>(Owner, O1, I1) {}
				virtual void calc() override
				{
					O1->getVal()->calc_set_transpose(I1->getVal(), I1->getRow(), I1->getColumn());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);

					dI1 += dO1[KAI::NODE::OPERATOR::transpose];
				}
			};
			template<typename Type>
			class RowSum : public OneParmCalc<Type>
			{
			public:
				RowSum(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1)
					: OneParmCalc<Type>(Owner, O1, I1) {}
				virtual void calc() override
				{
					O1->getVal()->calc_rowsum(I1->getVal(), I1->getRow());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);

					dI1 += dO1;
				}
			};
			template<typename Type>
			class ColumnSum : public OneParmCalc<Type>
			{
			public:
				ColumnSum(KAI::FLOW::Flow<Type>* Owner, KAI::NODE::Node<Type>* O1, KAI::NODE::Node<Type>* I1)
					: OneParmCalc<Type>(Owner, O1, I1) {}
				virtual void calc() override
				{
					O1->getVal()->calc_columnsum(I1->getVal(), I1->getColumn());
				}
				virtual void diff(DiffResult<Type>& Con) override
				{
					KAI::NODE::Node<Type>& dO1 = Con(*O1);
					KAI::NODE::Node<Type>& dI1 = Con(*I1);

					dI1 += dO1;
				}
			};
		}

		//미분결과는 컨트롤러가 기억하지 않음.
		template <typename Type>
		class DiffResult
		{	
		public:
			KAI::Controller<Type>* Based;
			//do not change this.
			Flow<Type>* DiffFlow;
			//do not change this.
			std::map<KAI::NODE::Node<Type>*, KAI::NODE::Node<Type>*> Map;
			DiffResult(KAI::Controller<Type>* Based) : Based(Based)
			{
				DiffFlow = Based->newFlow();
			}
			KAI::NODE::Node<Type>& operator()(KAI::NODE::Node<Type>& Key)
			{
				KAI::NODE::Node<Type>* Return;
				if (Map.find(&Key) == Map.end())
				{
					Return = Based->newNode(Key.getRow(), Key.getColumn());
					//Return->init(0);
					*Return = 0;
					Map.insert(std::make_pair(&Key, Return));
				}
				else
				{
					Return = Map[&Key];
				}
				return *Return;
			}
		};
		template <typename Type>
		class Flow
		{
		protected:
			KAI::Controller<Type>* Owner;

			Flow(Controller<Type>* Owner)
			{
				this->Owner = Owner;
			}
		public:
			~Flow()
			{
				Owner->Flows.removeVal(this);
				for (auto Elum : Stacks) delete Elum;
			}
			std::vector<STACK::Stack<Type>*> Stacks;
			void through()
			{
				for (auto Elum : Stacks) Elum->calc();
			}
			void trace(DiffResult<Type>& DiffResultContainer, KAI::NODE::Node<Type>& DiffTarget)
			{
				KAI::NODE::Node<Type>& DiffTargetPair = DiffResultContainer(DiffTarget);
				DiffTargetPair = 1;
				for (auto it = Stacks.rbegin(); it != Stacks.rend(); it++) (*it)->diff(DiffResultContainer);
			}
			friend Controller<Type>;
		};
	}
}