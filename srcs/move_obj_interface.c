/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_obj_interface.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:29:38 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 01:29:54 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

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