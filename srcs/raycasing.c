#include "includes/minirt.h"
#include "includes/vector.h"
#include "includes/struct.h"

t_ray3	create_ray(t_point3 camera, int i, int j)
{
	t_ray3	ray;
	double	width;
	double	height;

	width = 3;
	height = 2;
	//i range [0, 1500], j range [0, 1000]
	//left_up vector 에서 출발해서 width * i / 가로픽셀(1500)
	//height * j / 세로픽셀(1000) 을 더해주면 카메라의 기준에서
	//viewport로 가는 벡터를 구할 수 있다.
}

t_vec3	cammera(t_point3 origin, t_vec3 dir)
{
	t_camera	cam;
	t_vec3		up;
	t_vec3		right;

	cam.dir = init_vec(3.0, 4.0, 5.0);
	up = init_vec(0.0, 1.0, 0.0);
	cam.r_norm = vector_product(cam.dir, up);
	cam.v_norm = vector_product(cam.dir, cam.r_norm);
	//r_norm과 v_norm에 width, height 각각 절반씩 곱해서 dir벡터에 더해줌
}