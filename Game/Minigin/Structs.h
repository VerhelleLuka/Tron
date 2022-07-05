#pragma once

namespace dae
{
	struct Float4
	{
		float x{}, y{}, z{}, w{};
	};
	struct Float2
	{
		float x{}, y{};

		Float2 operator+=(Float2& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;

			return *this;
		}
		Float2(float xx, float yy)
		{
			x = xx;
			y = yy;
		}
	};
}