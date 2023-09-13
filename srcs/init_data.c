/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:36:16 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 08:33:04 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	set_texture(t_view *view, t_volume *obj)
{
	int	i;

	i = -1;
	while (++i < obj->sp_cnt)
	{
		if (obj->sp[i].type == TSP)
		{
			init_texture(&obj->sp[i].texture, view, obj->sp[i].filepath);
			init_texture(&obj->sp[i].bumtexture, view, obj->sp[i].bumppath);
		}
	}
	i = -1;
	while (++i < obj->pl_cnt)
		if (obj->pl[i].type == TPL)
			init_texture(&obj->pl[i].texture, view, obj->pl[i].filepath);
	i = -1;
	while (++i < obj->cy_cnt)
	{
		if (obj->cy[i].type == TCY)
		{
			init_texture(&obj->cy[i].texture, view, obj->cy[i].filepath);
			init_texture(&obj->cy[i].bumtexture, view, obj->cy[i].bumppath);
		}
	}
}

void	init_view_scale(t_view *view)
{
	view->anti_scalar = 1;
	view->low_scalar = 1;
	view->quality_scalar = -4;
	view->flag = 0;
	view->focus = 0;
	view->stop = 1;
	view->show_mouse = 1;
	view->click_status = 0;
	view->backup = NULL;
	view->change_dir = 0;
	view->grep.type = -1;
	view->mini_size = view->can.width - 200;
	view->real_size = view->can.width;
	if (view->can.bgt_filepath)
		init_texture(&view->back, view, view->can.bgt_filepath);
	mlx_mouse_hide();
	mlx_mouse_move(view->win, view->can.width / 2, view->can.height / 2);
}

void	make_cylinder_cap(t_cylinder *cy)
{
	int	idx;

	idx = 0;
	cy->ucap = (t_plane *)malloc(sizeof(t_plane));
	cy->lcap = (t_plane *)malloc(sizeof(t_plane));
	while (idx < 3)
	{
		cy->ucap->color[idx] = cy->color[idx];
		cy->lcap->color[idx] = cy->color[idx];
		idx++;
	}
	cy->ucap->norm = cy->dir;
	cy->lcap->norm = multiple_vector(-1.0, cy->dir);
	cy->ucap->on_plane = add_vector(cy->center, \
		multiple_vector(cy->height / 2.0, cy->dir));
	cy->lcap->on_plane = add_vector(cy->center, \
		multiple_vector(cy->height / -2.0, cy->dir));
}

void	make_obj_cap(t_volume *obj)
{
	int	idx;

	idx = 0;
	while (idx < obj->cy_cnt)
		make_cylinder_cap(&obj->cy[idx++]);
}
