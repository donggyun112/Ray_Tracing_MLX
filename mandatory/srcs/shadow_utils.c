/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 00:59:42 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/06 00:59:48 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	shadow_check(t_ray3 *ray, t_canvas canvas)
{
	int	idx;

	idx = 0;
	while (idx < canvas.obj->sp_cnt)
		shadow_sphere(ray, &canvas.obj->sp[idx++]);
	idx = 0;
	while (idx < canvas.obj->pl_cnt)
		shadow_plane(ray, &canvas.obj->pl[idx++]);
	idx = 0;
	while (idx < canvas.obj->cy_cnt)
		shadow_cylinder(ray, &canvas.obj->cy[idx++]);
}