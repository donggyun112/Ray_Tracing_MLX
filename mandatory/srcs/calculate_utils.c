/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 00:50:51 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/06 03:14:37 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int	discriminant(double a, double b, double c)
{
	if ((b * b) - (4 * a * c) >= 0.0)
		return (1);
	else
		return (0);
}

double	quad_formula(double a, double b, double c)
{
	double	sol1;
	double	sol2;

	sol1 = ((-1) * b - sqrt(b * b - (4 * a * c))) / (2.0 * a);
	sol2 = ((-1) * b + sqrt(b * b - (4 * a * c))) / (2.0 * a);
	if (sol1 > 0.0 && sol2 > 0.0)
		return (sol1);
	else if (sol1 < 0.0 && sol2 > 0.0)
		return (sol2);
	else
		return (-1.0);
}
