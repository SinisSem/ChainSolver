#include "MatrixCOO.h"
#include <fstream>
using namespace RandomChainMatrixGenerator;

void
MatrixCOO::ClearMatrix()
{
	val.clear();
	col_idx.clear();
	row_idx.clear();
}

int
MatrixCOO::GetRows()
{
	int Rows = 0;
	for (int elementNumber = 0; elementNumber < row_idx.size(); elementNumber++)
	{
		if (row_idx[elementNumber] > Rows)
			Rows = row_idx[elementNumber];
	}
	Rows++;
	return Rows;
}

int
MatrixCOO::GetCols()
{
	int Cols = 0;
	for (int elementNumber = 0; elementNumber < col_idx.size(); elementNumber++)
	{
		if (col_idx[elementNumber] > Cols)
			Cols = row_idx[elementNumber];
	}
	Cols++;
	return Cols;
}

int 
MatrixCOO::AddElementInto(double ival, int irow_idx, int icol_idx)
{
	int out = 1;
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
		return out;
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
		out = 0;
		return out;
	}
	if (icol_idx > col_idx[rowEndIdx - 1])
	{
		val.insert(val.begin() + rowEndIdx, ival);
		col_idx.insert(col_idx.begin() + rowEndIdx, icol_idx);
		row_idx.insert(row_idx.begin() + rowEndIdx, irow_idx);
		out = 0;
		return out;
	}
	for (int elementIdx = rowStartIdx; elementIdx < rowEndIdx - 1; elementIdx++)
	{
		if (icol_idx > col_idx[elementIdx] &&
			icol_idx < col_idx[elementIdx + 1])
		{
			val.insert(val.begin() + elementIdx + 1, ival);
			col_idx.insert(col_idx.begin() + elementIdx + 1, icol_idx);
			row_idx.insert(row_idx.begin() + elementIdx + 1, irow_idx);
			out = 0;
			return out;
		}
	}
}
void 
MatrixCOO::AddElement(double ival, int irow_idx, int icol_idx)
{
	val.push_back(ival);
	col_idx.push_back(icol_idx);
	row_idx.push_back(irow_idx);
}

void 
MatrixCOO::WriteMatrixMarketFile(const string filename)
{
	std::ofstream output(filename.c_str());

	if (!output)
		return;

	output.scientific;
	output.precision(10);
	output << "%%MatrixMarket matrix coordinate real general\n";

	// Rows - Cols - Entires
	output << "\t" << GetRows() << "\t" << GetCols() << "\t" << val.size() << "\n";

	for (int i = 0; i < val.size(); i++)
	{
		output << (row_idx[i] + 1) << " ";
		output << (col_idx[i] + 1) << " ";
		output << (val[i]);
		output << "\n";
	}
}
