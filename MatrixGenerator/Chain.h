#pragma once
#include <vector>
#include <string>
#include "MatrixCOO.h"
using namespace std;
namespace RandomChainMatrixGenerator
{

	// Структура одной нецепочечной связи в цепочке
	struct Link
	{
		int				col_idx;						// Номер столбца связи в крупной матрице
		vector<double>	chain_vector;					// Вектор D, который будет нужен для расчета на GPU (посчитаем его тут)
	};

	// Структура цепочки (точнее, сегмента матрицы и вектора, из которых потом будет состоять результирующая матрица)
	struct Chain
	{
		int				StartNode;						// Индекс первого узла в полной матрице цепей
		int				ChainNumber;					// Номер цепи в массиве цепей
		MatrixCOO		MainMatrixCOO;					// Основная матрица
		MatrixCOO		ChainMatrix;					// Матрица, где существуют только элементы цепи
		int				NodesNumber;					// Количество узлов в цепи
		vector<Link>	Links;							// Векторы для нецепочечных связей
		vector<double>	RightVector;					// Правый вектор
	private:
		bool			CheckRow(int NewElementRow);	// Проверка, принадлежит ли узел к данной цепи

	public:
		void			CopyChainMatrixToMainMatrix();	// Копирует содержимое матрицы цепочек в полную матрицу
		void			AddEDS(double, double, int);	// Добавит ЭДС в цепочку
		void			AddShunt(double, int);			// Добавит шунт в цепочку
		int				AddLink(double, int, int);	// Добавит новую нецепную связь
	};
}