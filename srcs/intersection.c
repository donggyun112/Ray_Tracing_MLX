#include "../includes/minirt.h"
#include <stdio.h> //remove

typedef struct {
    Color color_a;
    Color color_b;
    int width;
    int height;
} CheckerPattern;

Color checkerTexture(t_vec3 point, float scale) {
    // 체커무늬 패턴의 크기를 조절하기 위해 scale을 사용
    float s = scale;
	if (point.z < 0)
		point.z -= 1;
    int checkerX = (int)(point.x * s) % 2;
    int checkerY = (int)(point.y * s) % 2;
    int checkerZ = (int)(point.z * s) % 2;

    // 3D 체커무늬 패턴을 생성

    if ((checkerX + checkerY + checkerZ) % 2 == 0) {
        return (Color){0, 0, 0};
    } else {
        return (Color){255, 255, 255};
    }
}

void spherical_map(t_vec3 p, float* u, float* v, t_vec3 center)
{
    t_vec3 relative_point = {p.x - center.x, p.y - center.y, p.z - center.z};
    
    float theta = atan2f(relative_point.z, relative_point.x);
    float vecMagnitude = sqrtf(relative_point.x * relative_point.x + relative_point.y * relative_point.y + relative_point.z * relative_point.z);
    float phi = acosf(relative_point.y / vecMagnitude);

    float raw_u = theta / (2.0f * M_PI);
    *u = (raw_u + 0.5f);
    *v = 1.0f - phi / M_PI;
}

Color uv_grid_pattern_at(CheckerPattern pattern, float u, float v)
{
    const int u2 = (int)(u * pattern.width);
    const int v2 = (int)(v * pattern.height);

    if ((u2 + v2) % 2)
        return (pattern.color_a);
    else
		return (pattern.color_b);
}

Color gridTextureOnSphere(t_vec3 point, CheckerPattern pattern, t_vec3 center)
{
	float u;
	float v;

	spherical_map(point, &u, &v, center);
	return (uv_grid_pattern_at(pattern, u, v));
}

int	hit_line_sphere(t_ray3 *ray, t_sphere *sp)
{
	t_vec3	l;
	double	tca;
	double	d2;
	double	tnc;


	// p = p0 + tV
	l = sub_vector(sp->center, ray->origin); // 원점과 구 중심을 잇는 방향벡터
	tca = scalar_product(l, ray->dir); // 빛이 구를 향해가고 있는지를 판단
	if (tca < 0)
		return (0);
	d2 = scalar_product(l, l) - (tca * tca); // 원점에서 벡터 사이의 거리가 r^2보다 크면 FALSE d = d^2
	if (d2 > sp->radius * sp->radius)
		return (0);
	tnc = sqrt(sp->radius * sp->radius - d2);;
	if (tca - tnc < 0.0)
		return (1);
	else
		return (1);
	return (1);
}


int	intersect_sphere_shadow(t_ray3 *ray, t_canvas canvas)
{
	t_vec3	p;
	t_vec3	g_norm;
	t_ray3	light;
	int		idx[2];

	idx[0] = -1;
	p = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	// printf("x:%f y:%f z:%f\n", canvas.obj->l->light_orig.x, canvas.obj->l->light_orig.y, canvas.obj->l->light_orig.z);
	while (++idx[0] < canvas.obj->l_cnt)
	{
		g_norm = norm_vec(sub_vector(canvas.obj->l[idx[0]].light_orig, p));
		light.dir = g_norm;
		light.origin = p;
		idx[1] = -1;
		while (++idx[1] < canvas.obj->sp_cnt)
		{
			if (hit_line_sphere(&light, &canvas.obj->sp[idx[1]]))
				return (1);
		}
	}
	return (0);
}

void	hit_sphere(t_ray3 *ray, t_sphere *sp, t_canvas canvas)
{
	t_vec3	l;
	double	tca;
	double	tnc;
	double	d2;
	double	tmp;
    CheckerPattern pattern = {{255, 255, 255}, {100, 100, 0}, 32, 16};



	l = sub_vector(sp->center, ray->origin); // 원점과 구 중심을 잇는 방향벡터
	tca = scalar_product(l, ray->dir); // 빛이 구를 향해가고 있는지를 판단
	if (tca < 0)
		return ;
	d2 = scalar_product(l, l) - (tca * tca); // 원점에서 벡터 사이의 거리가 r^2보다 크면 FALSE d = d^2
	if (d2 > sp->radius * sp->radius)
		return ;
	tnc = sqrt(sp->radius * sp->radius - d2);
	if (tca - tnc < 0.0)
		tmp = tca + tnc;
	else
		tmp = tca - tnc;
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		ray->t = tmp;
		ray->color[RED] = sp->color[RED];
		ray->color[GREEN] = sp->color[GREEN];
		ray->color[BLUE] = sp->color[BLUE];
		ray->type = SP;
		Color c = gridTextureOnSphere(add_vector(ray->origin, multiple_vector(ray->t, ray->dir)), pattern, sp->center);
		ray->color[RED] = c.r;
		ray->color[GREEN] = c.g;
		ray->color[BLUE] = c.b;
		ray->obj = (void *)sp;
		if (intersect_sphere_shadow(ray, canvas))
			ray->type = SHADOW;
	}
}

double distance_a(t_vec3 a, t_vec3 b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double dz = a.z - b.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}


// double	intersect_sphere_shadow(t_ray3 *ray, t_canvas canvas, int num_of_light)
// {
// 	t_vec3	p;
// 	t_vec3	g_norm;
// 	t_vec3	sh;
// 	t_ray3	light;
// 	int		idx[2];
// 	double	count;

// 	idx[0] = -1;
// 	count = 0;
// 	p = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
// 	sh = p;
// 	while (++idx[0] < num_of_light)
// 	{
// 		idx[1] = -1;
// 		p.x += my_rand_double_range(0.0, 0.2);
// 		p.y += my_rand_double_range(0.0, 0.2);
// 		p.z += my_rand_double_range(0.0, 0.2);
// 		g_norm = norm_vec(sub_vector(canvas.light_orig, p));
// 		light.dir = g_norm;
// 		light.origin = p;
// 		while (++idx[1] < canvas.obj->sp_cnt)
// 		{
// 			if (hit_line_sphere(&light, &canvas.obj->sp[idx[1]]))
// 			{
// 				count++;
// 				break ;
// 			}
// 		}
// 		p = sh;
// 	}
// 	if (count == 0)
// 		return (0.0);
// 	return ((double)count / num_of_light);
// }


double mapToRange(double value, double minInput, double maxInput, double minOutput, double maxOutput)
{
    // 로그 함수를 사용하여 입력값을 [0, ∞) 범위로 매핑
    double logValue = log(value - minInput + 1);

    // [0, ∞) 범위의 값을 [minOutput, maxOutput] 범위로 선형 변환
    return (((logValue) / (log(maxInput - minInput + 1))) * (maxOutput - minOutput) + minOutput);
}




void	hit_plane(t_ray3 *ray, t_plane *pl, t_canvas canvas)
{
	double	tmp;
	double	scalar[3];
	// double	ll;
	// double	dist;

	(void)canvas;
	scalar[0] = scalar_product(pl->on_plane, pl->norm);
	scalar[1] = scalar_product(ray->origin, pl->norm);
	scalar[2] = scalar_product(ray->dir, pl->norm);
	tmp = (scalar[0] - scalar[1]) / scalar[2];
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		ray->t = tmp;
		ray->type = PL;
		ray->color[RED] = pl->color[RED];
		ray->color[GREEN] = pl->color[GREEN];
		ray->color[BLUE] = pl->color[BLUE];
		ray->obj = (void *)pl;

		Color c = checkerTexture(add_vector(ray->origin, multiple_vector(ray->t, ray->dir)), 1);
		ray->color[RED] = c.r;
		ray->color[GREEN] = c.g;
		ray->color[BLUE] = c.b;
		if (intersect_sphere_shadow(ray, canvas))
			ray->type = SHADOW;
		// ll = intersect_sphere_shadow(ray, canvas, 10); // 그림자 개수 --> 안티엘리어싱
		// if (ll)
		// {
		// 	ray->type = SHADOW;
		// 	dist = distance_a(add_vector(ray->origin, multiple_vector(ray->t, ray->dir)), canvas.light_orig);
		// 	dist = mapToRange(dist, 0, 10, 0.5, 1.0);
		// 	ll = mapToRange(ll, 0.0, 1.0, 1.0, 0.5);
		// 	ray->color[RED] *= dist * ll;
		// 	ray->color[GREEN] *= dist * ll;
		// 	ray->color[BLUE] *= dist * ll;
		// }
	}
}

