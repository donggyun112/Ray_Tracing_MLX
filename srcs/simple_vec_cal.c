/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_vec_cal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 13:07:58 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/07/27 21:05:11 by seodong-gyu      ###   ########.fr       */
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

t_vec3	multiple_vector(double t, t_vec3 vec)
{
	t_vec3	multiple;

	multiple.x = t * vec.x;
	multiple.y = t * vec.y;
	multiple.z = t * vec.z;
	multiple.size = t * vec.size;
	return (multiple);
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
