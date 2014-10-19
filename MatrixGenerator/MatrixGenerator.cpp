// MatrixGenerator.cpp : Defines the entry point for the console application.
//

#include "MatrixGen.h"
#include <ctime>


int main()
{
	// Test: Создадим случайную матрицу
	CMatrixGen A;
	vector<int> Vec;
	// 2  x 100
	Vec.push_back(100);
	Vec.push_back(100);
	// 2  x 50
	Vec.push_back(50);
	Vec.push_back(50);
	// 20 x 10
	for (int i = 0; i < 20; i++)
		Vec.push_back(10);
	// 50 x 5
	for (int i = 0; i < 50; i++)
		Vec.push_back(5);
	
	A.m_ChainNodesNumber = Vec;
	A.m_AdmittancesDispersion = 0.3;
	A.m_AdditionDispersion = 1;
	A.m_BaseAdmittance = 1;
	A.m_BaseMainDiagonalAddition = 0.5;
	A.m_RandomNetAdmittancesNumber = 300;
	A.m_AdditionsNumber = 0.3;
	A.m_EDSAdmittanceBase = 1;
	A.m_EDSAdmittanceDispersion = 0;
	A.m_EDSBase = 1;
	A.m_EDSNumber = 0.1;

	A.GetMatrix();

	time_t Now = time(0);

	tm structTime;
	localtime_s(&structTime, &Now);


	string Directory = "Matrices\\";
	string FileNameM = Directory + "testM_";
	FileNameM += std::to_string((int)structTime.tm_year + 1900) + ".";
	FileNameM += std::to_string((int)structTime.tm_mon + 1) + ".";
	FileNameM += std::to_string((int)structTime.tm_mday) + "_";
	FileNameM += std::to_string((int)structTime.tm_hour) + ",";
	FileNameM += std::to_string((int)structTime.tm_min) + ",";
	FileNameM += std::to_string((int)structTime.tm_sec) + ".txt";

	string FileNameV = Directory + "testV_";
	FileNameV += std::to_string((int)structTime.tm_year + 1900) + ".";
	FileNameV += std::to_string((int)structTime.tm_mon + 1) + ".";
	FileNameV += std::to_string((int)structTime.tm_mday) + "_";
	FileNameV += std::to_string((int)structTime.tm_hour) + ",";
	FileNameV += std::to_string((int)structTime.tm_min) + ",";
	FileNameV += std::to_string((int)structTime.tm_sec) + ".txt";


	A.WriteMatrixMarketFile_LastMatrix(FileNameM);
	A.WriteMatrixMarketFile_LastVector(FileNameV);

	return 0;

}

