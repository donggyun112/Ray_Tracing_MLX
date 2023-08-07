/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 20:59:30 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/07 19:01:46 by dongkseo         ###   ########.fr       */
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
double 		my_rand_double();
double		my_rand_double_range(double min, double max);
int 		my_rand();

//intersection
void		hit_sphere(t_ray3 *ray, t_sphere *sp, t_canvas canvas);
void		hit_plane(t_ray3 *ray, t_plane *pl, t_canvas canvas);

//raycasting
t_ray3		create_ray(t_camera cam, double u, double v);
t_camera	camera(t_canvas canvas);

//angle
double		cos_sp(t_sphere *sp, t_ray3 *ray, t_canvas canvas);
double		cos_pl(t_plane *pl, t_ray3 *ray, t_canvas canvas);
double		ref_sp(t_sphere *sp, t_ray3 *ray, t_canvas canvas);
double		ref_pl(t_plane *pl, t_ray3 *ray, t_canvas canvas);

//color
void		ray_color(t_canvas canvas, t_ray3 *ray);

//bvh
int		hit(t_ray3 ray, double t_min, double t_max, t_aabb box);
void	bounding_sphere(t_sphere *sp);


#endif
