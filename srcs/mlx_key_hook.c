/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_key_hook.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 22:07:51 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/13 22:14:54 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	key_hook5(int keycode, t_view *view, int lidx)
{
	if (keycode == HOME)
	{
		view->can.obj->l[lidx].light_col[BLUE] += 5;
		if (view->can.obj->l[lidx].light_col[BLUE] >= 255)
		{
			view->can.obj->l[lidx].light_col[BLUE] = 255;
			view->can.obj->l[lidx].light_col[GREEN] += 5;
			if (view->can.obj->l[lidx].light_col[GREEN] >= 255)
			{
				view->can.obj->l[lidx].light_col[GREEN] = 255;
				view->can.obj->l[lidx].light_col[RED] += 5;
				if (view->can.obj->l[lidx].light_col[RED] >= 255)
					view->can.obj->l[lidx].light_col[RED] = 255;
			}
		}
		newwin(view);
	}
	else if (keycode == END)
	{
		view->can.obj->l[lidx].light_col[RED] = rand() % 255;
		view->can.obj->l[lidx].light_col[GREEN] = rand() % 255;
		view->can.obj->l[lidx].light_col[BLUE] = rand() % 255;
		newwin(view);
	}
}

void	key_hook4(int keycode, t_view *view, int *lidx)
{
	if (keycode == NEXT && *lidx < view->can.obj->l_cnt - 1)
	{
		(*lidx)++;
		view->lnum = *lidx;
		newwin(view);
	}
	else if (keycode == PRIV && lidx > 0)
	{
		(*lidx)--;
		view->lnum = *lidx;
		newwin(view);
	}
}

void	key_hook3(int keycode, t_view *view, int lidx)
{
	if (view->grep.grep == ON && view->click_status)
		if (keycode == C || keycode == Z || keycode == F)
			obj_copy(view, keycode);
	if (keycode == G)
		if (clear_backup(&view->backup))
			newwin(view);
	if (keycode == Q)
	{
		view->change_dir = !view->change_dir;
		newwin(view);
	}
	if (keycode == PGUP)
	{
		view->can.obj->l[lidx].light_bright += 0.1;
		newwin(view);
	}
	else if (keycode == PGDN)
	{
		view->can.obj->l[lidx].light_bright -= 0.1;
		newwin(view);
	}
}

void	key_hook2(int keycode, t_view *view)
{
	if (view->stop && (keycode == W || keycode == S \
		|| keycode == A || keycode == D))
	{
		view->focus = 1;
		move_focus(0, view, 0.005);
	}
	else if (keycode == M && view->stop)
	{
		view->show_mouse = !view->show_mouse;
		if (view->show_mouse)
			mlx_mouse_hide();
		else
			mlx_mouse_show();
		newwin(view);
		mlx_mouse_move(view->win, view->can.width / 2, view->can.height / 2);
	}
	else if (keycode == PRINT)
		save_image_to_ppm("outfile.ppm", view);
	else if (keycode == MAKE)
		save_image_to_rtfile("outfile.rt", view);
}


int	key_hook(int keycode, t_view *view)
{
	static int	lidx;

	if (!view->stop && keycode != 35 && keycode != 53)
		return (0);
	if (keycode == 125 || keycode == 126 || keycode == 124 || keycode == 123)
		rotate_hook(keycode, view);
	else if (keycode == A || keycode == S || keycode == D || keycode == W)
		move_hook(keycode, view);
	else if (keycode == 24 || keycode == 27)
		up_down(keycode, view);
	else if (keycode == Q_UP || keycode == Q_DOWN || keycode == T || \
	keycode == Q1 || keycode == Q2)
		quality(keycode, view);
	else if (keycode == H)
		view->flag = !view->flag;
	else if (keycode == 35)
		pause_system(view);
	else if (keycode == 53)
		win_destroy(view);
	key_hook2(keycode, view);
	key_hook3(keycode, view, lidx);
	key_hook5(keycode, view, lidx);
	key_hook4(keycode, view, &lidx);
	return (0);
}
