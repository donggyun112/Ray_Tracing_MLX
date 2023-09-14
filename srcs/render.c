/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:43:17 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 12:12:57 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	intersection(t_ray3 *ray, t_volume *obj)
{
	int	idx;

	idx = 0;
	while (idx < obj->sp_cnt)
	{
		if (obj->sp[idx].type != NONE)
			hit_sphere(ray, &obj->sp[idx]);
		idx++;
	}
	idx = 0;
	while (idx < obj->pl_cnt)
	{
		if (obj->pl[idx].type != NONE)
			hit_plane(ray, &obj->pl[idx]);
		idx++;
	}
	idx = 0;
	while (idx < obj->cy_cnt)
	{
		if (obj->cy[idx].type != NONE)
			hit_cylinder(ray, &obj->cy[idx]);
		idx++;
	}
}

t_color	anti_aliasing(int pix[2], float vp_idx[2], t_view *view, t_ray3 *ray)
{
	t_color	color;
	float	offset[2];
	int		idx[2];

	color = (t_color){0, 0, 0};
	idx[0] = -1;
	while (++idx[0] < view->anti_scalar)
	{
		idx[1] = -1;
		while (++idx[1] < view->anti_scalar)
		{
			offset[0] = (float)idx[1] / view->anti_scalar;
			offset[1] = (float)idx[0] / view->anti_scalar;
			vp_idx[0] = view->can.ratio * 2.0 * ((float)pix[0] + \
			offset[0]) / (float)view->can.width;
			vp_idx[1] = 2.0 * ((float)pix[1] + \
			offset[1]) / (float)view->can.height;
			*ray = create_ray(view->cam, vp_idx[0], vp_idx[1]);
			ray->pix[0] = pix[0];
			ray->pix[1] = pix[1];
			intersection(ray, view->can.obj);
			color_cal(view, ray, &color, pix);
		}
	}
	return (color);
}

void	low_quality(int scalar, int pix[2], t_ray3 ray, t_view *view)
{
	int			offset[2];
	int			xy[2];

	offset[0] = 0;
	while (offset[0] < scalar)
	{
		offset[1] = 0;
		while (offset[1] < scalar)
		{
			xy[0] = pix[0] + offset[0];
			xy[1] = pix[1] + offset[1];
			if (xy[0] < view->can.width && xy[1] < view->can.height)
				my_mlx_pixel_put(view, xy[0], xy[1], rgb_to_int(ray.real));
			offset[1]++;
		}
		offset[0]++;
	}
}

void	set_thread_st_point(int *anti, int pix[2], t_thread *t)
{
	const int	gap = t->canvas.height / NUM_OF_THREAD;
	int			step;
	int			remain;

	step = gap / t->view->low_scalar;
	remain = gap % step;
	step += !!remain;
	*anti = t->view->anti_scalar * t->view->anti_scalar;
	pix[1] = t->id * (step * t->view->low_scalar);
	if (t->id == NUM_OF_THREAD - 1)
		pix[2] = t->canvas.height;
	else
		pix[2] = (t->id + 1) * (step * t->view->low_scalar);
}

void	make_image2(void *m)
{
	int			pix[3];
	float		vp_idx[2];
	t_color		c;
	t_thread	*t;
	int			anti;

	t = (t_thread *)m;
	set_thread_st_point(&anti, pix, t);
	while (pix[1] < pix[2] && pix[1] < t->canvas.height)
	{
		pix[0] = 0;
		while (pix[0] < t->canvas.width)
		{
			c = anti_aliasing(pix, vp_idx, t->view, &t->ray);
			t->ray.real[RED] = c.r / anti;
			t->ray.real[GREEN] = c.g / anti;
			t->ray.real[BLUE] = c.b / anti;
			low_quality(t->view->low_scalar, pix, t->ray, t->view);
			pix[0] += t->view->low_scalar;
		}
		pix[1] += (t->view->low_scalar);
	}
}
