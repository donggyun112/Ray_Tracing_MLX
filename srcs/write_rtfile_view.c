/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_ftfile_view.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 22:41:06 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/13 22:41:49 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	write_rt_camera(t_view *view, FILE *f)
{
	fprintf(f, "c	");
	write_rt_vec(view->cam.origin, f);
	write_rt_vec(view->cam.dir, f);
	fprintf(f, " %d \n", view->can.fov);
}

void	write_rt_rlight(t_view *view, FILE *f)
{
	int	i;

	i = -1;
	while (++i < view->can.obj->rl_cnt)
	{
		fprintf(f, "rl	");
		write_rt_vec(view->can.obj->rl[i].light->light_orig, f);
		fprintf(f, "%f	", view->can.obj->rl[i].light->light_bright);
		write_rt_vec(view->can.obj->rl[i].r_center, f);
		write_rt_vec(view->can.obj->rl[i].r_axis, f);
		write_rt_color(view->can.obj->rl[i].light->light_col, f);
		view->can.obj->l[view->can.obj->rl[i].light_idx].rotate_idx = -1;
	}
}

void	write_rt_light(t_view *view, FILE *f)
{
	int		i;
	t_light	tmp;

	write_rt_rlight(view, f);
	i = -1;
	while (++i < view->can.obj->l_cnt)
	{
		if (view->can.obj->l->rotate_idx == -1)
		{
			view->can.obj->l->rotate_idx = 0;
			continue ;
		}
		tmp = view->can.obj->l[i];
		fprintf(f, "l	");
		write_rt_vec(tmp.light_orig, f);
		fprintf(f, " %f	", tmp.light_bright);
		write_rt_color(tmp.light_col, f);
	}
}
