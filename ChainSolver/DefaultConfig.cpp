#include "DefaultConfig.h"

RandomChainMatrixGenerator::CMatrixGen
CreateDefaultChains()
{
	RandomChainMatrixGenerator::CMatrixGen A;
	vector<int> Vec;
	// 2  x 100
	Vec.push_back(128);
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
	A.m_RandomNetAdmittancesNumber = 50;
	A.m_AdditionsNumber = 0.3;
	A.m_EDSAdmittanceBase = 1;
	A.m_EDSAdmittanceDispersion = 0;
	A.m_EDSDispersion = 0;
	A.m_EDSBase = 1;
	A.m_EDSNumber = 0.1;

	A.GetMatrix();
	return A;
}