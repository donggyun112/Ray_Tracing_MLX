/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 20:41:33 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/07/19 20:51:51 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/struct.h"
#include "../includes/vector.h"

t_ray3	new_ray(t_point3 orig, t_vec3 dir)
{
	t_ray3	new;

	new.origin = orig;
	new.dir = norm_vec(dir);
	return (new);
}

t_point3	ray_position(t_ray3 ray, double t)
{
	t_point3	pos;

	pos.x = ray.origin.x + (t * ray.dir.x);
	pos.y = ray.origin.y + (t * ray.dir.y);
	pos.z = ray.origin.z + (t * ray.dir.z);
	return (pos);
}
