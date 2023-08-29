/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 20:59:30 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/29 20:37:34 by jinhyeop         ###   ########.fr       */
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
# define SHADOW 100

//parse
t_canvas	parse(char *av[]);

//utils

//mlx_utils
int			win_destroy(t_view *view);
int			key_hook(int keycode, t_view *view);
void		my_mlx_pixel_put(t_view *mlx, int x, int y, unsigned int color);
int			rgb_to_int(int color[]);
double		my_rand_double(void);
double		my_rand_double_range(double min, double max);
int			my_rand(void);

//intersection
int			discriminant(double a, double b, double c);
double		quad_formula(double a, double b, double c);
void		hit_sphere(t_ray3 *ray, t_sphere *sp, t_canvas canvas);
void		hit_plane(t_ray3 *ray, t_plane *pl, t_canvas canvas);
void		hit_cylinder(t_ray3 *ray, t_cylinder *cy, t_canvas canvas);
void		make_cylinder_cap(t_cylinder *cy);
t_vec3		check_plane_direction(t_plane *pl, t_ray3 *ray);
int			cy_in_range(t_ray3 *ray, double t, t_cylinder *cy);

//raycasting
t_ray3		create_ray(t_camera cam, double u, double v);
t_camera	camera(t_canvas canvas);

//angle
double		cos_sp(t_sphere *sp, t_ray3 *ray, t_canvas canvas);
double		cos_pl(t_plane *pl, t_ray3 *ray, t_canvas canvas);
double		cos_cy(t_cylinder *cy, t_ray3 *ray, t_canvas canvas);
double		ref_sp(t_sphere *sp, t_ray3 *ray, t_canvas canvas);
double		ref_pl(t_plane *pl, t_ray3 *ray, t_canvas canvas);
double		ref_cy(t_cylinder *cy, t_ray3 *ray, t_canvas canvas);

//color
void		ray_color(t_canvas canvas, t_ray3 *ray);

//shadow
int			hit_shadow(t_ray3 *ray, t_canvas canvas);

#endif
