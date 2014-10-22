#include "Chain.h"
using namespace RandomChainMatrixGenerator;

void
Chain::CopyChainMatrixToMainMatrix()
{
	MainMatrixCOO.ClearMatrix();
	Links.clear();
	MainMatrixCOO = ChainMatrix;
}

void
Chain::AddEDS(double Admittance, double EDSValue, int EDSRow)
{
	if (!CheckRow(EDSRow))
		return;
	// Добавим ЭДС в общую матрицу
	for (int elementIdx = 0; elementIdx < MainMatrixCOO.val.size(); elementIdx++)
	{
		if (MainMatrixCOO.row_idx[elementIdx] == MainMatrixCOO.col_idx[elementIdx] &&
			MainMatrixCOO.row_idx[elementIdx] == EDSRow)
		{
			MainMatrixCOO.val[elementIdx] -= Admittance;
			int insideRow = EDSRow - StartNode;
			RightVector[insideRow] -= EDSValue * Admittance;
			break;
		}
	}
	// Добавим ЭДС в цепочечную матрицу
	for (int elementIdx = 0; elementIdx < ChainMatrix.val.size(); elementIdx++)
	{
		if (ChainMatrix.row_idx[elementIdx] == ChainMatrix.col_idx[elementIdx] &&
			ChainMatrix.row_idx[elementIdx] == EDSRow)
		{
			ChainMatrix.val[elementIdx] -= Admittance;
			break;
		}
	}
}

void
Chain::AddShunt(double Val, int shuntRow)
{
	if (!CheckRow(shuntRow))
		return;
	// Добавим шунт в общую матрицу
	for (int elementIdx = 0; elementIdx < MainMatrixCOO.val.size(); elementIdx++)
	{
		if (MainMatrixCOO.row_idx[elementIdx] == MainMatrixCOO.col_idx[elementIdx] &&
			MainMatrixCOO.row_idx[elementIdx] == shuntRow)
		{
			MainMatrixCOO.val[elementIdx] -= Val;
			break;
		}
	}
	// Добавим шунт в цепочечную матрицу
	for (int elementIdx = 0; elementIdx < ChainMatrix.val.size(); elementIdx++)
	{
		if (ChainMatrix.row_idx[elementIdx] == ChainMatrix.col_idx[elementIdx] &&
			ChainMatrix.row_idx[elementIdx] == shuntRow)
		{
			ChainMatrix.val[elementIdx] -= Val;
			break;
		}
	}
}

int	
Chain::AddLink(double Val, int linkRow, int linkCol)
{
	if (!CheckRow(linkRow))
		return -1;

	int out = -1;
	int wasAdded = 0;
	// Добавим связь в общую матрицу
	for (int elementIdx = 0; elementIdx < MainMatrixCOO.val.size(); elementIdx++)
	{
		if (MainMatrixCOO.row_idx[elementIdx] == MainMatrixCOO.col_idx[elementIdx] &&
			MainMatrixCOO.row_idx[elementIdx] == linkRow)
		{
			MainMatrixCOO.val[elementIdx] -= Val;
			int added = MainMatrixCOO.AddElementInto(Val, linkRow, linkCol);
			if (added == 0)
				wasAdded++;
			break;
		}
	}
	// Добавим связь в цепочечную матрицу
	bool added = false;
	for (int elementIdx = 0; elementIdx < ChainMatrix.val.size(); elementIdx++)
	{
		if (ChainMatrix.row_idx[elementIdx] == ChainMatrix.col_idx[elementIdx] &&
			ChainMatrix.row_idx[elementIdx] == linkRow)
		{
			ChainMatrix.val[elementIdx] -= Val;
			added = true;
			break;
		}
	}
	if (wasAdded == 2 && added)
		out = 0;
	else
		out = 1;

	// Добавим вектор связи
	int NonZeroRow = linkRow - StartNode;
	Link NewLink;
	vector<double> NewLinkVector(NodesNumber);
	NewLinkVector[NonZeroRow] = Val;
	NewLink.chain_vector = NewLinkVector;
	NewLink.col_idx = linkCol;
	Links.push_back(NewLink);
	return out;
}

bool
Chain::CheckRow(int NewElementRow)
{
	int EndNode = StartNode + NodesNumber - 1;
	if (NewElementRow < StartNode ||
		NewElementRow > EndNode)
		return false;
	else
		return true;
}