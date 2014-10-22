#pragma once
#include <vector>
#include <string>
#include "Chain.h"
using namespace std;

// http://www.jcohen.name/papers/Zhang_Fast_2009.pdf
// http://conferences.computer.org/sc/2012/papers/1000a071.pdf
// ��������� ������ � ������� ����������� ����� � �������� ����������� ��������� ������ ����� ������
class CMatrixGen
{
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
	int					m_RandomNetAdmittancesLeft;
	int					m_NodesNumber;
	vector<Chain>		m_Chains;						// ������ �������

//==[������]================================================================================================

public:
						CMatrixGen();					// �����������, ��� ������ ������� ������ ���������� ��������� �����
						~CMatrixGen();					// ���������� (��� ����� ��� �������)

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