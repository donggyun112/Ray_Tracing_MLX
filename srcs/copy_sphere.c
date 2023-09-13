/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_sphere.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:19:40 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 01:20:39 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	init_rsp_sp(t_view *view, int size)
{
	int	i;

	i = -1;
	while (++i < size)
		view->can.obj->rsp[i].sp = \
		&view->can.obj->sp[view->can.obj->rsp[i].sp_idx];
}

void	set_rsp(t_view *view, size_t t, t_sphere *sp, int keycode)
{
	int			i;
	const int	size = view->can.obj->rsp_cnt;
	t_rsphere	rsp;

	init_rsp_sp(view, size);
	if (sp->rsp_type == 1)
	{
		i = -1;
		while (++i < size)
		{
			if (view->can.obj->rsp[i].sp_idx == sp->idx)
			{
				rsp = view->can.obj->rsp[i];
				rsp.sp_idx = t;
				memcpy(&view->can.obj->rsp[size], &rsp, sizeof(rsp));
				if (keycode == Z)
					rand_vec(&view->can.obj->rsp[size].r_axis, 3.0f);
				view->can.obj->rsp[size].sp = &view->can.obj->sp[t];
				view->can.obj->rsp_cnt++;
				break ;
			}
		}
	}
}

void	copy_sphere(t_view *view, int keycode)
{
	t_sphere		sp;
	const size_t	t = view->can.obj->sp_cnt;
	const size_t	y = view->can.obj->rsp_cnt;

	sp = *(t_sphere *)view->grep.obj;
	if (sp.rsp_type == 1)
		view->can.obj->rsp = \
		(t_rsphere *)realloc(view->can.obj->rsp, sizeof(t_rsphere) * (y + 1));
	view->can.obj->sp = \
	(t_sphere *)reallocf(view->can.obj->sp, sizeof(t_sphere) * (t + 1));
	memcpy(&view->can.obj->sp[t], &sp, sizeof(sp));
	if (keycode == Z)
	{
		view->can.obj->sp[t].color[RED] = rand() % 255;
		view->can.obj->sp[t].color[GREEN] = rand() % 255;
		view->can.obj->sp[t].color[BLUE] = rand() % 255;
	}
	view->can.obj->sp[t].idx = t;
	view->can.obj->sp[t].center.x += 1;
	view->can.obj->sp[t].center.y += 1;
	view->can.obj->sp[t].center.z += 1;
	set_rsp(view, t, &sp, keycode);
	view->can.obj->sp_cnt++;
}