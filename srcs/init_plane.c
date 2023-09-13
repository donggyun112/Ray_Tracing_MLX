/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_plane.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 21:59:13 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/13 21:59:36 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	init_nomal_plane(char **tmp, t_canvas *canvas, int idx)
{
	canvas->obj->pl[idx].type = PL;
	canvas->obj->pl[idx].on_plane.x = ft_strtod(tmp[1]);
	canvas->obj->pl[idx].on_plane.y = ft_strtod(tmp[2]);
	canvas->obj->pl[idx].on_plane.z = ft_strtod(tmp[3]);
	canvas->obj->pl[idx].norm.x = ft_strtod(tmp[4]);
	canvas->obj->pl[idx].norm.y = ft_strtod(tmp[5]);
	canvas->obj->pl[idx].norm.z = ft_strtod(tmp[6]);
	canvas->obj->pl[idx].color[RED] = ft_strtod(tmp[7]);
	canvas->obj->pl[idx].color[GREEN] = ft_strtod(tmp[8]);
	canvas->obj->pl[idx].color[BLUE] = ft_strtod(tmp[9]);
	canvas->obj->pl[idx].tr_v1 = (t_vec3){0.0, 0.0, 0.0};
	canvas->obj->pl[idx].tr_v2 = (t_vec3){0.0, 0.0, 0.0};
	canvas->obj->pl[idx].norm = norm_vec(canvas->obj->pl[idx].norm);
}

void	init_checker_plane(char **tmp, t_canvas *canvas, int idx)
{
	canvas->obj->pl[idx].type = CPL;
	canvas->obj->pl[idx].on_plane.x = ft_strtod(tmp[1]);
	canvas->obj->pl[idx].on_plane.y = ft_strtod(tmp[2]);
	canvas->obj->pl[idx].on_plane.z = ft_strtod(tmp[3]);
	canvas->obj->pl[idx].norm.x = ft_strtod(tmp[4]);
	canvas->obj->pl[idx].norm.y = ft_strtod(tmp[5]);
	canvas->obj->pl[idx].norm.z = ft_strtod(tmp[6]);
	canvas->obj->pl[idx].norm = norm_vec(canvas->obj->pl[idx].norm);
	canvas->obj->pl[idx].tr_v1 = (t_vec3){0.0, 0.0, 0.0};
	canvas->obj->pl[idx].tr_v2 = (t_vec3){0.0, 0.0, 0.0};
}

void	init_texture_plane(char **tmp, t_canvas *canvas, int idx)
{
	canvas->obj->pl[idx].type = TPL;
	canvas->obj->pl[idx].on_plane.x = ft_strtod(tmp[1]);
	canvas->obj->pl[idx].on_plane.y = ft_strtod(tmp[2]);
	canvas->obj->pl[idx].on_plane.z = ft_strtod(tmp[3]);
	canvas->obj->pl[idx].norm.x = ft_strtod(tmp[4]);
	canvas->obj->pl[idx].norm.y = ft_strtod(tmp[5]);
	canvas->obj->pl[idx].norm.z = ft_strtod(tmp[6]);
	canvas->obj->pl[idx].filepath = ft_strdup(tmp[7]);
	canvas->obj->pl[idx].norm = norm_vec(canvas->obj->pl[idx].norm);
	canvas->obj->pl[idx].tr_v1 = (t_vec3){0.0, 0.0, 0.0};
	canvas->obj->pl[idx].tr_v2 = (t_vec3){0.0, 0.0, 0.0};
}

void	init_triangle_plane(char **tmp, t_canvas *canvas, int idx)
{
	canvas->obj->pl[idx].type = TRI;
	canvas->obj->pl[idx].on_plane.x = ft_strtod(tmp[1]);
	canvas->obj->pl[idx].on_plane.y = ft_strtod(tmp[2]);
	canvas->obj->pl[idx].on_plane.z = ft_strtod(tmp[3]);
	canvas->obj->pl[idx].tr_v1.x = ft_strtod(tmp[4]) - ft_strtod(tmp[1]);
	canvas->obj->pl[idx].tr_v1.y = ft_strtod(tmp[5]) - ft_strtod(tmp[2]);
	canvas->obj->pl[idx].tr_v1.z = ft_strtod(tmp[6]) - ft_strtod(tmp[3]);
	canvas->obj->pl[idx].tr_v2.x = ft_strtod(tmp[7]) - ft_strtod(tmp[1]);
	canvas->obj->pl[idx].tr_v2.y = ft_strtod(tmp[8]) - ft_strtod(tmp[2]);
	canvas->obj->pl[idx].tr_v2.z = ft_strtod(tmp[9]) - ft_strtod(tmp[3]);
	canvas->obj->pl[idx].color[RED] = ft_strtod(tmp[10]);
	canvas->obj->pl[idx].color[GREEN] = ft_strtod(tmp[11]);
	canvas->obj->pl[idx].color[BLUE] = ft_strtod(tmp[12]);
	canvas->obj->pl[idx].norm = norm_vec(vector_product(\
		canvas->obj->pl[idx].tr_v1, canvas->obj->pl[idx].tr_v2));
	canvas->obj->pl[idx].filepath = NULL;
}

int	init_plane(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (count == 9 && !ft_strcmp(tmp[0], "pl"))
		init_nomal_plane(tmp, canvas, idx);
	else if (count == 6 && !ft_strcmp(tmp[0], "cpl"))
		init_checker_plane(tmp, canvas, idx);
	else if ((count == 7 || count == 8) && !ft_strcmp(tmp[0], "tpl"))
		init_texture_plane(tmp, canvas, idx);
	else if (count == 12 && !ft_strcmp(tmp[0], "tr"))
		init_triangle_plane(tmp, canvas, idx);
	else
		return (-1);
	idx++;
	return (0);
}
