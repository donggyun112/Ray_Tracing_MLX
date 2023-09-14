/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_loop_hook.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:45:20 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 14:07:59 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vec3	rotate_around_specific_point(t_vec3 vec, t_vec3 center, float angle)
{
	t_vec3		translated_vec;
	t_vec3		rotated_vec;
	const float	cos_angle = cos(angle);
	const float	sin_angle = sin(angle);

	translated_vec.x = vec.x - center.x;
	translated_vec.y = vec.y - center.y;
	translated_vec.z = vec.z - center.z;
	rotated_vec.x = translated_vec.x * cos_angle - translated_vec.z * sin_angle;
	rotated_vec.y = translated_vec.y;
	rotated_vec.z = translated_vec.x * sin_angle + translated_vec.z * cos_angle;
	rotated_vec.x += center.x;
	rotated_vec.y += center.y;
	rotated_vec.z += center.z;
	return (rotated_vec);
}

void	change_angle(t_view *view)
{
	int	x;

	x = 0;
	while (x < view->can.obj->sp_cnt || x < view->can.obj->cy_cnt)
	{
		if (x < view->can.obj->sp_cnt)
		{
			if (view->can.obj->sp[x].type == TSP)
				view->can.obj->sp[x].angle += 0.05;
			else
				view->can.obj->sp[x].angle = \
					fmod(view->can.obj->sp[x].angle, 2.0 * M_PI);
		}
		if (x < view->can.obj->cy_cnt)
		{
			if (view->can.obj->cy[x].type == TCY \
			|| view->can.obj->cy[x].type == CCY)
				view->can.obj->cy[x].angle += 0.2;
			view->can.obj->cy[x].angle = \
			fmod(view->can.obj->cy[x].angle, 2.0 * M_PI);
		}
		x++;
	}
}

void	rotate_rsp(int x, t_view *view)
{
	view->can.obj->rsp[x].sp->center = sub_vector(view->can.obj->\
	rsp[x].sp->center, view->can.obj->rsp[x].r_center);
	view->can.obj->rsp[x].sp->center = \
	rotate_around_axis(view->can.obj->rsp[x] \
	.sp->center, view->can.obj->rsp[x].r_axis, 0.05);
	view->can.obj->rsp[x].sp->center = \
	add_vector(view->can.obj->\
	rsp[x].sp->center, view->can.obj->rsp[x].r_center);
}

void	rotate_rl(int x, t_view *view)
{
	view->can.obj->rl[x].light->light_orig = sub_vector(view->can.obj->\
	rl[x].light->light_orig, view->can.obj->rl[x].r_center);
	view->can.obj->rl[x].light->light_orig = \
	rotate_around_axis(view->can.obj->rl[x] \
	.light->light_orig, view->can.obj->rl[x].r_axis, 0.05);
	view->can.obj->rl[x].light->light_orig = \
	add_vector(view->can.obj->\
	rl[x].light->light_orig, view->can.obj->rl[x].r_center);
}

int	loop_hook(t_view *view)
{
	int	i;

	i = -1;
	if (view->flag && view->stop)
	{
		change_angle(view);
		if (view->can.obj->rsp_cnt > 0 || view->can.obj->rl_cnt)
		{
			while (++i < view->can.obj->rsp_cnt)
				rotate_rsp(i, view);
			i = -1;
			while (++i < view->can.obj->rl_cnt)
				rotate_rl(i, view);
		}
		newwin(view);
		move_focus(1, view, 0.005);
	}
	return (0);
}
