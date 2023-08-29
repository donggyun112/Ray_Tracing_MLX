/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 20:29:45 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/30 00:42:55 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"



t_color	texture_at(t_vec3 point, t_texture texture)
{
	const int	x = (int)(point.z * texture.width) % texture.width;
	const int	y = (int)(point.x * texture.height) % texture.height;
	const int	index = (y * texture.width + x) * 4;
	const int	color = *(int *)(texture.data + index);
	t_color		c;

	c.r = (color >> 16) & 0xFF;
	c.g = (color >> 8) & 0xFF;
	c.b = color & 0xFF;
	return (c);
}

t_color	checkertexture(t_vec3 point, float scale)
{
	int		checkerx;
	int		checkery;
	int		checkerz;

	if (point.z < 0)
		point.z -= 1;
	checkerx = (int)(point.x * scale) % 2;
	checkery = (int)(point.y * scale) % 2;
	checkerz = (int)(point.z * scale) % 2;
	if ((checkerx + checkery + checkerz) % 2 == 0)
		return ((t_color){0, 0, 0});
	else
		return ((t_color){255, 255, 255});
}

void	spherical_map(t_vec3 p, float *u, float *v, t_vec3 center, float ag)
{
	float	theta;
	float	vecmagnitude;
	float	phi;
	float	raw_u;
	t_vec3	relative_point;

	relative_point = (t_vec3){p.x - center.x, p.y - center.y, p.z - center.z};
	theta = atan2f(relative_point.z, relative_point.x);
	vecmagnitude = sqrtf(relative_point.x * relative_point.x + \
	relative_point.y * relative_point.y + relative_point.z * relative_point.z);
	phi = acosf(relative_point.y / vecmagnitude);
	theta += ag;
	raw_u = theta / (2.0f * M_PI);
	*u = (raw_u + 0.5f);
	*v = phi / M_PI;
}

t_color	uv_grid_pattern_at(t_checker pattern, float u, float v)
{
	const int	u2 = (int)(u * pattern.width);
	const int	v2 = (int)(v * pattern.height);

	if ((u2 + v2) % 2)
		return (pattern.color_a);
	else
		return (pattern.color_b);
}

t_color	get_texture_color(t_texture texture, float u, float v)
{
	const int	x = (int)(u * (float)texture.width) % texture.width;
	const int	y = (int)(v * (float)texture.height) % texture.height;
	const int	offset = (x + y * texture.width) * (texture.bpp / 8);
	const int	color = *(int *)(texture.data + offset);
	t_color		c;

	c.r = (color >> 16) & 0xFF;
	c.g = (color >> 8) & 0xFF;
	c.b = color & 0xFF;
	return (c);
}

t_color	image_texture_on_sphere(t_vec3 point, t_vec3 center, t_texture *texture, t_canvas can)
{
	float	u;
	float	v;

	spherical_map(point, &u, &v, center, can.obj->ag);
	return (get_texture_color(*texture, u, v));
}

t_color	grid_texture_on_sphere(t_vec3 point, t_checker pattern, t_vec3 center, t_canvas can)
{
	float	u;
	float	v;

	spherical_map(point, &u, &v, center, can.obj->ag);
	return (uv_grid_pattern_at(pattern, u, v));
}

void	init_texture(t_texture *texture, t_view *view, char *path)
{
	texture->img = mlx_xpm_file_to_image(view->mlx, path, \
	&texture->width, &texture->height);
	if (!texture->img)
	{
		fprintf(stderr, "Failed to load texture: %s\n", path);
		exit(1);
	}
	texture->data = mlx_get_data_addr(texture->img, \
	&texture->bpp, &texture->size_line, &texture->endian);
}

void	sphere_texture(t_ray3 *ray, t_sphere *sp, t_canvas can)
{
	t_color			c;
	const t_checker	pattern = {{255, 255, 255}, {100, 100, 0}, 32, 16};
	t_vec3			hit;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	if (sp->type == TSP)
		c = image_texture_on_sphere(hit, sp->center, &sp->texture, can);
	else
		c = grid_texture_on_sphere(hit, pattern, sp->center, can);
	ray->type = SP;
	ray->color[RED] = c.r;
	ray->color[GREEN] = c.g;
	ray->color[BLUE] = c.b;
}

void	init_sp_color(t_ray3 *ray, t_sphere *sp)
{
	ray->type = SP;
	ray->color[RED] = sp->color[RED];
	ray->color[GREEN] = sp->color[GREEN];
	ray->color[BLUE] = sp->color[BLUE];
}

void	hit_sphere(t_ray3 *ray, t_sphere *sp, t_canvas canvas)
{
	const t_vec3	l = sub_vector(sp->center, ray->origin);
	const double	tca = scalar_product(l, ray->dir);
	const double	d2 = scalar_product(l, l) - (tca * tca);
	const double	tnc = sqrt(sp->radius * sp->radius - d2);
	double			tmp;

	if (tca < 0)
		return ;
	if (d2 > sp->radius * sp->radius)
		return ;
	if (tca - tnc < 0.0)
		tmp = tca + tnc;
	else
		tmp = tca - tnc;
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		ray->t = tmp;
		ray->obj = (void *)sp;
		if (sp->type == TSP || sp->type == CSP)
			sphere_texture(ray, sp, canvas);
		else
			init_sp_color(ray, sp);
		if (hit_shadow(ray, canvas))
			ray->type = SHADOW;
	}
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

void	init_pl_color(t_ray3 *ray, t_plane *pl)
{
	ray->color[RED] = pl->color[RED];
	ray->color[GREEN] = pl->color[GREEN];
	ray->color[BLUE] = pl->color[BLUE];
}

void	init_pltexture(t_ray3 *ray, t_plane *pl)
{
	t_color	c;
	t_vec3	hit;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	if (pl->type == TPL)
		c = texture_at(hit, pl->texture);
	else
		c = checkertexture(hit, 1);
	ray->color[RED] = c.r;
	ray->color[GREEN] = c.g;
	ray->color[BLUE] = c.b;
}

void	hit_plane(t_ray3 *ray, t_plane *pl, t_canvas canvas)
{
	double	tmp;
	double	scalar[3];

	(void)canvas;
	pl->norm = check_plane_direction(pl, ray);
	scalar[0] = scalar_product(pl->on_plane, pl->norm);
	scalar[1] = scalar_product(ray->origin, pl->norm);
	scalar[2] = scalar_product(ray->dir, pl->norm);
	tmp = (scalar[0] - scalar[1]) / scalar[2];
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		ray->t = tmp;
		ray->obj = (void *)pl;
		ray->type = PL;
		if (pl->type == TPL || pl->type == CPL)
			init_pltexture(ray, pl);
		else
			init_pl_color(ray, pl);
		if (hit_shadow(ray, canvas))
			ray->type = SHADOW;
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

void	init_cy_color(t_ray3 *ray, t_cylinder *cy, double tmp)
{
	ray->t = tmp;
	ray->color[RED] = cy->color[RED];
	ray->color[GREEN] = cy->color[GREEN];
	ray->color[BLUE] = cy->color[BLUE];
	ray->type = CY;
	ray->obj = (void *)cy;
}

void	hit_cylinder(t_ray3 *ray, t_cylinder *cy, t_canvas canvas)
{
	const t_vec3	oc = sub_vector(ray->origin, cy->center);
	t_vec3			v[2];
	double			coef[3];
	double			tmp;

	make_cylinder_cap(cy);
	hit_cap(ray, cy, cy->ucap, canvas);
	hit_cap(ray, cy, cy->lcap, canvas);
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
		init_cy_color(ray, cy, tmp);
		if (hit_shadow(ray, canvas))
			ray->type = SHADOW;
	}
}
