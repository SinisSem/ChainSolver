#pragma once
#include <vector>
#include <string>
using namespace std;

namespace RandomChainMatrixGenerator
{
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