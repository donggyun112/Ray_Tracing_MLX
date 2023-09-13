/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_cam.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 22:22:15 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 08:33:11 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	rotate_vertical(int keycode, t_view *view)
{
	if (keycode == 125)
	{
		view->can.cam_dir = add_vector(view->can.cam_dir, \
			multiple_vector(0.09, view->cam.v_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 126)
	{
		view->can.cam_dir = sub_vector(view->can.cam_dir, \
			multiple_vector(0.09, view->cam.v_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
}

void	rotate_horizontal(int keycode, t_view *view)
{
	if (keycode == 123)
	{
		view->can.cam_dir = sub_vector(view->can.cam_dir, \
			multiple_vector(0.1, view->cam.r_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 124)
	{
		view->can.cam_dir = add_vector(view->can.cam_dir, \
			multiple_vector(0.1, view->cam.r_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
}

void	foward_back(int keycode, t_view *view)
{
	if (keycode == S)
	{
		view->can.cam_orig = \
		sub_vector(view->can.cam_orig, multiple_vector(0.5, view->can.cam_dir));
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		if (view->grep.grep == ON && view->click_status)
			move_obj(keycode, view);
		newwin(view);
	}
	else if (keycode == W)
	{
		view->can.cam_orig = \
		add_vector(view->can.cam_orig, multiple_vector(0.5, view->can.cam_dir));
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		if (view->grep.grep == ON && view->click_status)
			move_obj(keycode, view);
		newwin(view);
	}
}

void	left_right(int keycode, t_view *view)
{
	if (keycode == A)
	{
		view->can.cam_orig = \
		sub_vector(view->can.cam_orig, multiple_vector(0.5, view->cam.r_norm));
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		if (view->grep.grep == ON && view->click_status)
			move_obj(keycode, view);
		newwin(view);
	}
	else if (keycode == D)
	{
		view->can.cam_orig = \
		add_vector(view->can.cam_orig, multiple_vector(0.5, view->cam.r_norm));
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		if (view->grep.grep == ON && view->click_status)
			move_obj(keycode, view);
		newwin(view);
	}
}

void	up_down(int keycode, t_view *view)
{
	if (keycode == 24)
	{
		view->can.cam_orig.y += 1;
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 27)
	{
		view->can.cam_orig.y -= 1;
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
}
