/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sphere.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 21:56:41 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/13 21:57:54 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	init_nomal_sphere(t_canvas *canvas, char **tmp, int idx)
{
	canvas->obj->sp[idx].type = SP;
	canvas->obj->sp[idx].center.x = ft_strtod(tmp[1]);
	canvas->obj->sp[idx].center.y = ft_strtod(tmp[2]);
	canvas->obj->sp[idx].center.z = ft_strtod(tmp[3]);
	canvas->obj->sp[idx].radius = ft_strtod(tmp[4]);
	canvas->obj->sp[idx].color[RED] = ft_strtod(tmp[5]);
	canvas->obj->sp[idx].color[GREEN] = ft_strtod(tmp[6]);
	canvas->obj->sp[idx].color[BLUE] = ft_strtod(tmp[7]);
	canvas->obj->sp[idx].angle = 0.000000;
	canvas->obj->sp[idx].rsp_type = 0;
	canvas->obj->sp[idx].idx = idx;
}

void	init_texture_sphere(t_canvas *canvas, char **tmp, int idx, int count)
{
	canvas->obj->sp[idx].type = TSP;
	canvas->obj->sp[idx].center.x = ft_strtod(tmp[1]);
	canvas->obj->sp[idx].center.y = ft_strtod(tmp[2]);
	canvas->obj->sp[idx].center.z = ft_strtod(tmp[3]);
	canvas->obj->sp[idx].radius = ft_strtod(tmp[4]);
	canvas->obj->sp[idx].filepath = ft_strdup(tmp[5]);
	if (count == 6)
		canvas->obj->sp[idx].bumppath = ft_strdup(tmp[6]);
	else
		canvas->obj->sp[idx].bumppath = NULL;
	canvas->obj->sp[idx].angle = 0.000000;
	canvas->obj->sp[idx].rsp_type = 0;
	canvas->obj->sp[idx].idx = idx;
}

void	init_checker_sphere(t_canvas *canvas, char **tmp, int idx)
{
	canvas->obj->sp[idx].type = CSP;
	canvas->obj->sp[idx].center.x = ft_strtod(tmp[1]);
	canvas->obj->sp[idx].center.y = ft_strtod(tmp[2]);
	canvas->obj->sp[idx].center.z = ft_strtod(tmp[3]);
	canvas->obj->sp[idx].radius = ft_strtod(tmp[4]);
	canvas->obj->sp[idx].angle = 0.000000;
	canvas->obj->sp[idx].rsp_type = SP;
	canvas->obj->sp[idx].idx = idx;
}

void	init_rotate_sphere(t_canvas *canvas, char **tmp, int idx)
{
	static int	ridx;

	init_checker_sphere(canvas, tmp, idx);
	canvas->obj->sp[idx].type = TSP;
	canvas->obj->rsp[ridx].r_center.x = ft_strtod(tmp[5]);
	canvas->obj->rsp[ridx].r_center.y = ft_strtod(tmp[6]);
	canvas->obj->rsp[ridx].r_center.z = ft_strtod(tmp[7]);
	canvas->obj->rsp[ridx].r_axis.x = ft_strtod(tmp[8]);
	canvas->obj->rsp[ridx].r_axis.y = ft_strtod(tmp[9]);
	canvas->obj->rsp[ridx].r_axis.z = ft_strtod(tmp[10]);
	canvas->obj->sp[idx].filepath = ft_strdup(tmp[11]);
	canvas->obj->sp[idx].bumppath = ft_strdup(tmp[12]);
	canvas->obj->rsp[ridx].sp = &canvas->obj->sp[idx];
	canvas->obj->rsp[ridx].r_axis = norm_vec(canvas->obj->rsp[ridx].r_axis);
	canvas->obj->rsp[ridx].r_radius = \
	size_of_vec(sub_vector(canvas->obj->sp[idx].center, \
	canvas->obj->rsp[ridx].r_center));
	canvas->obj->sp[idx].angle = 0.000000;
	canvas->obj->rsp[ridx].sp_idx = idx;
	canvas->obj->sp[idx].rsp_type = 1;
	canvas->obj->sp[idx].idx = idx;
	ridx++;
}

int	init_sphere(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (count == 7 && !ft_strcmp(tmp[0], "sp"))
		init_nomal_sphere(canvas, tmp, idx);
	else if ((count == 5 || count == 6) && !ft_strcmp(tmp[0], "tsp"))
		init_texture_sphere(canvas, tmp, idx, count);
	else if (count == 4 && !ft_strcmp(tmp[0], "csp"))
		init_checker_sphere(canvas, tmp, idx);
	else if (!ft_strcmp(tmp[0], "rsp") && count == 12)
		init_rotate_sphere(canvas, tmp, idx);
	else
		return (-1);
	idx++;
	return (0);
}