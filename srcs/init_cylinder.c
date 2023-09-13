/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cylinder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 21:58:08 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/13 21:58:37 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	init_texture_cylinder(char **tmp, t_canvas *canvas, int idx, int count)
{
	canvas->obj->cy[idx].type = TCY;
	canvas->obj->cy[idx].center.x = ft_strtod(tmp[1]);
	canvas->obj->cy[idx].center.y = ft_strtod(tmp[2]);
	canvas->obj->cy[idx].center.z = ft_strtod(tmp[3]);
	canvas->obj->cy[idx].dir.x = ft_strtod(tmp[4]);
	canvas->obj->cy[idx].dir.y = ft_strtod(tmp[5]);
	canvas->obj->cy[idx].dir.z = ft_strtod(tmp[6]);
	canvas->obj->cy[idx].radius = ft_strtod(tmp[7]);
	canvas->obj->cy[idx].height = ft_strtod(tmp[8]);
	canvas->obj->cy[idx].filepath = ft_strdup(tmp[9]);
	if (count == 10)
		canvas->obj->cy[idx].bumppath = ft_strdup(tmp[9]);
	else
		canvas->obj->cy[idx].bumppath = NULL;
}

void	init_normal_cylinder(char **tmp, t_canvas *canvas, int idx)
{
	canvas->obj->cy[idx].type = CY;
	canvas->obj->cy[idx].center.x = ft_strtod(tmp[1]);
	canvas->obj->cy[idx].center.y = ft_strtod(tmp[2]);
	canvas->obj->cy[idx].center.z = ft_strtod(tmp[3]);
	canvas->obj->cy[idx].dir.x = ft_strtod(tmp[4]);
	canvas->obj->cy[idx].dir.y = ft_strtod(tmp[5]);
	canvas->obj->cy[idx].dir.z = ft_strtod(tmp[6]);
	canvas->obj->cy[idx].radius = ft_strtod(tmp[7]);
	canvas->obj->cy[idx].height = ft_strtod(tmp[8]);
	canvas->obj->cy[idx].color[RED] = ft_strtod(tmp[9]);
	canvas->obj->cy[idx].color[GREEN] = ft_strtod(tmp[10]);
	canvas->obj->cy[idx].color[BLUE] = ft_strtod(tmp[11]);
	canvas->obj->cy[idx].dir = norm_vec(canvas->obj->cy[idx].dir);
}

void	init_checker_cylinder(char **tmp, t_canvas *canvas, int idx)
{
	canvas->obj->cy[idx].type = CCY;
	canvas->obj->cy[idx].center.x = ft_strtod(tmp[1]);
	canvas->obj->cy[idx].center.y = ft_strtod(tmp[2]);
	canvas->obj->cy[idx].center.z = ft_strtod(tmp[3]);
	canvas->obj->cy[idx].dir.x = ft_strtod(tmp[4]);
	canvas->obj->cy[idx].dir.y = ft_strtod(tmp[5]);
	canvas->obj->cy[idx].dir.z = ft_strtod(tmp[6]);
	canvas->obj->cy[idx].radius = ft_strtod(tmp[7]);
	canvas->obj->cy[idx].height = ft_strtod(tmp[8]);
}

int	init_cylinder(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (count == 11 && !ft_strcmp(tmp[0], "cy"))
		init_normal_cylinder(tmp, canvas, idx);
	else if (count == 8 && !ft_strcmp(tmp[0], "ccy"))
		init_checker_cylinder(tmp, canvas, idx);
	else if ((count == 9 || count == 10) && !ft_strcmp(tmp[0], "tcy"))
		init_texture_cylinder(tmp, canvas, idx, count);
	else
		return (-1);
	canvas->obj->cy[idx].dir = norm_vec(canvas->obj->cy[idx].dir);
	canvas->obj->cy[idx].ucap = NULL;
	canvas->obj->cy[idx].lcap = NULL;
	canvas->obj->cy[idx].angle = 0.0;
	idx++;
	return (0);
}
