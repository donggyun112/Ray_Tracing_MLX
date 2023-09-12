/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 20:29:45 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/12 15:36:24 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vec3	get_u_v_axes(t_vec3 norm)
{
	if (fabs(norm.x) > fabs(norm.y) && fabs(norm.x) > fabs(norm.z))
		return ((t_vec3){0, 1, 0});
	else if (fabs(norm.y) > fabs(norm.x) && fabs(norm.y) > fabs(norm.z))
		return ((t_vec3){1, 0, 0});
	else
		return ((t_vec3){1, 0, 0});
}

int	checker_value(float coordinate)
{
	if (coordinate < 0)
	{
		coordinate -= 1;
	}
	return ((int)coordinate % 2);
}

t_color	checkertexture(t_vec3 point, float scale, t_plane *pl)
{
	t_vec3	u_axis;
	t_vec3	v_axis;
	t_vec3	relative_pos;
	float	u;
	float	v;

	relative_pos = sub_vector(point, pl->on_plane);
	u_axis = get_u_v_axes(pl->norm);
	v_axis = vector_product(pl->norm, u_axis);
	u = scalar_product(relative_pos, u_axis) * scale;
	v = scalar_product(relative_pos, v_axis) * scale;
	if ((checker_value(u) + checker_value(v)) % 2 == 0)
		return ((t_color){0, 0, 0});
	else
		return ((t_color){255, 255, 255});
}

void	spherical_map(t_vec3 p, float *u, float *v, t_sphere *sp)
{
	float	theta;
	float	vecmagnitude;
	float	phi;
	float	raw_u;
	t_vec3	relative_point;

	relative_point = \
	(t_vec3){p.x - sp->center.x, p.y - sp->center.y, p.z - sp->center.z};
	theta = atan2f(relative_point.z, relative_point.x);
	vecmagnitude = sqrtf(relative_point.x * relative_point.x + \
	relative_point.y * relative_point.y + relative_point.z * relative_point.z);
	phi = acosf(relative_point.y / vecmagnitude);
	theta += sp->angle;
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

t_color	image_texture_on_sphere(t_vec3 point, t_sphere *sp, t_texture *texture)
{
	float	u;
	float	v;

	spherical_map(point, &u, &v, sp);
	return (get_texture_color(*texture, u, v));
}

t_color	grid_texture_on_sphere(t_vec3 point, t_checker pattern, t_sphere *sp)
{
	float	u;
	float	v;

	spherical_map(point, &u, &v, sp);
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

void	sphere_texture(t_ray3 *ray, t_sphere *sp)
{
	t_color			c;
	const t_checker	pattern = {{255, 255, 255}, {100, 100, 0}, 32, 16};
	t_vec3			hit;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	if (sp->type == TSP)
		c = image_texture_on_sphere(hit, sp, &sp->texture);
	else
		c = grid_texture_on_sphere(hit, pattern, sp);
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

void	hit_sphere(t_ray3 *ray, t_sphere *sp)
{
	const t_vec3	l = sub_vector(sp->center, ray->origin);
	const float		tca = scalar_product(l, ray->dir);
	const float		d2 = scalar_product(l, l) - (tca * tca);
	const float		tnc = sqrt(sp->radius * sp->radius - d2);
	float			tmp;

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
			sphere_texture(ray, sp);
		else
			init_sp_color(ray, sp);
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

void	get_plane_uv(t_vec3 point, t_plane *pl, float scale, float *u, float *v)
{
	t_vec3	u_axis;
	t_vec3	v_axis;
	t_vec3	relative_pos;

	relative_pos = sub_vector(point, pl->on_plane);
	u_axis = get_u_v_axes(pl->norm);
	v_axis = vector_product(pl->norm, u_axis);
	*u = scalar_product(relative_pos, u_axis) * scale;
	*v = scalar_product(relative_pos, v_axis) * scale;
}

void	init_pltexture(t_ray3 *ray, t_plane *pl)
{
	t_color	c;
	t_vec3	hit;
	float	u;
	float	v;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	if (pl->type == TPL)
	{
		get_plane_uv(hit, pl, 0.02, &u, &v);
		u = u - floorf(u);
		v = v - floorf(v);
		c = get_texture_color(pl->texture, u, v);
		// c = get_texture_color(pl->texture, \
		// ((float)ray->pix[0] / pl->texture.width), \
		// ((float)ray->pix[1] / pl->texture.height));
	}
	else
		c = checkertexture(hit, 1, pl);
	ray->color[RED] = c.r;
	ray->color[GREEN] = c.g;
	ray->color[BLUE] = c.b;
}

int	in_triangle(t_vec3 hit, t_plane *pl)
{
	t_vec3	pos;
	t_vec3	v1_hit;
	t_vec3	v2_hit;
	float	size;

	pos = sub_vector(hit, pl->on_plane);
	v1_hit = vector_product(pl->tr_v1, pos);
	v2_hit = vector_product(pos, pl->tr_v2);
	if (!(scalar_product(pl->norm, v1_hit) > 0.0
			&& scalar_product(pl->norm, v2_hit) > 0.0))
		return (0);
	size = size_of_vec2(v1_hit) + size_of_vec2(v2_hit);
	if (size > size_of_vec2(vector_product(pl->tr_v1, pl->tr_v2)))
		return (0);
	return (1);
}

void	hit_triangle(t_ray3 *ray, t_plane *pl)
{
	float	tmp;
	float	scalar[3];
	t_vec3	hit;

	scalar[0] = scalar_product(pl->on_plane, pl->norm);
	scalar[1] = scalar_product(ray->origin, pl->norm);
	scalar[2] = scalar_product(ray->dir, pl->norm);
	tmp = (scalar[0] - scalar[1]) / scalar[2];
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		hit = add_vector(ray->origin, multiple_vector(tmp, ray->dir));
		if (in_triangle(hit, pl) == 0)
			return ;
		ray->t = tmp;
		ray->obj = (void *)pl;
		ray->type = TRI;
		init_pl_color(ray, pl);
	}
}

void	hit_plane(t_ray3 *ray, t_plane *pl)
{
	float	tmp;
	float	scalar[3];

	if (pl->type == TRI)
	{
		hit_triangle(ray, pl);
		return ;
	}
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
	}
}

int	discriminant(float a, float b, float c)
{
	if ((b * b) - (4 * a * c) >= 0.0)
		return (1);
	else
		return (0);
}

float	quad_formula(float a, float b, float c)
{
	float	sol1;
	float	sol2;

	sol1 = ((-1) * b - sqrt(b * b - (4 * a * c))) / (2.0 * a);
	sol2 = ((-1) * b + sqrt(b * b - (4 * a * c))) / (2.0 * a);
	if (sol1 > 0.0 && sol2 > 0.0)
		return (sol1);
	else if (sol1 < 0.0 && sol2 > 0.0)
		return (sol2);
	else
		return (-1.0);
}

int	cy_in_range(t_ray3 *ray, float t, t_cylinder *cy)
{
	t_vec3		hit;
	const float	condition = cy->height / 2;
	float		height[2];

	hit = add_vector(ray->origin, multiple_vector(t, ray->dir));
	height[0] = scalar_product(sub_vector(hit, cy->center), cy->dir);
	height[1] = scalar_product(sub_vector(hit, cy->center), \
		multiple_vector(-1.0, cy->dir));
	if (height[0] > 0 && height[0] > condition)
		return (0);
	if (height[1] > 0 && height[1] > condition)
		return (0);
	return (1);
}

void	cap_texture(t_vec3 point, t_cylinder *cy, t_plane *cap, t_ray3 *ray)
{
	t_color	c;

	if (cy->type == CCY)
		c = checkertexture(point, 5.0, cap);
	else
		c = get_texture_color(cy->texture, fabs(point.x), fabs(point.z));
	ray->color[RED] = c.r;
	ray->color[GREEN] = c.g;
	ray->color[BLUE] = c.b;
}

void	init_cap_color(t_ray3 *ray, t_plane *cap)
{
	ray->color[RED] = cap->color[RED];
	ray->color[GREEN] = cap->color[GREEN];
	ray->color[BLUE] = cap->color[BLUE];
}

void	hit_cap(t_ray3 *ray, t_cylinder *cy, t_plane *cap)
{
	float	tmp;
	float	scalar[3];
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
		ray->type = CAP;
		ray->obj = (void *)cap;
		if (cy->type == CCY || cy->type == TCY)
			cap_texture(hit, cy, cap, ray);
		else
			init_cap_color(ray, cap);
	}
}

void	init_cy_color(t_ray3 *ray, t_cylinder *cy, float tmp)
{
	ray->t = tmp;
	ray->color[RED] = cy->color[RED];
	ray->color[GREEN] = cy->color[GREEN];
	ray->color[BLUE] = cy->color[BLUE];
	ray->type = CY;
	ray->obj = (void *)cy;
}

t_vec3 main_axis(t_vec3 dir)
{
	t_vec3	abs_dir;

	abs_dir.x = fabsf(dir.x);
	abs_dir.y = fabsf(dir.y);
	abs_dir.z = fabsf(dir.z);
	if (abs_dir.x > abs_dir.y && abs_dir.x > abs_dir.z)
		return ((t_vec3){1, 0, 0});
	if (abs_dir.y > abs_dir.x && abs_dir.y > abs_dir.z)
		return ((t_vec3){0, 1, 0});
	return ((t_vec3){0, 0, 1});
}

void cylindrical_map(t_vec3 p, float *u, float *v, t_cylinder *cy)
{
	const t_vec3	relative_point = sub_vector(p, cy->center);
	float			projected_y;
	float			theta;
	const t_vec3	proj = sub_vector(relative_point, \
	multiple_vector(scalar_product(relative_point, cy->dir), cy->dir));
	const t_vec3	axis = main_axis(cy->dir);

	if (axis.x == 1)
		theta = atan2f(proj.z, proj.y);
	else if (axis.y == 1)
		theta = atan2f(proj.z, proj.x);
	else
		theta = atan2f(proj.y, proj.x);
	theta += cy->angle;
	theta = fmod(theta + 2.0f * M_PI, 2.0f * M_PI);
	*u = (theta + M_PI) / (2.0f * M_PI);
	projected_y = scalar_product(relative_point, cy->dir);
	*v = (projected_y + cy->height / 2) / cy->height;
}

t_color	get_checker_pattern(t_vec3 p, t_cylinder *cy)
{
	float	u;
	float	v;
	int		u_checker;
	int		v_checker;

	cylindrical_map(p, &u, &v, cy);
	u_checker = (int)(u * 32);
	v_checker = (int)(v * 16);
	if ((u_checker + v_checker) % 2 == 0)
		return ((t_color){0, 0, 0});
	else
		return ((t_color){255, 255, 255});
}

t_color	image_textur_on_cylinder(t_vec3 point, t_cylinder *cy, t_texture *tex)
{
	float	u;
	float	v;

	cylindrical_map(point, &u, &v, cy);
	return (get_texture_color(*tex, u, v));
}

void	cylinder_texture(t_ray3 *ray, t_cylinder *cy, float tmp)
{
	t_vec3	hit;
	t_color	c;

	ray->obj = (void *)cy;
	ray->t = tmp;
	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	if (cy->type == CCY)
		c = get_checker_pattern(hit, cy);
	else
		c = image_textur_on_cylinder(hit, cy, &cy->texture);
	ray->color[RED] = c.r;
	ray->color[GREEN] = c.g;
	ray->color[BLUE] = c.b;
	ray->type = CY;
}

void	hit_cylinder(t_ray3 *ray, t_cylinder *cy)
{
	const t_vec3	oc = sub_vector(ray->origin, cy->center);
	t_vec3			v[2];
	float			coef[3];
	float			tmp;

	hit_cap(ray, cy, cy->ucap);
	hit_cap(ray, cy, cy->lcap);
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
		if (cy->type == CCY || cy->type == TCY)
			cylinder_texture(ray, cy, tmp);
		else
			init_cy_color(ray, cy, tmp);
	}
}
