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
		vector<double> a;				// ������� ���������
		vector<double> b;				// ������� ���������
		vector<double> c;				// ������ ���������
	};
	// ��������������� ��������� COO �������
	struct MatrixCOO
	{
		vector<double>	val;								// ������ �������� �������
		vector<int>		row_idx;							// ������ �������� ����� ������� (������ ������� ���������� ������ ���� ��������)
		vector<int>		col_idx;							// ������ �������� �������� ������� (������ ������� ���������� ������ ���� ��������)
		void			ClearMatrix();						// ������� �������� ���� ��������
		int				GetRows();							// ����� ����� �������
		int				GetCols();							// ����� �������� �������
		int				AddElementInto(double, int, int);	// �������� ������� � ������ ����������
		void			AddElement(double, int, int);		// �������� ������� ��� ����� ����������
		void			WriteMatrixMarketFile(const string);// �������� ������� � ���� �������� ������� matrix market
															// ��������� ���������� �� ������� �� �����:
															// http://math.nist.gov/MatrixMarket/formats.html
	};
}

#endif // RANDOMCHAINMATRIXGENERATOR_MATRIXCOO_H