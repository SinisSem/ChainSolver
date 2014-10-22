#pragma once
#include <vector>
#include <string>
#include "Chain.h"
using namespace std;

// http://www.jcohen.name/papers/Zhang_Fast_2009.pdf
// http://conferences.computer.org/sc/2012/papers/1000a071.pdf
// Генератор матриц с заднным количеством цепей и заданным количеством случайных связей между цепями
class CMatrixGen
{
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
	int					m_RandomNetAdmittancesLeft;
	int					m_NodesNumber;
	vector<Chain>		m_Chains;						// Вектор цепочек

//==[Методы]================================================================================================

public:
						CMatrixGen();					// Конструктор, там просто вызовем движок генератора случайных чисел
						~CMatrixGen();					// Деструктор (там будем все удалять)

public:
	MatrixCOO			GetMatrix();
	MatrixCOO			GetLastMatrix();
	void				WriteMatrixMarketFile_LastMatrix(const string&);
	void				WriteMatrixMarketFile_LastVector(const string&);

private:
	int					CreateCOO();
	int					GetNodesAmount();
	void				CreateChains();

	double				GetRandomAdmittance();
	double				GetRandomAddition();

public:
	static double		RandomDouble(double, double);
	static int			RandomInt(int, int);
	static vector<int>	RandomVectorInsert(int, int);
	static vector<int>	RandomPairVectorInsert(int, vector<int>);
	static void			WriteMatrixMarkeFileVector(const string, vector<double>);

};