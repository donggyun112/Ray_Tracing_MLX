/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_vec_cal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 13:07:58 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/01 23:00:19 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vector.h"
#include "../includes/struct.h"

float	size_of_vec(float x, float y, float z)
{
	float	size;

	size = sqrt(x * x + y * y + z * z);
	return (size);
}

float	size_of_vec2(t_vec3 vec)
{
	float	size;

	size = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	return (size);
}

t_vec3	multiple_vector(float t, t_vec3 vec)
{
	t_vec3	multiple;

	multiple.x = t * vec.x;
	multiple.y = t * vec.y;
	multiple.z = t * vec.z;
	return (multiple);
}

t_vec3	add_vector(t_vec3 v1, t_vec3 v2)
{
	t_vec3	sum;

	sum.x = v1.x + v2.x;
	sum.y = v1.y + v2.y;
	sum.z = v1.z + v2.z;
	return (sum);
}

t_vec3	sub_vector(t_vec3 v1, t_vec3 v2)
{
	t_vec3	sub;

	sub.x = v1.x - v2.x;
	sub.y = v1.y - v2.y;
	sub.z = v1.z - v2.z;
	return (sub);
}

t_vec3	add_three_vector(t_vec3 v1, t_vec3 v2, t_vec3 v3)
{
	t_vec3	sum;

	sum.x = v1.x + v2.x + v3.x;
	sum.y = v1.y + v2.y + v3.y;
	sum.z = v1.z + v2.z + v3.z;
	return (sum);
}
