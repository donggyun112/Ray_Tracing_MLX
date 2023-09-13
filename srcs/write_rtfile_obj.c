/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_rtfile_obj.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 22:39:41 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 08:33:30 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	write_rt_rsphere(t_view *view, FILE *f, int idx[100])
{
	int	i;

	i = -1;
	while (++i < 100)
		idx[i] = 0;
	i = -1;
	while (++i < view->can.obj->rsp_cnt)
	{
		if (view->can.obj->rsp[i].sp->type == NONE)
			continue ;
		fprintf(f, "rsp	");
		write_rt_vec(view->can.obj->rsp[i].sp->center, f);
		fprintf(f, " %f ", view->can.obj->rsp[i].sp->radius);
		write_rt_vec(view->can.obj->rsp[i].r_center, f);
		write_rt_vec(view->can.obj->rsp[i].r_axis, f);
		fprintf(f, " %s %s\n", view->can.obj->rsp[i].sp->filepath, \
								view->can.obj->rsp[i].sp->bumppath);
		idx[view->can.obj->rsp[i].sp_idx] = 1;
	}
}

void	write_rt_sphere(t_view *view, FILE *f)
{
	int	i;
	int	idx[100];

	write_rt_rsphere(view, f, idx);
	i = -1;
	while (++i < view->can.obj->sp_cnt)
	{
		if (view->can.obj->sp[i].type == NONE && idx[i] == 1)
			continue ;
		if (view->can.obj->sp[i].type == CSP)
			fprintf(f, "csp	");
		else if (view->can.obj->sp[i].type == TSP)
			fprintf(f, "tsp	");
		else
			fprintf(f, "sp	");
		write_rt_vec(view->can.obj->sp[i].center, f);
		fprintf(f, " %f ", view->can.obj->sp[i].radius);
		if (view->can.obj->sp[i].type == CSP)
			fprintf(f, "\n");
		else if (view->can.obj->sp[i].type == TSP)
			fprintf(f, " %s %s\n", view->can.obj->sp[i].filepath, \
								view->can.obj->sp[i].bumppath);
		else
			write_rt_color(view->can.obj->sp[i].color, f);
	}
}

void	write_rt_tr(t_plane pl, FILE *f)
{
	fprintf(f, "tr ");
	write_rt_vec(pl.on_plane, f);
	write_rt_vec(add_vector(pl.tr_v1, pl.on_plane), f);
	write_rt_vec(add_vector(pl.tr_v2, pl.on_plane), f);
	write_rt_color(pl.color, f);
}

void	write_rt_cylinder(t_view *view, FILE *f)
{
	int	i;

	i = -1;
	while (++i < view->can.obj->cy_cnt)
	{
		if (view->can.obj->cy[i].type == NONE)
			continue ;
		if (view->can.obj->cy[i].type == TCY)
			fprintf(f, "tcy ");
		else if (view->can.obj->cy[i].type == CCY)
			fprintf(f, "ccy ");
		else
			fprintf(f, "cy ");
		write_rt_vec(view->can.obj->cy[i].center, f);
		write_rt_vec(view->can.obj->cy[i].dir, f);
		fprintf(f, "%f ", view->can.obj->cy[i].radius);
		fprintf(f, "%f ", view->can.obj->cy[i].height);
		if (view->can.obj->cy[i].type == TCY)
			fprintf(f, "%s %s\n", view->can.obj->cy[i].filepath, \
								view->can.obj->cy[i].bumppath);
		else if (view->can.obj->cy[i].type == CCY)
			fprintf(f, "\n");
		else
			write_rt_color(view->can.obj->cy[i].color, f);
	}
}
