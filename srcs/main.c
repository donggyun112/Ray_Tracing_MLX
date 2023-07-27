#include "includes/struct.h"
#include "includes/minirt.h"
#include "includes/vector.h"

int main()
{
	t_ray3	ray;

	for (int j = 0; j < /* 세로 */; j++)
	{
		for (int i = 0; i < /* 가로 */; i++)
		{
			double u = double(i) / (/* 가로 - 1 */);
			double v = double(j) / (/* 세로 - 1 */);
			ray = create_ray(cam, u, v);
		}
	}
}