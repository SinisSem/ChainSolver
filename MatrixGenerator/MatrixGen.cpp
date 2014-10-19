#include "MatrixGen.h"
#include <stdexcpt.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <numeric>

CMatrixGen::CMatrixGen()
{
	std::srand(std::time(0));
}


CMatrixGen::~CMatrixGen()
{
}

int CMatrixGen::CreateCSR()
{
// -------------------------------------------------------------------------------------------------------
	if (m_ChainNodesNumber.empty() == true)
		return 1;								// С пустым набором цепей не начинать
	if (m_BaseAdmittance == 0)
		return 2;								// Базовая проводимость не может быть == 0
	if (m_BaseMainDiagonalAddition == 0)
		return 3;								// Базовая добавка для диагонали не может быть == 0
	if (m_RandomNetAdmittancesNumber < m_ChainNodesNumber.size() - 1)
		return 4;								// Число случайных связей - хотя бы == числу цепочек - 1
	for (int idx = 0; idx < m_ChainNodesNumber.size(); idx++)
	{
		if (m_ChainNodesNumber[idx] < 2)
			return 5;							// В цепочке должно быть хотябы 2 узла
	}
	// Избавимся от старых значений
	m_LastMatrix.col_idx.clear();
	m_LastMatrix.row_idx.clear();
	m_LastMatrix.val.clear();
// -------------------------------------------------------------------------------------------------------

	m_NodesNumber = GetNodesAmount();
	m_RandomNetAdmittancesLeft = m_RandomNetAdmittancesNumber;
	int ElementsNumber = m_NodesNumber + 2 * (m_NodesNumber - 1 + m_RandomNetAdmittancesNumber);

	// Соберем кучу цепей с необходимыми для связности связями
	CreateChains();

	// Соберем целую матрицу
	for (int chainIdx = 0; chainIdx < m_Chains.size(); chainIdx++)
	{
		MatrixCOO* ChainMTX = &(m_Chains[chainIdx].ChainMatrix);
		for (int elementIdx = 0; elementIdx < ChainMTX->val.size(); elementIdx++)
		{
			double elVal = ChainMTX->val[elementIdx];
			int elRow = ChainMTX->row_idx[elementIdx];
			int elCol = ChainMTX->col_idx[elementIdx];
			m_LastMatrix.AddElement(elVal, elRow, elCol);
		}
	}

	// Добавим проводимости шунта
	vector<int> WhereAdd = RandomVectorInsert(m_NodesNumber, m_NodesNumber * m_AdditionsNumber);
	
	for (int addIdx = 0; addIdx < WhereAdd.size(); addIdx++)
	{
		for (int elementIdx = 0; elementIdx < m_LastMatrix.val.size(); elementIdx++)
		{
			if (m_LastMatrix.row_idx[elementIdx] == m_LastMatrix.col_idx[elementIdx] &&
				m_LastMatrix.row_idx[elementIdx] == WhereAdd[addIdx])
			{
				double AddAdmittance = RandomDouble(m_BaseMainDiagonalAddition, m_AdditionDispersion);
				m_LastMatrix.val[elementIdx] += AddAdmittance;
				break;
			}
		}
	}

	// Добавим ЭДС
	vector<int> WhereEDS = RandomVectorInsert(m_NodesNumber, m_NodesNumber * m_EDSNumber);

	m_LastVector = vector<double>(m_NodesNumber, 0.0);
	
	for (int addIdx = 0; addIdx < WhereEDS.size(); addIdx++)
	{
		for (int elementIdx = 0; elementIdx < m_LastMatrix.val.size(); elementIdx++)
		{
			if (m_LastMatrix.row_idx[elementIdx] == m_LastMatrix.col_idx[elementIdx] &&
				m_LastMatrix.row_idx[elementIdx] == WhereEDS[addIdx])
			{
				double AddAdmittance = RandomDouble(m_EDSAdmittanceBase, m_EDSAdmittanceDispersion);
				m_LastMatrix.val[elementIdx] += AddAdmittance;
				double EDSValue = RandomDouble(m_EDSBase, m_EDSDispersion);
				m_LastVector[WhereEDS[addIdx]] = EDSValue * AddAdmittance;
				break;
			}
		}
	}
}

MatrixCOO CMatrixGen::GetMatrix()
{
	int Check;
	Check = CreateCSR();
	return m_LastMatrix;
}

MatrixCOO CMatrixGen::GetLastMatrix()
{
	return m_LastMatrix;
}

int CMatrixGen::GetNodesAmount()
{
	int NodesNumber = 0;
	for (int chainIdx = 0; chainIdx < m_ChainNodesNumber.size(); chainIdx++)
	{
		NodesNumber += m_ChainNodesNumber[chainIdx];
	}
	return NodesNumber;
}

void CMatrixGen::CreateChains()
{
	// Перед построением новых цепей, убедимся, что предыдущие цепи удалены
	m_Chains.clear();

	double Yii;
	double Yimj;
	double Yij;
	double Yimi;
	int nodeIdx = 0;
	// 1. Просто создадим цепи
	for (int chainIdx = 0; chainIdx < m_ChainNodesNumber.size(); chainIdx++)
	{
		Chain Current;
		Current.ChainNumber = chainIdx;
		Current.NodesNumber = m_ChainNodesNumber[chainIdx];
		Current.StartNode = nodeIdx;
		Yimj = 0.0;
		for (int nodeNumber = 0; nodeNumber < m_ChainNodesNumber[chainIdx]; nodeNumber++)
		{
			Yij = GetRandomAdmittance();
			Yii = -Yij - Yimj;
			Current.ChainMatrix.AddElement(Yii, nodeIdx, nodeIdx);
			if (nodeNumber == m_ChainNodesNumber[chainIdx] - 1)
			{
				nodeIdx++;
				break;
			}
			Current.ChainMatrix.AddElement(Yij, nodeIdx, nodeIdx + 1);
			Current.ChainMatrix.AddElement(Yij, nodeIdx + 1, nodeIdx);
			Yimj = Yij;
			nodeIdx++;
		}
		m_Chains.push_back(Current);
	}

	// 2. Накидаем в цепь связей, обеспечивающих связность сети в целом
	for (int chainIdx = 1; chainIdx < m_Chains.size(); chainIdx++)
	{
		// У каждой цепи должна быть хотябы одна связь
		int NewLink = RandomInt(0, chainIdx - 1);		// Выбрать нужно цепь, из тех, что уже в связной области
		// Случайно выберем откуда и куда придут связи внутри заданных цепей NewLink и chainIdx
		int NodeFrom = RandomInt(m_Chains[chainIdx].StartNode, m_Chains[chainIdx].StartNode + m_Chains[chainIdx].NodesNumber - 1);
		int NodeTo = RandomInt(m_Chains[NewLink].StartNode, m_Chains[NewLink].StartNode + m_Chains[NewLink].NodesNumber - 1);

		double NewLinkAdmittance = GetRandomAdmittance();

		for (int elementIdx = 0; elementIdx < m_Chains[chainIdx].ChainMatrix.row_idx.size(); elementIdx++)
		{
			int elementCol = m_Chains[chainIdx].ChainMatrix.col_idx[elementIdx];
			int elementRow = m_Chains[chainIdx].ChainMatrix.row_idx[elementIdx];

			if (elementRow == NodeFrom	&&
				elementRow == elementCol)
			{
				m_Chains[chainIdx].ChainMatrix.val[elementIdx] -= NewLinkAdmittance;
				m_Chains[chainIdx].ChainMatrix.AddElementInto(NewLinkAdmittance, NodeFrom, NodeTo);
				break;
			}
		}

		for (int elementIdx = 0; elementIdx < m_Chains[NewLink].ChainMatrix.row_idx.size(); elementIdx++)
		{
			int elementCol = m_Chains[NewLink].ChainMatrix.col_idx[elementIdx];
			int elementRow = m_Chains[NewLink].ChainMatrix.row_idx[elementIdx];

			if (elementRow == NodeTo	&&
				elementRow == elementCol)
			{
				m_Chains[NewLink].ChainMatrix.val[elementIdx] -= NewLinkAdmittance;
				m_Chains[NewLink].ChainMatrix.AddElementInto(NewLinkAdmittance, NodeTo, NodeFrom);
				break;
			}
		}
		m_RandomNetAdmittancesLeft--;
	}
	// 3. Случайно раскидаем оставшиеся связи (добавка не проверяет, были ли там уже связи)
	while (m_RandomNetAdmittancesLeft > 0)
	{
		int ChainFrom = RandomInt(0, m_Chains.size());
		int ChainTo = RandomInt(0, m_Chains.size());

		Chain* ChainFromPtr = &(m_Chains[ChainFrom]);
		Chain* ChainToPtr = &(m_Chains[ChainTo]);
		int NodeFrom = RandomInt(ChainFromPtr->StartNode, ChainFromPtr->StartNode + ChainFromPtr->NodesNumber - 1);
		int NodeTo = RandomInt(ChainToPtr->StartNode, ChainToPtr->StartNode + ChainToPtr->NodesNumber - 1);

		double NewLinkAdmittance = GetRandomAdmittance();

		for (int elementIdx = 0; elementIdx < ChainFromPtr->ChainMatrix.row_idx.size(); elementIdx++)
		{
			int elementCol = ChainFromPtr->ChainMatrix.col_idx[elementIdx];
			int elementRow = ChainFromPtr->ChainMatrix.row_idx[elementIdx];

			if (elementRow == NodeFrom	&&
				elementRow == elementCol)
			{
				ChainFromPtr->ChainMatrix.val[elementIdx] -= NewLinkAdmittance;
				ChainFromPtr->ChainMatrix.AddElementInto(NewLinkAdmittance, NodeFrom, NodeTo);
				break;
			}
		}
		for (int elementIdx = 0; elementIdx < ChainToPtr->ChainMatrix.row_idx.size(); elementIdx++)
		{
			int elementCol = ChainToPtr->ChainMatrix.col_idx[elementIdx];
			int elementRow = ChainToPtr->ChainMatrix.row_idx[elementIdx];

			if (elementRow == NodeTo	&&
				elementRow == elementCol)
			{
				ChainToPtr->ChainMatrix.val[elementIdx] -= NewLinkAdmittance;
				ChainToPtr->ChainMatrix.AddElementInto(NewLinkAdmittance, NodeTo, NodeFrom);
				break;
			}
		}
		m_RandomNetAdmittancesLeft--;
	}
}

double CMatrixGen::GetRandomAdmittance()
{
	return RandomDouble(m_BaseAdmittance, m_AdmittancesDispersion);
}

double CMatrixGen::GetRandomAddition()
{
	return RandomDouble(m_BaseMainDiagonalAddition, m_AdditionDispersion);
}

double CMatrixGen::RandomDouble(double Base, double Dispersion)
{
	double x = ((double)rand() / (double)(RAND_MAX)) - 0.5;			// Случайное число [-0.5,0.5]
	double delta = Base * Dispersion;
	return (Base + (delta + delta) * x);
}

int CMatrixGen::RandomInt(int From, int To)
{
	if (To < From)
	{
		int temp = To;
		To = From;
		From = temp;
	}
	if (To == From)
		return To;

	double Dispersion = To - From;
	Dispersion *= ((double)rand() / (double)(RAND_MAX));
	int Random = From + (int)(Dispersion);
}

vector<int> CMatrixGen::RandomVectorInsert(int iVectorSize, int iElementsNumber)
{
	vector<int> oRes;
	vector<int> Vec(iVectorSize);
	iota(Vec.begin(), Vec.end(), 0);
	int ElementsLeft = iElementsNumber;
	while (ElementsLeft > 0)
	{
		int SetElement = RandomInt(0, Vec.size());
		oRes.push_back(Vec[SetElement]);
		Vec.erase(Vec.begin() + SetElement);
		ElementsLeft--;
	}
	
	return oRes;
}
