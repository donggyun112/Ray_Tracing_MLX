/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 20:59:30 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/07/28 19:32:13 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "struct.h"
# include "vector.h"
# include "../mlx/mlx.h"
# include "../libft/libft.h"

# define RED 0
# define GREEN 1
# define BLUE 2
# define HEIGHT 0
# define WIDTH 1
# define PL 0
# define SP 1
# define CY 2

//utils
t_point3	new_point(double x, double y, double z);

//mlx_utils
int			win_destroy(t_view *view);
int			key_hook(int keycode, t_view *view);

//ray casing
t_ray3		create_ray(t_camera cam, double u, int v);

//hit_sphere
int			discriminant(double a, double b, double c);
double		quad_formula(double a, double b, double c);
double		hit_sphere(t_ray3 ray, t_sphere sphere);

#endif
