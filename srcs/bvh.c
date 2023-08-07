#include "../includes/minirt.h"

void	bounding_sphere(t_sphere *sp)
{
	t_aabb aabb;
	t_vec3	tmp;

	tmp = vec3(sp->radius, sp->radius, sp->radius);
	aabb.lowerbound = sub_vector(sp->center, tmp);
	aabb.upperbound = add_vector(sp->center, tmp);
	sp->box = aabb;
}

t_aabb box_union(t_aabb a, t_aabb b)
{
	t_aabb c;

	c.lowerbound.x = fmin(a.lowerbound.x, b.lowerbound.x);
	c.lowerbound.y = fmin(a.lowerbound.y, b.lowerbound.y);
	c.lowerbound.z = fmin(a.lowerbound.z, b.lowerbound.z);
	c.upperbound.x = fmax(a.upperbound.x, b.upperbound.x);
	c.upperbound.y = fmax(a.upperbound.y, b.upperbound.y);
	c.upperbound.z = fmax(a.upperbound.z, b.upperbound.z);
	return (c);
}


double calculate_t(double origin, double direction, double min, double max, double* t_min, double* t_max)
{
    double invd;
	double t0;
	double t1;
    double temp;

	invd = 1.0 / direction;
	t0 = (min - origin) * invd;
	t1 = (max - origin) * invd;
    if (invd < 0.0)
	{
		temp = t0;
		t0 = t1;
		t1 = temp;
	}
	if (t0 > *t_min)
		*t_min = t0;
	if (t1 < *t_max)
		*t_max = t1;
    if (*t_max <= *t_min)
		return (0);
	return (1);
}

int hit(t_ray3 ray, double t_min, double t_max, t_aabb box)
{
	if (!calculate_t(ray.origin.x, ray.dir.x, box.lowerbound.x, box.upperbound.x, &t_min, &t_max))
		return (0);
    if (!calculate_t(ray.origin.y, ray.dir.y, box.lowerbound.y, box.upperbound.y, &t_min, &t_max))
		return (0);
    if (!calculate_t(ray.origin.z, ray.dir.z, box.lowerbound.z, box.upperbound.z, &t_min, &t_max))
		return (0);
	return (1);
}
