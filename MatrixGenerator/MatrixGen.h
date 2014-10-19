#pragma once
#include <vector>
using namespace std;

// Вспомогательная структура CSR матрицы
struct MatrixCSR
{
	vector<double>	val;
	vector<int>		col_idx;
	vector<int>		row_ptr;
};

struct MatrixCOO
{
	vector<double>	val;
	vector<int>		col_idx;
	vector<int>		row_idx;
	void AddElementInto(double ival, int irow_idx, int icol_idx)
	{
		int rowStartIdx = -1;
		for (int elementIdx = 0; elementIdx < row_idx.size(); elementIdx++)
		{
			if (irow_idx == row_idx[elementIdx])
			{
				rowStartIdx = elementIdx;
				break;
			}
		}
		if (rowStartIdx == -1)
			return;
		int rowEndIdx = -1;
		for (int elementIdx = rowStartIdx; elementIdx < row_idx.size(); elementIdx++)
		{
			if (irow_idx != row_idx[elementIdx])
			{
				rowEndIdx = elementIdx;
				break;
			}
		}
		if (rowEndIdx == -1)
			rowEndIdx = row_idx.size();
		if (icol_idx < col_idx[rowStartIdx])
		{
			val.insert(val.begin() + rowStartIdx, ival);
			col_idx.insert(col_idx.begin() + rowStartIdx, icol_idx);
			row_idx.insert(row_idx.begin() + rowStartIdx, irow_idx);
			return;
		}
		if (icol_idx > col_idx[rowEndIdx - 1])
		{
			val.insert(val.begin() + rowEndIdx, ival);
			col_idx.insert(col_idx.begin() + rowEndIdx, icol_idx);
			row_idx.insert(row_idx.begin() + rowEndIdx, irow_idx);
			return;
		}
		for (int elementIdx = rowStartIdx; elementIdx < rowEndIdx - 1; elementIdx++)
		{
			if (icol_idx > col_idx[elementIdx]	&&
				icol_idx < col_idx[elementIdx + 1])
			{
				val.insert(val.begin() + elementIdx + 1, ival);
				col_idx.insert(col_idx.begin() + elementIdx + 1, icol_idx);
				row_idx.insert(row_idx.begin() + elementIdx + 1, irow_idx);
				return;
			}
		}
//		val.push_back(ival);
//		col_idx.push_back(icol_idx);
//		row_idx.push_back(irow_idx);
	}
	void AddElement(double ival, int irow_idx, int icol_idx)
	{
		val.push_back(ival);
		col_idx.push_back(icol_idx);
		row_idx.push_back(irow_idx);
	}
};

// Генератор матриц с заднным количеством цепей и заданным количеством случайных связей между цепями
class CMatrixGen
{
//==[Nested]================================================================================================
private:
	struct Chain
	{
		int			StartNode;
		int			ChainNumber;
		MatrixCOO	ChainMatrix;
		int			NodesNumber;
		MatrixCOO	UnchainedLinks;
	};
//==[Переменные]============================================================================================

public:
	vector<int>			m_ChainNodesNumber;				// Сколько узлов будет в каждой цепочке
														// Размерность этого вектора - количество цепочек
														// Каждый элемент этого вектора >=2
	int					m_RandomNetAdmittancesNumber;	// Количество случайных связей
	double				m_BaseAdmittance;				// Базовая проводимость
	double				m_AdmittancesDispersion;		// Дисперсия проводимостей относительно базовой (относительно базовой)
	double				m_BaseMainDiagonalAddition;		// Базовая добавка к главной диагонали (проводимости шунта)
	double				m_AdditionDispersion;			// Дисперсия добавок к главной диагонали (относительно базовой)
	double				m_AdditionsNumber;				// Количество узлов с добавкой к главной диагонали (относительно числа узлов)
	double				m_EDSBase;						// Базовая ЭДС
	double				m_EDSDispersion;				// Дисперсия ЭЛС
	double				m_EDSNumber;					// Число узлов с ЭДС (относительно числа узлов)
	double				m_EDSAdmittanceBase;			// Базовая проводимость ЭДС
	double				m_EDSAdmittanceDispersion;		// Дисперсия проводимостей ЭДС

private:
	MatrixCOO			m_LastMatrix;
	vector<double>		m_LastVector;
	vector<Chain>		m_Chains;
	int					m_RandomNetAdmittancesLeft;
	int					m_NodesNumber;

//==[Методы]================================================================================================

public:
						CMatrixGen();					// Конструктор, пустой
						~CMatrixGen();					// Деструктор (там будем все удалять)

public:
	MatrixCOO			GetMatrix();
	MatrixCOO			GetLastMatrix();

private:
	int					CreateCSR();
	int					GetNodesAmount();
	void				CreateChains();

	double				GetRandomAdmittance();
	double				GetRandomAddition();

private:
	static double		RandomDouble(double, double);
	static int			RandomInt(int, int);
	static vector<int>	RandomVectorInsert(int, int);
};

