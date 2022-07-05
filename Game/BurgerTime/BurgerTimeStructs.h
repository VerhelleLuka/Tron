#pragma once
namespace dae
{
	enum class GameMode
	{
		SINGLE,
		COOP,
		VERSUS
	};
	struct Platform
	{
		int row, column;
		Platform(int roww, int columnn)
		{
			row = roww;
			column = columnn;
		}
	};
	struct Ladder
	{
		int row, column;
		Ladder(int roww, int columnn)
		{
			row = roww;
			column = columnn;
		}
	};
	struct Burger
	{
		int row, column;
		std::string partName;
		Burger(int roww, int columnn, std::string partname)
		{
			row = roww;
			column = columnn;
			partName = partname;
		}
	};
}
