/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:17:38 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 22:41:46 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int	win_destroy(t_view *view)
{
	mlx_destroy_window(view->mlx, view->win);
	exit(0);
	return (0);
}



void	newwin(t_view *view)
{
	if (!view->stop)
		return ;
	mlx_clear_window(view->mlx, view->win);
	multi_rend(view);
	mlx_put_image_to_window(view->mlx, view->win, view->img, 0, 0);
	string_put(view);
}

void	quality(int keycode, t_view *view)
{
	if (keycode == Q_UP)
	{
		if (view->quality_scalar >= 6)
			view->quality_scalar = 6;
		view->quality_scalar += 1;
	}
	else if (keycode == Q_DOWN)
		view->quality_scalar -= 1;
	else if (keycode == T)
		view->quality_scalar = 1;
	else if (keycode == Q2)
		view->quality_scalar = -10;
	if (keycode == Q1)
	{
		view->quality_scalar = 6;
		newwin(view);
		pause_system(view);
	}
	else
		newwin(view);
}


void	make_cylinder_cap2(t_cylinder *cy)
{
	free(cy->ucap);
	free(cy->lcap);
	make_cylinder_cap(cy);
}



void	pause_system(t_view *view)
{
	view->stop = !view->stop;
	if (view->stop)
	{
		if (!view->show_mouse)
		{
			mlx_mouse_hide();
			view->show_mouse = !view->show_mouse;
		}
		mlx_mouse_move(view->win, view->can.width / 2, view->can.height / 2);
	}
	else
	{
		if (view->show_mouse)
		{
			mlx_mouse_show();
			view->show_mouse = !view->show_mouse;
		}
	}
}

void	move_hook(int keycode, t_view *view)
{
	view->focus = 1;
	if (keycode == A || keycode == D)
		left_right(keycode, view);
	else if (keycode == W || keycode == S)
		foward_back(keycode, view);
}

void	rotate_hook(int keycode, t_view *view)
{
	if (keycode == 125 || keycode == 126)
		rotate_vertical(keycode, view);
	else if (keycode == 124 || keycode == 123)
		rotate_horizontal(keycode, view);
}

void	init_ppm(FILE *f, unsigned int tmp, unsigned char color[3])
{
	color[RED] = (tmp >> 16) & 0xFF;
	color[GREEN] = (tmp >> 8) & 0xFF;
	color[BLUE] = tmp & 0xFF;
	fwrite(&color[RED], 1, 1, f);
	fwrite(&color[GREEN], 1, 1, f);
	fwrite(&color[BLUE], 1, 1, f);
}

void	save_image_to_ppm(char *filename, t_view *view)
{
	FILE			*f;
	int				xy[2];
	char			*dst;
	unsigned int	tmp;
	unsigned char	color[3];

	f = fopen(filename, "w");
	if (!f)
		return ;
	fprintf(f, "P6\n%d %d\n255\n", view->can.width, view->can.height);
	xy[1] = -1;
	while (++xy[1] < view->can.height)
	{
		xy[0] = -1;
		while (++xy[0] < view->can.width)
		{
			dst = view->addr + (xy[1] * view->line_length + xy[0] * \
			(view->bits_per_pixel / 8));
			tmp = *(unsigned int *)dst;
			init_ppm(f, tmp, color);
		}
	}
	fclose(f);
}


t_vec3	init_copy_vec(t_vec3 *vec, t_vec3 tar)
{
	vec->x = tar.x + 1;
	vec->y = tar.y + 1;
	vec->z = tar.z + 1;
	return (*vec);
}

float	get_rand(float a, float b)
{
	return (a + (b - a) * (float)rand() / (float)RAND_MAX);
}

void	rand_vec(t_vec3 *vec, float max)
{
	vec->x = get_rand(0.0f, max);
	vec->y = get_rand(0.0f, max);
	vec->z = get_rand(0.0f, max);
	*vec = norm_vec(*vec);
}

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

void	copy_normal_cylinder(t_view *view, size_t t, int keycode)
{
	if (keycode == Z)
	{
		view->can.obj->cy[t].color[RED] = rand() % 255;
		view->can.obj->cy[t].color[GREEN] = rand() % 255;
		view->can.obj->cy[t].color[BLUE] = rand() % 255;
	}
}

void	push_front(t_backup **backup, void *obj, int type)
{
	t_backup	*tmp;
	t_backup	*head;

	tmp = (t_backup *)malloc(sizeof(t_backup));
	tmp->obj = obj;
	tmp->type = type;
	head = *backup;
	if (head == NULL)
	{
		head = tmp;
		head->next = NULL;
		*backup = head;
	}
	else
	{
		tmp->next = head;
		head = tmp;
		*backup = head;
	}
}

void	copy_cylinder(t_view *view, int keycode)
{
	t_cylinder		cy;
	const size_t	t = view->can.obj->cy_cnt;

	cy = *(t_cylinder *)view->grep.obj;
	view->can.obj->cy = \
	(t_cylinder *)reallocf(view->can.obj->cy, sizeof(t_cylinder) * (t + 1));
	memcpy(&view->can.obj->cy[t], &cy, sizeof(cy));
	view->can.obj->cy[t].center.x += 1;
	view->can.obj->cy[t].center.y += 1;
	view->can.obj->cy[t].center.z += 1;
	if (keycode == Z)
	{
		view->can.obj->cy[t].height = get_rand(0.5, 3);
		view->can.obj->cy[t].radius = get_rand(0.5, 3);
		view->can.obj->cy[t].dir.x = get_rand(-1.0f, 1.0f);
		view->can.obj->cy[t].dir.y = get_rand(-1.0f, 1.0f);
		view->can.obj->cy[t].dir.z = get_rand(-1.0f, 1.0f);
		view->can.obj->cy[t].dir = norm_vec(view->can.obj->cy[t].dir);
		view->can.obj->cy[t].type = CY;
	}
	if (view->can.obj->cy[t].type == CY)
		copy_normal_cylinder(view, t, keycode);
	make_cylinder_cap(&view->can.obj->cy[t]);
	view->can.obj->cy_cnt++;
}

void	copy_normal_plane(t_view *view, size_t t, int keycode)
{
	if (keycode == Z)
	{
		view->can.obj->pl[t].type = PL;
		view->can.obj->pl[t].color[RED] = rand() % 255;
		view->can.obj->pl[t].color[GREEN] = rand() % 255;
		view->can.obj->pl[t].color[BLUE] = rand() % 255;
	}
}

void	copy_plane(t_view *view, int keycode)
{
	t_plane			pl;
	const size_t	t = view->can.obj->pl_cnt;

	pl = *(t_plane *)view->grep.obj;
	view->can.obj->pl = \
	(t_plane *)realloc(view->can.obj->pl, sizeof(t_plane) * (t + 1));
	memcpy(&view->can.obj->pl[t], &pl, sizeof(pl));
	copy_normal_plane(view, t, keycode);
	view->can.obj->pl_cnt++;
}

void	push_obj(t_sphere *sp, t_cylinder *cy, t_plane *pl, t_view *view)
{
	if (sp)
	{
		push_front(&view->backup, view->grep.obj, sp->type);
		sp->type = NONE;
	}
	else if (cy)
	{
		push_front(&view->backup, view->grep.obj, cy->type);
		cy->type = NONE;
	}
	else if (pl)
	{
		push_front(&view->backup, view->grep.obj, pl->type);
		pl->type = NONE;
	}
}

void	obj_copy(t_view *view, int keycode)
{
	if (view->grep.type == SP)
	{
		if (keycode == F)
			push_obj((t_sphere *)view->grep.obj, NULL, NULL, view);
		else
			copy_sphere(view, keycode);
	}
	else if (view->grep.type == CY)
	{
		if (keycode == F)
			push_obj(NULL, (t_cylinder *)view->grep.obj, NULL, view);
		else
			copy_cylinder(view, keycode);
	}
	else if (view->grep.type == PL)
	{
		if (keycode == F)
			push_obj(NULL, NULL, (t_plane *)view->grep.obj, view);
		else
			copy_plane(view, keycode);
	}
	view->grep.grep = OFF;
	newwin(view);
}

void	type_def(void *obj, int type)
{
	t_sphere	*sp;
	t_plane		*pl;
	t_cylinder	*cy;

	if (type == SP || type == TSP || type == CSP)
	{
		sp = (t_sphere *)obj;
		sp->type = type;
	}
	else if (type == CY || type == TCY || type == CCY)
	{
		cy = (t_cylinder *)obj;
		cy->type = type;
	}
	else if (type == PL || type == TPL || type == CPL)
	{
		pl = (t_plane *)obj;
		pl->type = type;
	}
}

int	clear_backup(t_backup **backup)
{
	t_backup	*tmp;

	if (*backup == NULL)
		return (0);
	if ((*backup)->type == SP || (*backup)->type == TSP \
	|| (*backup)->type == CSP)
		type_def((*backup)->obj, (*backup)->type);
	else if ((*backup)->type == CY || (*backup)->type == CCY \
	|| (*backup)->type == TCY)
		type_def((*backup)->obj, (*backup)->type);
	else if ((*backup)->type == PL || (*backup)->type == CPL \
	|| (*backup)->type == TPL)
		type_def((*backup)->obj, (*backup)->type);
	tmp = (*backup)->next;
	free((*backup));
	(*backup) = tmp;
	return (1);
}

void	my_mlx_pixel_put(t_view *mlx, int x, int y, unsigned int color)
{
	char	*dst;

	dst = mlx->addr + (y * mlx->line_length + x * (mlx->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	rgb_to_int(int color[])
{
	return ((color[RED] << 16) | (color[GREEN] << 8) \
	| color[BLUE]);
}
