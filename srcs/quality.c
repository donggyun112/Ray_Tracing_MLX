/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quality.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:31:01 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 01:33:22 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	zomm_in(t_view *view)
{
	t_sphere	*sp;
	t_cylinder	*cy;

	if (view->grep.type == SP)
	{
		sp = (t_sphere *)view->grep.obj;
		sp->radius += 0.2;
	}
	else if (view->grep.type == CY)
	{
		cy = (t_cylinder *)view->grep.obj;
		cy->height -= 0.5;
		cy->radius -= 0.2;
		make_cylinder_cap2(cy);
	}
}

void	zoom_out(t_view *view)
{
	t_sphere	*sp;
	t_cylinder	*cy;

	if (view->grep.type == SP)
	{
		sp = (t_sphere *)view->grep.obj;
		sp->radius -= 0.2;
	}
	else if (view->grep.type == CY)
	{
		cy = (t_cylinder *)view->grep.obj;
		cy->radius += 0.2;
		cy->height += 0.5;
		make_cylinder_cap2(cy);
	}
}

void	zoom_inout(int button, t_view *view)
{
	if (button == 4)
		zomm_in(view);
	else
		zoom_out(view);
	newwin(view);
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