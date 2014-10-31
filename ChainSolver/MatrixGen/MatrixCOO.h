#ifndef RANDOMCHAINMATRIXGENERATOR_MATRIXCOO_H
#define RANDOMCHAINMATRIXGENERATOR_MATRIXCOO_H

#pragma once
#include <vector>
#include <string>
using namespace std;

namespace RandomChainMatrixGenerator
{
	struct TriDiagonal
	{
		vector<double> a;				// Верхняя диагональ
		vector<double> b;				// Главная диагональ
		vector<double> c;				// Нижняя диагональ
	};
	// Вспомогательная структура COO матрицы
	struct MatrixCOO
	{
		vector<double>	val;								// Вектор значений матрицы
		vector<int>		row_idx;							// Вектор индексов строк матрицы (первый уровень сортировки должен быть выполнен)
		vector<int>		col_idx;							// Вектор индексов столбцов матрицы (второй уровень сортировки должен быть выполнен)
		void			ClearMatrix();						// Удалить значения всех векторов
		int				GetRows();							// Число рядов матрицы
		int				GetCols();							// Число столбцов матрицы
		int				AddElementInto(double, int, int);	// Добавить элемент с учетом сортировки
		void			AddElement(double, int, int);		// Добавить элемент без учета сортировки
		void			WriteMatrixMarketFile(const string);// Записать матрицу в файл согласно формату matrix market
															// Подробная информация по формату на сайте:
															// http://math.nist.gov/MatrixMarket/formats.html
	};
}

#endif // RANDOMCHAINMATRIXGENERATOR_MATRIXCOO_H