/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 14:36:02 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/07/26 04:53:13 by seodong-gyu      ###   ########.fr       */
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
	t_vec3		dir;
	t_vec3		r_norm;
	t_vec3		v_norm;
	t_vec3		left_up;
}	t_camera;

#endif
