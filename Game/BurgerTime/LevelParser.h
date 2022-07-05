#pragma once
#include <string>
#include <fstream>
#include "BurgerTimeStructs.h"
#include <vector>
namespace dae
{
	//Todo >> Replace with your own structure



	//Just parses vertices and indices
	static bool ParseLevelFile(const std::string& filename, std::vector<Platform>& platforms, std::vector<Ladder>& ladders, std::vector<Float2> spawnPositions, std::vector<Burger>& burgers)
	{
		std::ifstream file(filename);
		if (!file)
			return false;

		platforms.clear();
		ladders.clear();
		spawnPositions.clear();

		std::string sCommand;
		// start a while iteration ending when the end of file is reached (ios::eof)
		while (!file.eof())
		{
			//read the first word of the string, use the >> operator (istream::operator>>) 
			file >> sCommand;
			//use conditional statements to process the different commands	
			if (sCommand == "#")
			{
				// Ignore Comment
			}
			else if (sCommand == "p")
			{
				
				int row, column;
				file >> row >> column;
				
				platforms.push_back(Platform(row, column));
			}
			else if (sCommand == "ld")
			{
				int row, column;
				file >> row >> column;

				ladders.push_back(Ladder(row,column));
			}
			else if (sCommand == "b")
			{
				int row, column;
				std::string burgerPart;
				file >> row >> column >>burgerPart;

				burgers.push_back(Burger(row, column, burgerPart));
			}
			else if (sCommand == "ps")
			{
				// Vertex Normal
				float x, y;
				file >> x >> y;
				spawnPositions.push_back(Float2(x, y));
			}
		}

		return true;
	}
}