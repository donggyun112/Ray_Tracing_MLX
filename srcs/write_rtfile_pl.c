/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_rtfile_obj.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 22:38:33 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/13 22:39:08 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	write_rt_tpl(t_plane pl, FILE *f)
{
	fprintf(f, "tpl ");
	write_rt_vec(pl.on_plane, f);
	write_rt_vec(pl.norm, f);
	fprintf(f, "%s \n", pl.filepath);
}

void	write_rt_cpl(t_plane pl, FILE *f)
{
	fprintf(f, "cpl ");
	write_rt_vec(pl.on_plane, f);
	write_rt_vec(pl.norm, f);
	fprintf(f, " \n");
}

void	write_rt_pl(t_plane pl, FILE *f)
{
	fprintf(f, "pl ");
	write_rt_vec(pl.on_plane, f);
	write_rt_vec(pl.norm, f);
	write_rt_color(pl.color, f);
}

void	write_rt_plane(t_view *view, FILE *f)
{
	int	idx;

	idx = 0;
	while (idx < view->can.obj->pl_cnt)
	{
		if (view->can.obj->pl[idx].type == TPL)
			write_rt_tpl(view->can.obj->pl[idx], f);
		else if (view->can.obj->pl[idx].type == CPL)
			write_rt_cpl(view->can.obj->pl[idx], f);
		else if (view->can.obj->pl[idx].type == PL)
			write_rt_pl(view->can.obj->pl[idx], f);
		else if (view->can.obj->pl[idx].type == TRI)
			write_rt_tr(view->can.obj->pl[idx], f);
		idx++;
	}
}
