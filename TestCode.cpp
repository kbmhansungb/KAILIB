#include <iostream>
#include "KAICONSOLE.h"
using namespace KAI::NODE::OPERATOR;

void operatorTest_Assign()
{
	KAI::Controller<float> Con;
	auto& I1 = *Con.newNodeVal(2, 4);
	I1.init(2.0f);
	auto& I2 = *Con.newNodeVal(2, 4);
	I2.init(3.0f);

	//Not allowed.
	I1 += I2;
	Con.MainFlow->through();

	//working
	//KAI::FLOW::STACK::AssignAdd<float> AssignAdd(Con.MainFlow, &I1, &I2);
	//AssignAdd.calc();

	//working
	//I1.getVal()->calc_assign_add(I2.getVal());
	
	printNodeVal(I1);
	printNodeVal(I2);
	printFrameStacks(*Con.MainFlow);
}

void diffTest_Array()
{
	KAI::Controller<float> Con;
	auto& O1 = *Con.newNodeVal(2, 4);
	auto& I1 = *Con.newNodeVal(2, 4);
	I1.init(2.0f);
	auto& I2 = *Con.newNodeVal(2, 4);
	I2.init(3.0f);

	O1 = I1 + I2;
	auto& E = (O1 - O1)^2;
	Con.MainFlow->through();

	printFrameStacks(*Con.MainFlow);
	printNodeVal(O1);

	//Diff
	KAI::FLOW::DiffResult<float> DiffResult(&Con);
	Con.MainFlow->trace(DiffResult, E);
	DiffResult.DiffFlow->through();

	printFrameStacks(*DiffResult.DiffFlow);
	printNodeVal(DiffResult(I1));
}

void diffTest_Matrix()
{
	KAI::Controller<float> Con;
	auto& D1 = *Con.newNodeVal(2, 4);
	D1.init(1.0f);

	auto& O1 = *Con.newNodeVal(2, 4);
	auto& I1 = *Con.newNodeVal(2, 4);
	I1.init(2.0f);
	auto& I2 = *Con.newNodeVal(4, 4);
	I2.init(3.0f);

	O1 = I1[matrix] + I2;
	auto& E = (D1 - O1) * (D1 - O1);

	Con.MainFlow->through();
}

void error_minimize_test()
{
	KAI::Controller<float> Con;
	auto MainFlow = Con.MainFlow;

	//행렬 계산 함수.
	auto& I1 = *Con.newNodeVal(1, 4);
	auto& B1 = *Con.newNodeVal(4, 1);
	auto& D1 = *Con.newNodeVal(1, 1);

	I1.init(1.0f);
	B1.init(3.0f);
	D1.init(1.0);

	auto& O1 = I1[matrix] * B1;
	auto& E = (D1 - O1) ^ 2;

	//역전파 함수.
	KAI::FLOW::DiffResult<float> DiffResult(&Con);
	MainFlow->trace(DiffResult, E);

	//학습 함수.
	auto LearningFlow = Con.newFlow();
	B1 -= DiffResult(B1) * 0.01f;

	//학습 테스트.
	for (int i = 0; i < 100; i++)
	{
		Con.MainFlow->through();
		DiffResult.DiffFlow->through();
		LearningFlow->through();

		//std::cout << "T : " << i << std::endl;
		if (i % 5 == 0)
		{
			std::cout << "Test Num : " << i << std::endl;
			std::cout << " B1 is		: "; printNodeVal(B1);
			std::cout << " O1 is		: "; printNodeVal(O1);
			std::cout << " E  is		: "; printNodeVal(E);
			std::cout << " E dif		: "; printNodeVal(DiffResult(E));
			std::cout << " O1dif		: "; printNodeVal(DiffResult(O1));
			std::cout << " B1dif		: "; printNodeVal(DiffResult(B1));
			std::cout << std::endl;
			system("pause");
		}
	}
}

int main()
{
	error_minimize_test();
	system("pause");
}