/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 20:45:44 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/07/19 21:11:20 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/struct.h"
#include "../includes/minirt.h"

t_point3	new_point(double x, double y, double z)
{
	t_point3	new;

	new.x = x;
	new.y = y;
	new.z = z;
	return (new);
}
