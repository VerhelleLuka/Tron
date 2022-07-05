#include "MiniginPCH.h"
#include "TrashTheCache.h"
#include <chrono>
#include <time.h>
std::vector<float> dae::TrashTheCache::IntegerCache() const
{
	std::vector<float> timeStamps;
	const int arraySize = 100000;
	int* arr = new int[arraySize];
	for (size_t i = 0; i < arraySize; i++)
	{
		arr[i] = 10;
	}
	int step = 1;

	while (step <= 1024)
	{
		const auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < arraySize; i += step)
		{
			arr[i] *= 2;
		}
		const auto end = std::chrono::high_resolution_clock::now();
		float timeStamp = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		//std::cout << step << "; " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "\n";

		timeStamps.push_back(timeStamp);
		step *= 2;
	}
	return timeStamps;
}

std::vector<float> dae::TrashTheCache::GameObject3DTime()
{
	std::vector<float> timeStamps;
	const int arraySize = 100000;
	GameObject3D* arr = new GameObject3D[arraySize];
	for (size_t i = 0; i < arraySize; i++)
	{
		arr[i].ID = i;
	}
	int step = 1;

	while (step <= 1024)
	{
		const auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < arraySize; i += step)
		{
			arr[i].ID *= 2;
		}
		const auto end = std::chrono::high_resolution_clock::now();
		float timeStamp = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		//std::cout << step << "; " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "\n";

		timeStamps.push_back(timeStamp);
		step *= 2;
	}
	return timeStamps;
}

std::vector<float> dae::TrashTheCache::GameObject3DAltTime()
{
	std::vector<float> timeStamps;
	const int arraySize = 100000;
	GameObject3DAlt* arrAlt = new GameObject3DAlt[arraySize];
	for (size_t i = 0; i < arraySize; i++)
	{
		arrAlt[i].ID = i;
	}
	int step = 1;

	while (step <= 1024)
	{
		const auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < arraySize; i += step)
		{
			arrAlt[i].ID *= 2;
		}
		const auto end = std::chrono::high_resolution_clock::now();
		float timeStamp = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		//std::cout << step << "; " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "\n";

		timeStamps.push_back(timeStamp);
		step *= 2;
	}
	return timeStamps;

}