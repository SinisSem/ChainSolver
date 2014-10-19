// MatrixGenerator.cpp : Defines the entry point for the console application.
//

#include "MatrixGen.h"


int main()
{
	// Test: Создадим случайную матрицу
	CMatrixGen A;
	vector<int> Vec(10);
	int B = 10;
	for (int i = 0; i < 10; i++)
	{
		Vec[i] = B;
		B += 10;
	}
	A.m_ChainNodesNumber = Vec;
	A.m_AdmittancesDispersion = 0.;
	A.m_AdditionDispersion = 0.3;
	A.m_BaseAdmittance = 1;
	A.m_BaseMainDiagonalAddition = 0.5;
	A.m_RandomNetAdmittancesNumber = 20;
	A.m_AdditionsNumber = 0.3;
	A.m_EDSAdmittanceBase = 1;
	A.m_EDSAdmittanceDispersion = 0;
	A.m_EDSBase = 1;
	A.m_EDSNumber = 0.1;


	A.GetMatrix();

	return 0;
}

