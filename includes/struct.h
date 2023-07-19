/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 14:36:02 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/07/19 21:11:19 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
	double	size;
}	t_vec3;

typedef struct s_point3
{
	double	x;
	double	y;
	double	z;
}	t_point3;

typedef struct s_ray3
{
	t_point3	origin;
	t_vec3		dir;
}	t_ray3;

#endif
