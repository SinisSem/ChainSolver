#pragma once
#include <vector>
using namespace std;

// ��������������� ��������� CSR �������
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

// ��������� ������ � ������� ����������� ����� � �������� ����������� ��������� ������ ����� ������
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
//==[����������]============================================================================================

public:
	vector<int>			m_ChainNodesNumber;				// ������� ����� ����� � ������ �������
														// ����������� ����� ������� - ���������� �������
														// ������ ������� ����� ������� >=2
	int					m_RandomNetAdmittancesNumber;	// ���������� ��������� ������
	double				m_BaseAdmittance;				// ������� ������������
	double				m_AdmittancesDispersion;		// ��������� ������������� ������������ ������� (������������ �������)
	double				m_BaseMainDiagonalAddition;		// ������� ������� � ������� ��������� (������������ �����)
	double				m_AdditionDispersion;			// ��������� ������� � ������� ��������� (������������ �������)
	double				m_AdditionsNumber;				// ���������� ����� � �������� � ������� ��������� (������������ ����� �����)
	double				m_EDSBase;						// ������� ���
	double				m_EDSDispersion;				// ��������� ���
	double				m_EDSNumber;					// ����� ����� � ��� (������������ ����� �����)
	double				m_EDSAdmittanceBase;			// ������� ������������ ���
	double				m_EDSAdmittanceDispersion;		// ��������� ������������� ���

private:
	MatrixCOO			m_LastMatrix;
	vector<double>		m_LastVector;
	vector<Chain>		m_Chains;
	int					m_RandomNetAdmittancesLeft;
	int					m_NodesNumber;

//==[������]================================================================================================

public:
						CMatrixGen();					// �����������, ������
						~CMatrixGen();					// ���������� (��� ����� ��� �������)

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

