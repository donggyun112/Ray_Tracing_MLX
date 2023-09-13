/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grep_obj.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 22:26:42 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/13 22:29:44 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	def_grep_type(t_view *view, t_ray3 ray)
{
	if (ray.type == SP)
	{
		view->grep.obj = ray.obj;
		view->grep.type = SP;
		view->grep.grep = ON;
	}
	else if (ray.type == CY)
	{
		view->grep.obj = ray.obj;
		view->grep.type = CY;
		view->grep.grep = ON;
	}
	else if (ray.type == PL)
	{
		view->grep.obj = ray.obj;
		view->grep.type = PL;
		view->grep.grep = ON;
	}
	else
		view->grep.grep = OFF;
}

void	grep_obj(int x, int y, t_view *view)
{
	float	vp_idx[2];
	t_ray3	ray;

	vp_idx[0] = view->can.ratio * 2.0 * ((float)x) / (float)view->can.width;
	vp_idx[1] = 2.0 * ((float)y) / (float)view->can.height;
	ray = create_ray(view->cam, vp_idx[0], vp_idx[1]);
	ray.type = -1;
	ray.pix[0] = x;
	ray.pix[1] = y;
	intersection(&ray, view->can.obj);
	def_grep_type(view, ray);
}

int	mouse_press(int button, int x, int y, t_view *view)
{
	(void)x;
	(void)y;
	if (button == 2 || button == 1)
	{
		mlx_mouse_get_pos(view->win, &x, &y);
		grep_obj(x, y, view);
		view->click_status = 1;
	}
	if ((button == 4 || button == 5) && view->click_status)
		zoom_inout(button, view);
	return (0);
}

int	mouse_release(int button, int x, int y, t_view *view)
{
	(void)x;
	(void)y;
	if (button == 2 || button == 1)
		view->click_status = 0;
	return (0);
}
