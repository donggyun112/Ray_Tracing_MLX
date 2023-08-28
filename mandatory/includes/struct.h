/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 14:36:02 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/25 16:14:18 by seodong-gyu      ###   ########.fr       */
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
}	t_vec3;

typedef struct s_aabb
{
	t_vec3	lowerbound;
	t_vec3	upperbound;
}	t_aabb;

typedef struct s_ray3
{
	t_vec3			origin;
	t_vec3			dir;
	double			t;
	int				type;
	void			*obj;
	int				color[3];
}	t_ray3;

typedef struct s_camera
{
	t_vec3	origin;
	t_vec3	left_upper;
	t_vec3	dir;
	t_vec3	r_norm;
	t_vec3	v_norm;
	double	fov;
	double	focal_len;
}	t_camera;

typedef struct s_plane
{
	t_vec3	on_plane;
	t_vec3	norm;
	int		color[3];
}	t_plane;

typedef struct s_sphere
{
	t_aabb			box;
	t_vec3			center;
	double			radius;
	int				color[3];
}	t_sphere;

typedef struct s_cylinder
{
	t_vec3	center;
	t_vec3	dir;
	double	radius;
	double	height;
	int		color[3];
	t_plane	*ucap;
	t_plane	*lcap;
}	t_cylinder;

typedef struct s_volume
{
	int			pl_cnt;
	int			sp_cnt;
	int			cy_cnt;
	t_plane		*pl;
	t_sphere	*sp;
	t_cylinder	*cy;
}	t_volume;

typedef struct s_canvas
{
	int				width;
	int				height;
	double			ratio;
	double			amb_bright;
	int				amb_col[3];
	t_vec3			cam_orig;
	t_vec3			cam_dir;
	int				fov;
	t_vec3			light_orig;
	double			light_bright;
	int				light_col[3];
	t_volume		*obj;
}	t_canvas;

// bvh




#endif
