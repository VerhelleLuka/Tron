#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "../Tron/TronStructs.h"
#include "Structs.h"
namespace dae
{
	//Todo >> Replace with your own structure



	//Just parses vertices and indices
	static bool ParseLevelFile(const std::string& filename, std::vector<Block>& blocks, std::vector<Float2> spawnPositions)
	{
		std::ifstream file(filename);
		if (!file)
			return false;

		blocks.clear();
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

			else if (sCommand == "b")
			{
				int row, column;
				file >> row >> column;

				blocks.push_back(Block(row, column));
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