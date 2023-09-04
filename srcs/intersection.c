/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 20:29:45 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/04 23:37:39 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_color	checkertexture(t_vec3 point, float scale, t_plane *pl, int flag)
{
	int		checkerx;
	int		checkery;
	int		checkerz;

	if (point.z < 0)
		point.z -= 1;
	if (point.x < 0)
		point.x -= 1;
	if (point.y < 0)
		point.y -= 1;
	checkerx = (int)(point.x * scale) % 2;
	checkery = (int)(point.y * scale) % 2;
	checkerz = (int)(point.z * scale) % 2;
	if (flag == 1 && (checkerx + checkerz) % 2 == 0)
		return ((t_color){0, 0, 0});
	else if (flag == 1)
		return ((t_color){255, 255, 255});
	else if ((pl->norm.z == 1.0 && ((checkerx + checkery) % 2 == 0)) || (pl->norm.z == -1.0 && ((checkerx + checkery) % 2 == 0)))
		return ((t_color){0, 0, 0});
	else if (pl->norm.z != 1.0 && (checkerx + checkery + checkerz) % 2 == 0 && pl->norm.z != -1.0)
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

t_color	image_texture_on_sphere(t_vec3 point, t_sphere *sp, t_texture *texture)
{
	float	u;
	float	v;

	spherical_map(point, &u, &v, sp->center, sp->angle);
	return (get_texture_color(*texture, u, v));
}

t_color	grid_texture_on_sphere(t_vec3 point, t_checker pattern, t_sphere *sp)
{
	float	u;
	float	v;

	spherical_map(point, &u, &v, sp->center, sp->angle);
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

void	hit_sphere(t_ray3 *ray, t_sphere *sp, t_canvas canvas)
{
	const t_vec3	l = sub_vector(sp->center, ray->origin);
	const float	tca = scalar_product(l, ray->dir);
	const float	d2 = scalar_product(l, l) - (tca * tca);
	const float	tnc = sqrt(sp->radius * sp->radius - d2);
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
		c = get_texture_color(pl->texture, ((float)ray->pix[0] / pl->texture.width), ((float)ray->pix[1] / pl->texture.height));
	else
		c = checkertexture(hit, 1, pl, 0);
	ray->color[RED] = c.r;
	ray->color[GREEN] = c.g;
	ray->color[BLUE] = c.b;
}

void	hit_plane(t_ray3 *ray, t_plane *pl, t_canvas canvas)
{
	float	tmp;
	float	scalar[3];

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

void endcap_map(t_vec3 p, float *u, float *v, t_vec3 center, float radius)
{
    t_vec3 relative_point = (t_vec3){p.z - center.z, p.x - center.x, 0};

    *u = relative_point.x / (2.0 * radius) + 0.5; // Mapping x to U with normalization
    *v = relative_point.z / (2.0 * radius) + 0.5; // Mapping z to V with normalization
}

t_color checker_pattern(t_vec3 p, t_vec3 center, float radius, float tile_size)
{
    float u, v;
    endcap_map(p, &u, &v, center, radius);

    int u_checker = (int)(u * tile_size);
    int v_checker = (int)(v * tile_size);

    // Decide color based on UV coordinates
    if ((u_checker + v_checker) % 2)
        return (t_color){255, 255, 255}; // white
    else
        return (t_color){255, 255, 0}; // black
}


void	hit_cap(t_ray3 *ray, t_cylinder *cy, t_plane *cap, t_canvas canvas)
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
		ray->type = PL;
		ray->color[RED] = cap->color[RED];
		ray->color[GREEN] = cap->color[GREEN];
		ray->color[BLUE] = cap->color[BLUE];
		ray->obj = (void *)cap;
		if (cy->type == CCY)
		{
			t_color c = checkertexture(hit, 5.0, cap, 1);
			ray->color[RED] = c.r;
			ray->color[GREEN] = c.g;
			ray->color[BLUE] = c.b;
		}
		if (hit_shadow(ray, canvas))
			ray->type = SHADOW;
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

void cylindrical_map(t_vec3 p, float *u, float *v, t_vec3 center, float height)
{
	t_vec3	relative_point;
	float	theta;

	relative_point = (t_vec3){p.x - center.x, p.y - center.y, p.z - center.z};
	theta = atan2f(relative_point.x, relative_point.z); 
	*u = (theta + M_PI) / (2.0f * M_PI);
	*v = (p.y - center.y + height/2) / height;
}


t_color get_checker_pattern(t_vec3 p, t_vec3 center, float height)
{
    float	u; 
	float	v;
	int		u_checker;
	int		v_checker;

	cylindrical_map(p, &u, &v, center, height);
    u_checker = (int)(u * 32);
    v_checker = (int)(v * 16);
    if ((u_checker + v_checker) % 2 == 0)
        return ((t_color){0, 0, 0});
    else
        return ((t_color){255, 255, 255});
}

t_color	image_textur_on_cylinder(t_vec3 point, t_cylinder *cy, t_texture *texture)
{
	float	u;
	float	v;

	cylindrical_map(point, &u, &v, cy->center, cy->height);
	return (get_texture_color(*texture, u, v));
}

void	cylinder_texture(t_ray3 *ray, t_cylinder *cy)
{
	const t_vec3	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	t_color			c;

	if (cy->type == CCY)
		c = get_checker_pattern(hit, cy->center, cy->height);
	else
		c = image_textur_on_cylinder(hit, cy, &cy->texture);
	ray->color[RED] = c.r;
	ray->color[GREEN] = c.g;
	ray->color[BLUE] = c.b;
	ray->type = CY;
}

void	hit_cylinder(t_ray3 *ray, t_cylinder *cy, t_canvas canvas)
{
	const t_vec3	oc = sub_vector(ray->origin, cy->center);
	t_vec3			v[2];
	float			coef[3];
	float			tmp;

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
		ray->t = tmp;
		ray->obj = (void *)cy;
		if (cy->type == CCY)
			cylinder_texture(ray, cy);
		else
			init_cy_color(ray, cy, tmp);
		if (hit_shadow(ray, canvas))
			ray->type = SHADOW;
	}
}
