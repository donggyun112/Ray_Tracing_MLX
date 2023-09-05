/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 00:55:04 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/06 01:18:07 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int	init_view(char **tmp, t_canvas *canvas, int count)
{
	if (count == 2 && !ft_strcmp(tmp[0], "R"))
		init_view2(canvas, tmp);
	else if (count == 4 && !ft_strcmp(tmp[0], "A"))
	{
		canvas->amb_bright = ft_strtod(tmp[1]);
		canvas->amb_col[RED] = ft_strtod(tmp[2]);
		canvas->amb_col[GREEN] = ft_strtod(tmp[3]);
		canvas->amb_col[BLUE] = ft_strtod(tmp[4]);
	}
	else if (count == 7 && !ft_strcmp(tmp[0], "c"))
	{
		canvas->cam_orig.x = ft_strtod(tmp[1]);
		canvas->cam_orig.y = ft_strtod(tmp[2]);
		canvas->cam_orig.z = ft_strtod(tmp[3]);
		canvas->cam_dir.x = ft_strtod(tmp[4]);
		canvas->cam_dir.y = ft_strtod(tmp[5]);
		canvas->cam_dir.z = ft_strtod(tmp[6]);
		canvas->fov = ft_strtod(tmp[7]);
		canvas->cam_dir = norm_vec(canvas->cam_dir);
	}
	else
		return (-1);
	return (0);
}

int	init_plane(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (count == 9 && !ft_strcmp(tmp[0], "pl"))
	{
		canvas->obj->pl[idx].on_plane.x = ft_strtod(tmp[1]);
		canvas->obj->pl[idx].on_plane.y = ft_strtod(tmp[2]);
		canvas->obj->pl[idx].on_plane.z = ft_strtod(tmp[3]);
		canvas->obj->pl[idx].norm.x = ft_strtod(tmp[4]);
		canvas->obj->pl[idx].norm.y = ft_strtod(tmp[5]);
		canvas->obj->pl[idx].norm.z = ft_strtod(tmp[6]);
		canvas->obj->pl[idx].color[RED] = ft_strtod(tmp[7]);
		canvas->obj->pl[idx].color[GREEN] = ft_strtod(tmp[8]);
		canvas->obj->pl[idx].color[BLUE] = ft_strtod(tmp[9]);
		canvas->obj->pl[idx].norm = norm_vec(canvas->obj->pl[idx].norm);
		idx++;
	}
	else
		return (-1);
	return (0);
}

int	init_sphere(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (count == 7 && !ft_strcmp(tmp[0], "sp"))
	{
		canvas->obj->sp[idx].center.x = ft_strtod(tmp[1]);
		canvas->obj->sp[idx].center.y = ft_strtod(tmp[2]);
		canvas->obj->sp[idx].center.z = ft_strtod(tmp[3]);
		canvas->obj->sp[idx].radius = ft_strtod(tmp[4]);
		canvas->obj->sp[idx].color[RED] = ft_strtod(tmp[5]);
		canvas->obj->sp[idx].color[GREEN] = ft_strtod(tmp[6]);
		canvas->obj->sp[idx].color[BLUE] = ft_strtod(tmp[7]);
		idx++;
	}
	else
		return (-1);
	return (0);
}

int	init_cylinder(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (count == 11 && !ft_strcmp(tmp[0], "cy"))
	{
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
		canvas->obj->cy[idx].ucap = NULL;
		canvas->obj->cy[idx].lcap = NULL;
		idx++;
	}
	else
		return (-1);
	return (0);
}

int	init_light(char **tmp, t_canvas *canvas, int count)
{
	if (!ft_strcmp(tmp[0], "l") && count == 7)
	{
		canvas->light_orig.x = ft_strtod(tmp[1]);
		canvas->light_orig.y = ft_strtod(tmp[2]);
		canvas->light_orig.z = ft_strtod(tmp[3]);
		canvas->light_bright = ft_strtod(tmp[4]);
		canvas->light_col[RED] = ft_strtod(tmp[5]);
		canvas->light_col[GREEN] = ft_strtod(tmp[6]);
		canvas->light_col[BLUE] = ft_strtod(tmp[7]);
	}
	else
		return (-1);
	return (0);
}
