#pragma once
#include <vector>
struct Transform_
{
	float matrix[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
};

class GameObject3D
{
public:
	Transform_ transform;
	size_t ID = 10;
};

class GameObject3DAlt
{
public:
	size_t ID = 10;
	Transform_* transform;
};
namespace dae
{
	class TrashTheCache
	{
	public:
		std::vector<float> IntegerCache() const;
		std::vector<float> GameObject3DTime();
		std::vector<float> GameObject3DAltTime();
	};
}
