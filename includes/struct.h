/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 14:36:02 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/07/27 21:35:55 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_view
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_view;


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

typedef struct s_camera
{
	t_point3	origin;
	t_point3	left_lower;
	t_vec3		dir;
	t_vec3		r_norm;
	t_vec3		v_norm;
	double		aspect_ratio;
	double		fov;
	double		focal_len;
	double		vp_height;
	double		vp_width;
}	t_camera;


typedef struct s_screen
{
	double	aspect_ratio;
	int		width;
	int		height;
	double	fov;
}	t_screen;

#endif
