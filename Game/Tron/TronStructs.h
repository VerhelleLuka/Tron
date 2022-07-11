#pragma once
namespace dae
{
	enum class GameMode
	{
		SINGLE,
		COOP,
		VERSUS
	};
	struct Block
	{
		int row, column;
		Block(int roww, int columnn)
		{
			row = roww;
			column = columnn;
		}
	};
}