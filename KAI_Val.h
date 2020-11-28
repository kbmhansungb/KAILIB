/*
	Copyright (c) 2020 Kim,Bummoo
	Read KAI.h
*/
#pragma once
#include "KAI.h"

namespace KAI
{
	//
	namespace VAL {
		template <typename Type>
		class Val
		{
		protected: 
			KAI::Controller<Type>* Owner;
			//constructor is called by controller
			Val(KAI::Controller<Type>* Owner)
			{
				this->Owner = Owner;
			}
		public:
			~Val()
			{
				Owner->Vals.removeVal(this);
			}
			int Size;
			std::shared_ptr<concurrency::array_view<Type>> Ref;
			void SetSize(int Size)
			{
				this->Ref = std::make_shared<concurrency::array_view<Type>>(Size);
				this->Size = Size;
			}

			//
			//	Calculator
			//

			//Default
			void calc_set_type(Type InitVal)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] = InitVal;
				});
			}
			void calc_set_val(KAI::VAL::Val<Type>* I1)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx) restrict(amp)
				{
					o[idx] = i1[idx];
				});
			}
			void calc_set_looparrange(Val* I1, int I1arrange)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx) restrict(amp)
				{
					o[idx] = i1[idx % I1arrange];
				});
			}
			void calc_set_transpose(Val* I1, int I1_Row, int I1_Column)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] = i1[idx / I1_Column * I1_Row + idx % I1_Column];
				});
			}
			void calc_rowsum(Val* I1, int I1_Row)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					for (int i = 0; i < I1_Row; i++)
					{
						o[idx] = i1[idx + i * I1_Row];
					}
				});
			}
			void calc_columnsum(Val* I1, int I1_Column)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					for (int i = 0; i < I1_Column; i++)
					{
						o[idx] = i1[idx * I1_Column  + i];
					}
				});
			}
			void calc_times(Val* I1, int Loop)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				if (Loop >= 0)
				{
					concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
						o[idx] = 1;
						for(int i = 0; i < Loop; i++) o[idx] *= i1[idx];
					});
				}
				else // (0 > Loop)
				{
					concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
						o[idx] = 1;
						for (int i = 0; i > Loop; i--) o[idx] /= i1[idx];
					});
				}
			}
			//Assign
			void calc_assign_add(Type Val)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] += I1;
				});
			}
			void calc_assign_sub(Type Val)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] -= I1;
				});
			}
			void calc_assign_mul(Type Val)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] *= I1;
				});
			}
			void calc_assign_div(Type Val)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] /= I1;
				});
			}
			void calc_assign_add(Val* I1)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] += i1[idx];
				});
			}
			void calc_assign_sub(Val* I1)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] -= i1[idx];
				});
			}
			void calc_assign_mul(Val* I1)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] *= i1[idx];
				});
			}
			void calc_assign_div(Val* I1)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] /= i1[idx];
				});
			}
			//Array
			void calc_array_add(Val* I1, Type Const1)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] = i1[idx] + Const1;
				});
			}
			void calc_array_sub(Val* I1, Type Const1)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] = i1[idx] - Const1;
				});
			}
			void calc_array_mul(Val* I1, Type Const1)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] = i1[idx] * Const1;
				});
			}
			void calc_array_div(Val* I1, Type Const1)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] = i1[idx] / Const1;
				});
			}
			void calc_array_add(Val* I1, Val* I2)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::array_view<Type>& i2 = *I2->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] = i1[idx] + i2[idx];
				});
			}
			void calc_array_sub(Val* I1, Val* I2)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::array_view<Type>& i2 = *I2->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] = i1[idx] - i2[idx];
				});
			}
			void calc_array_mul(Val* I1, Val* I2)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::array_view<Type>& i2 = *I2->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] = i1[idx] * i2[idx];
				});
			}
			void calc_array_div(Val* I1, Val* I2)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::array_view<Type>& i2 = *I2->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] = i1[idx] / i2[idx];
				});
			}
			void calc_array_add_looparrange(Val* I1, Val* I2, int I1arrange, int I2arrange)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::array_view<Type>& i2 = *I2->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] = i1[idx % I1arrange] + i2[idx % I2arrange];
				});
			}
			void calc_array_sub_looparrange(Val* I1, Val* I2, int I1arrange, int I2arrange)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::array_view<Type>& i2 = *I2->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] = i1[idx % I1arrange] - i2[idx % I2arrange];
				});
			}
			void calc_array_mul_looparrange(Val* I1, Val* I2, int I1arrange, int I2arrange)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::array_view<Type>& i2 = *I2->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] = i1[idx % I1arrange] * i2[idx % I2arrange];
				});
			}
			void calc_array_div_looparrange(Val* I1, Val* I2, int I1arrange, int I2arrange)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::array_view<Type>& i2 = *I2->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx)restrict(amp) {
					o[idx] = i1[idx % I1arrange] / i2[idx % I2arrange];
				});
			}
			//Matrix
			void calc_matrix_add(Val* I1, Val* I2, int Row, int Column, int Matrix)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::array_view<Type>& i2 = *I2->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx) restrict(amp)
				{
					o[idx] = 0;
					for (int loop = 0; loop < Matrix; loop++)
					{
						o[idx] += i1[(idx / Row)*Matrix + loop] + i2[idx%Column + (loop*Column)];
					}
				});
			}
			void calc_matrix_sub(Val* I1, Val* I2, int Row, int Column, int Matrix)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::array_view<Type>& i2 = *I2->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx) restrict(amp)
				{
					o[idx] = 0;
					for (int loop = 0; loop < Matrix; loop++)
					{
						o[idx] += i1[(idx / Row)*Matrix + loop] - i2[idx%Column + (loop*Column)];
					}
				});
			}
			void calc_matrix_mul(Val* I1, Val* I2, int Row, int Column, int Matrix)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::array_view<Type>& i2 = *I2->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx) restrict(amp)
				{
					o[idx] = 0;
					for (int loop = 0; loop < Matrix; loop++)
					{
						o[idx] += i1[(idx / Row)*Matrix + loop] * i2[idx%Column + (loop*Column)];
					}
				});
			}
			void calc_matrix_div(Val* I1, Val* I2, int Row, int Column, int Matrix)
			{
				concurrency::array_view<Type>& o = *Ref;
				concurrency::array_view<Type>& i1 = *I1->Ref;
				concurrency::array_view<Type>& i2 = *I2->Ref;
				concurrency::parallel_for_each(o.extent, [=](concurrency::index<1> idx) restrict(amp)
				{
					o[idx] = 0;
					for (int loop = 0; loop < Matrix; loop++)
					{
						o[idx] += i1[(idx / Row)*Matrix + loop] / i2[idx%Column + (loop*Column)];
					}
				});
			}
			friend Controller<Type>;
		};
	}
}