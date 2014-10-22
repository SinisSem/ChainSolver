#pragma once
#include <vector>
#include <string>
#include "MatrixCOO.h"
using namespace std;
namespace RandomChainMatrixGenerator
{

	// ��������� ����� ������������ ����� � �������
	struct Link
	{
		int				col_idx;						// ����� ������� ����� � ������� �������
		vector<double>	chain_vector;					// ������ D, ������� ����� ����� ��� ������� �� GPU (��������� ��� ���)
	};

	// ��������� ������� (������, �������� ������� � �������, �� ������� ����� ����� �������� �������������� �������)
	struct Chain
	{
		int				StartNode;						// ������ ������� ���� � ������ ������� �����
		int				ChainNumber;					// ����� ���� � ������� �����
		MatrixCOO		MainMatrixCOO;					// �������� �������
		MatrixCOO		ChainMatrix;					// �������, ��� ���������� ������ �������� ����
		int				NodesNumber;					// ���������� ����� � ����
		vector<Link>	Links;							// ������� ��� ������������ ������
		vector<double>	RightVector;					// ������ ������
	private:
		bool			CheckRow(int NewElementRow);	// ��������, ����������� �� ���� � ������ ����

	public:
		void			CopyChainMatrixToMainMatrix();	// �������� ���������� ������� ������� � ������ �������
		void			AddEDS(double, double, int);	// ������� ��� � �������
		void			AddShunt(double, int);			// ������� ���� � �������
		int				AddLink(double, int, int);	// ������� ����� �������� �����
	};
}