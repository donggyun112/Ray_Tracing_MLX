/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_view.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 22:02:34 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/13 22:02:52 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	init_view2(t_canvas *canvas, char **tmp)
{
	canvas->width = fabs(ft_strtod(tmp[1]));
	canvas->height = fabs(ft_strtod(tmp[2]));
	canvas->ratio = (float)canvas->width / (float)canvas->height;
}

int	init_view(char **tmp, t_canvas *canvas, int count)
{
	if (count == 2 && !ft_strcmp(tmp[0], "R"))
		init_view2(canvas, tmp);
	else if (count == 1 && !ft_strcmp(tmp[0], "bg"))
		canvas->bgt_filepath = ft_strdup(tmp[1]);
	else if (count == 4 && !ft_strcmp(tmp[0], "A"))
	{
		canvas->amb_bright = ft_strtod(tmp[1]);
		canvas->amb_col[RED] = ft_strtod(tmp[2]);
		canvas->amb_col[GREEN] = ft_strtod(tmp[3]);
		canvas->amb_col[BLUE] = ft_strtod(tmp[4]);
	}
	else if (count == 7 && !ft_strcmp(tmp[0], "C"))
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

void	init_rlight(char **tmp, t_canvas *canvas, int idx)
{
	static int	ridx;

	canvas->obj->l[idx].light_orig.x = ft_strtod(tmp[1]);
	canvas->obj->l[idx].light_orig.y = ft_strtod(tmp[2]);
	canvas->obj->l[idx].light_orig.z = ft_strtod(tmp[3]);
	canvas->obj->l[idx].light_bright = ft_strtod(tmp[4]);
	canvas->obj->rl[ridx].r_center.x = ft_strtod(tmp[5]);
	canvas->obj->rl[ridx].r_center.y = ft_strtod(tmp[6]);
	canvas->obj->rl[ridx].r_center.z = ft_strtod(tmp[7]);
	canvas->obj->rl[ridx].r_axis.x = ft_strtod(tmp[8]);
	canvas->obj->rl[ridx].r_axis.y = ft_strtod(tmp[9]);
	canvas->obj->rl[ridx].r_axis.z = ft_strtod(tmp[10]);
	canvas->obj->l[idx].light_col[RED] = ft_strtod(tmp[11]);
	canvas->obj->l[idx].light_col[GREEN] = ft_strtod(tmp[12]);
	canvas->obj->l[idx].light_col[BLUE] = ft_strtod(tmp[13]);
	canvas->obj->rl[ridx].light_idx = idx;
	canvas->obj->rl[ridx].light = &canvas->obj->l[idx];
	ridx++;
}

int	init_light(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (!ft_strcmp(tmp[0], "L") && count == 7)
	{
		canvas->obj->l[idx].light_orig.x = ft_strtod(tmp[1]);
		canvas->obj->l[idx].light_orig.y = ft_strtod(tmp[2]);
		canvas->obj->l[idx].light_orig.z = ft_strtod(tmp[3]);
		canvas->obj->l[idx].light_bright = ft_strtod(tmp[4]);
		canvas->obj->l[idx].light_col[RED] = ft_strtod(tmp[5]);
		canvas->obj->l[idx].light_col[GREEN] = ft_strtod(tmp[6]);
		canvas->obj->l[idx].light_col[BLUE] = ft_strtod(tmp[7]);
	}
	else if (!ft_strcmp(tmp[0], "RL") && count == 13)
		init_rlight(tmp, canvas, idx);
	else
		return (-1);
	idx++;
	return (0);
}
