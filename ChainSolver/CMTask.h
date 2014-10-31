#pragma once

struct CSRMatrix
{
	float*			val;
	int*			col_idx;
	int*			row_ptr;
};

struct Link
{
	int				col_idx;
	int				start_row;
	int				rows_num;
	float*			vector_d;
};

struct CMTask
{
public:
	CSRMatrix		Chain;
	float*			RightVector;//Пока что, просто вектор
	int				links_num;

};