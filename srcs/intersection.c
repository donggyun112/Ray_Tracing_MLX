/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 20:29:45 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/29 20:49:01 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

typedef struct {
    Color color_a;
    Color color_b;
    int width;
    int height;
} CheckerPattern;

Color texture_at(t_vec3 point, t_texture texture)
{
    int x = (int)(point.z * texture.width) % texture.width;
    int y = (int)(point.x * texture.height) % texture.height;
    
    int index = (y * texture.width + x) * 4;
    int color = *(int *)(texture.data + index);
    
    Color c;
    c.r = (color >> 16) & 0xFF;
    c.g = (color >> 8) & 0xFF;
    c.b = color & 0xFF;
    return c;
}

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

Color get_texture_color(t_texture texture, float u, float v)
{
    int x = (int)(u * (float)texture.width) % texture.width;
    int y = (int)(v * (float)texture.height) % texture.height;
    int offset = (x + y * texture.width) * (texture.bpp / 8);
    
    int color = *(int *)(texture.data + offset);
    Color c;
	c.r = (color >> 16) & 0xFF;
	c.g = (color >> 8) & 0xFF;
	c.b = color & 0xFF;
    return c;
}

Color image_texture_on_sphere(t_vec3 point, t_vec3 center, t_texture *texture)
{
	float		u;
	float 		v;
	spherical_map(point, &u, &v, center);
	return (get_texture_color(*texture, u, v));
}

Color grid_texture_on_sphere(t_vec3 point, CheckerPattern pattern, t_vec3 center)
{
	float		u;
	float 		v;
	spherical_map(point, &u, &v, center);
	return (uv_grid_pattern_at(pattern, u, v));
}

void	init_texture(t_texture *texture, t_view *view, char *path)
{
	texture->img = mlx_xpm_file_to_image(view->mlx, path, &texture->width, &texture->height);
	if (!texture->img)
	{
		fprintf(stderr, "Failed to load texture: %s\n", path);
		exit(1);
    }
	texture->data = mlx_get_data_addr(texture->img, &texture->bpp, &texture->size_line, &texture->endian);
}

void	hit_sphere(t_ray3 *ray, t_sphere *sp, t_canvas canvas)
{
	t_vec3	l;
	double	tca;
	double	tnc;
	double	d2;
	double	tmp;
	Color c;
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
		if (sp->type == TSP || sp->type == CSP)
		{
			if (sp->type == TSP)
				c = image_texture_on_sphere(add_vector(ray->origin, multiple_vector(ray->t, ray->dir)), sp->center, &sp->texture);
			else
				c = grid_texture_on_sphere(add_vector(ray->origin, multiple_vector(ray->t, ray->dir)), pattern, sp->center);
			ray->color[RED] = c.r;
			ray->color[GREEN] = c.g;
			ray->color[BLUE] = c.b;
		}
		ray->obj = (void *)sp;
		if (hit_shadow(ray, canvas))
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


t_vec3	check_plane_direction(t_plane *pl, t_ray3 *ray)
{
	t_vec3	orig_to_pl;

	orig_to_pl = sub_vector(ray->origin, pl->on_plane);
	if (scalar_product(orig_to_pl, pl->norm) < 0.0)
		return (multiple_vector(-1.0, pl->norm));
	else
		return (pl->norm);
}

void	hit_plane(t_ray3 *ray, t_plane *pl, t_canvas canvas)
{
	double	tmp;
	double	scalar[3];
	Color c;

	(void)canvas;
	pl->norm = check_plane_direction(pl, ray);
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
		if (pl->type == TPL || pl->type == CPL)
		{
			if (pl->type == TPL)
				c = texture_at(add_vector(ray->origin, multiple_vector(ray->t, ray->dir)), pl->texture);
			else
				c = checkerTexture(add_vector(ray->origin, multiple_vector(ray->t, ray->dir)), 1);
			ray->color[RED] = c.r;
			ray->color[GREEN] = c.g;
			ray->color[BLUE] = c.b;
		}
		if (hit_shadow(ray, canvas))
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

int	discriminant(double a, double b, double c)
{
	if ((b * b) - (4 * a * c) >= 0.0)
		return (1);
	else
		return (0);
}

double	quad_formula(double a, double b, double c)
{
	double	sol1;
	double	sol2;

	sol1 = ((-1) * b - sqrt(b * b - (4 * a * c))) / (2.0 * a);
	sol2 = ((-1) * b + sqrt(b * b - (4 * a * c))) / (2.0 * a);
	if (sol1 > 0.0 && sol2 > 0.0)
		return (sol1);
	else if (sol1 < 0.0 && sol2 > 0.0)
		return (sol2);
	else
		return (-1.0);
}

int	cy_in_range(t_ray3 *ray, double t, t_cylinder *cy)
{
	t_vec3	hit;
	double	height[2];

	hit = add_vector(ray->origin, multiple_vector(t, ray->dir));
	height[0] = scalar_product(sub_vector(hit, cy->center), cy->dir);
	height[1] = scalar_product(sub_vector(hit, cy->center), \
		multiple_vector(-1.0, cy->dir));
	if (height[0] > 0 && height[0] > cy->height / 2)
		return (0);
	if (height[1] > 0 && height[1] > cy->height / 2)
		return (0);
	return (1);
}

void	make_cylinder_cap(t_cylinder *cy)
{
	int	idx;

	idx = 0;
	cy->ucap = (t_plane *)malloc(sizeof(t_plane));
	cy->lcap = (t_plane *)malloc(sizeof(t_plane));
	while (idx < 3)
	{
		cy->ucap->color[idx] = cy->color[idx];
		cy->lcap->color[idx] = cy->color[idx];
		idx++;
	}
	cy->ucap->norm = cy->dir;
	cy->lcap->norm = multiple_vector(-1.0, cy->dir);
	cy->ucap->on_plane = add_vector(cy->center, \
		multiple_vector(cy->height / 2.0, cy->dir));
	cy->lcap->on_plane = add_vector(cy->center, \
		multiple_vector(cy->height / -2.0, cy->dir));
}

void	hit_cap(t_ray3 *ray, t_cylinder *cy, t_plane *cap, t_canvas canvas)
{
	double	tmp;
	double	scalar[3];
	t_vec3	hit;

	scalar[0] = scalar_product(cap->on_plane, cap->norm);
	scalar[1] = scalar_product(ray->origin, cap->norm);
	scalar[2] = scalar_product(ray->dir, cap->norm);
	tmp = (scalar[0] - scalar[1]) / scalar[2];
	hit = add_vector(ray->origin, multiple_vector(tmp, ray->dir));
	if (size_of_vec2(sub_vector(hit, cap->on_plane)) > cy->radius)
		return ;
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		ray->t = tmp;
		ray->type = PL;
		ray->color[RED] = cap->color[RED];
		ray->color[GREEN] = cap->color[GREEN];
		ray->color[BLUE] = cap->color[BLUE];
		ray->obj = (void *)cap;
		if (hit_shadow(ray, canvas))
			ray->type = SHADOW;
	}
}

void	hit_cylinder(t_ray3 *ray, t_cylinder *cy, t_canvas canvas)
{
	t_vec3	oc;
	t_vec3	v[2];
	double	coef[3];
	double	tmp;

	make_cylinder_cap(cy);
	hit_cap(ray, cy, cy->ucap, canvas);
	hit_cap(ray, cy, cy->lcap, canvas);
	oc = sub_vector(ray->origin, cy->center);
	v[0] = vector_product(ray->dir, cy->dir);
	v[1] = vector_product(oc, cy->dir);
	coef[0] = scalar_product(v[0], v[0]);
	coef[1] = 2 * scalar_product(v[0], v[1]);
	coef[2] = scalar_product(v[1], v[1]) - cy->radius * cy->radius;
	if (discriminant(coef[0], coef[1], coef[2]))
		tmp = quad_formula(coef[0], coef[1], coef[2]);
	else
		tmp = -1.0;
	if (tmp >= 0.0 && cy_in_range(ray, tmp, cy) == 0)
		return ;
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		ray->t = tmp;
		ray->color[RED] = cy->color[RED];
		ray->color[GREEN] = cy->color[GREEN];
		ray->color[BLUE] = cy->color[BLUE];
		ray->type = CY;
		ray->obj = (void *)cy;
		if (hit_shadow(ray, canvas))
			ray->type = SHADOW;
	}
}
