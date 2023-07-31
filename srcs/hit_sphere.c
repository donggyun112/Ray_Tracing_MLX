/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 15:44:00 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/07/31 15:22:29 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

// 판별식
int	discriminant(double a, double b, double c)
{
	if ((b * b) - (4 * a * c) < 0.0)
		return (0);
	else
		return (1);
}

// 2차방정식의 근의 공식
double	quad_formula(double a, double b, double c)
{
	double	sol1;
	double	sol2;

	sol1 = ((-1) * b - sqrt(b * b - (4 * a * c))) / (2 * a);
	sol2 = ((-1) * b + sqrt(b * b - (4 * a * c))) / (2 * a);
	if (sol1 >= 0.0 && sol2 >= 0.0)
		return (sol1);
	else if (sol1 < 0.0 && sol2 >= 0.0)
		return (sol2);
	else
		return (-1.0);
}

// 근의 공식에서 b항을 계산하는 식
double	cal_coe_b(t_vec3 ray_dir, t_vec3 ray_orig, t_vec3 sph_center)
{
	t_vec3	sub;
	double	ret;

	sub = sub_vector(ray_orig, sph_center);
	ret = scalar_product(ray_dir, sub);
	return (2.0 * ret);
}

// 근의 공식에서 c항을 계산하는 식
double	cal_coe_c(t_vec3 ray_orig, t_vec3 sph_center, double rad)
{
	t_vec3	sub;
	double	ret;

	sub = sub_vector(ray_orig, sph_center);
	ret = scalar_product(sub, sub) - (rad * rad);
	return (ret);
}

// ray에 hit 됐을때 t값을 반환하는 함수, hit이 없을경우 -1.0을 반환
double	my_hit_sphere(t_ray3 *ray, t_sphere *sphere)
{
	double	coef[3];

	coef[0] = scalar_product(ray->dir, ray->dir); //a
	coef[1] = cal_coe_b(ray->dir, ray->origin, sphere->center); //b
	coef[2] = cal_coe_c(ray->origin, sphere->center, sphere->radius); //c
	if (discriminant(coef[0], coef[1], coef[2]))
		return (quad_formula(coef[0], coef[1], coef[2]));
	else
		return (-1.0);
}
