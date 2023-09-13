/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_mouse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:46:25 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 01:47:21 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	move_focus(int scalar, t_view *view, float sensitivity)
{
	static int	count;
	int			xy[2];
	float		yaw;
	float		pitch;
	t_vec3		right;

	if (count > scalar)
	{
		mlx_mouse_get_pos(view->win, &xy[0], &xy[1]);
		yaw = (xy[0] - view->can.width / 2) * sensitivity;
		pitch = (xy[1] - view->can.height / 2) * sensitivity;
		right = vector_product(view->can.cam_dir, (t_vec3){0.0f, 1.0f, 0.0f});
		view->can.cam_dir = rotate_around_axis(view->can.cam_dir, \
		(t_vec3){0.0f, 1.0f, 0.0f}, -yaw);
		view->can.cam_dir = \
		rotate_around_axis(view->can.cam_dir, right, -pitch);
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->cam = camera(view->can);
		if (view->click_status && view->grep.grep == ON)
			move_grep_obj(view, right, pitch, yaw);
		newwin(view);
		mlx_mouse_move(view->win, view->can.width / 2, view->can.height / 2);
		count = 0;
	}
	count++;
}

int	mouse_motion(int x, int y, t_view *view)
{
	static int	pos[2];

	mlx_mouse_get_pos(view->win, &x, &y);
	if ((pos[0] != x || pos[1] != y) && view->stop)
	{
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		if (!view->focus)
			move_focus(2, view, 0.005);
		else
			move_focus(5, view, 0.005);
	}
	pos[0] = x;
	pos[1] = y;
	return (0);
}
