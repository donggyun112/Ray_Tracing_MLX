/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_vec_cal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 13:07:58 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/07/19 14:37:08 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vector.h"
#include "../includes/struct.h"

double	size_of_vec(double x, double y, double z)
{
	double	size;

	size = sqrt(x * x + y * y + z * z);
	return (size);
}

void	multiple_vector(double t, t_vec3 *vec)
{
	vec->x = t * vec->x;
	vec->y = t * vec->y;
	vec->z = t * vec->z;
	vec->size = t * vec->size;
}

t_vec3	add_vector(t_vec3 v1, t_vec3 v2)
{
	t_vec3	sum;

	sum.x = v1.x + v2.x;
	sum.y = v1.y + v2.y;
	sum.z = v1.z + v2.z;
	sum.size = size_of_vec(sum.x, sum.y, sum.z);
	return (sum);
}

t_vec3	sub_vector(t_vec3 v1, t_vec3 v2)
{
	t_vec3	sub;

	sub.x = v1.x - v2.x;
	sub.y = v1.y - v2.y;
	sub.z = v1.z - v2.z;
	sub.size = size_of_vec(sub.x, sub.y, sub.z);
	return (sub);
}
