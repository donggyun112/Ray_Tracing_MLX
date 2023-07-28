#include "../includes/minirt.h"

t_bool	hit_sphere(t_ray3 *ray, t_sphere *sp)
{
	t_vec3	l;
	double	tca;
	double	tnc;
	double	d2;
	double	tmp;


	// p = p0 + tV
	l = sub_vector(sp->sphere, ray->origin); // 원점과 구 중심을 잇는 방향벡터
	tca = scalar_product(l, ray->dir); // 빛이 구를 향해가고 있는지를 판단
	if (tca < 0)
		return (FALSE);
	d2 = scalar_product(l, l) - (tca * tca); // 원점에서 벡터 사이의 거리가 r^2보다 크면 FALSE d = d^2
	if (d2 > sp->r * sp->r)
		return (FALSE);
	tnc = sqrt(sp->r * sp->r - d2);
	if (tca - tnc < 0.0)
		tmp = tca + tnc;
	else
		tmp = tca - tnc;
	if (ray->t > tmp)
	{
		ray->t = tmp;
		ray->type = SP;
		ray->obj = (void *)sp;
	}
	return (TRUE);
}
