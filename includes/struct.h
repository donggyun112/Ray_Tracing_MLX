/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 14:36:02 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/30 05:12:59 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

typedef struct s_texture
{
	void	*img;
	char	*data;
	int		width;
	int		height;
	int		bpp;
	int		size_line;
	int		endian;
}	t_texture;

typedef struct s_checkerpattern
{
	t_color	color_a;
	t_color	color_b;
	int		width;
	int		height;
}	t_checker;

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

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
	t_vec3			on_plane;
	t_vec3			norm;
	int				type;
	t_texture		texture;
	char			*filepath;
	int				color[3];
}	t_plane;

typedef struct s_sphere
{
	int				type;
	int				rotate;
	float			angle;
	t_texture		texture;
	t_texture		bumtexture;
	char			*filepath;
	char			*bumppath;
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

typedef struct s_light
{
	t_vec3			light_orig;
	double			light_bright;
	int				light_col[3];
}	t_light;

typedef struct s_volume
{
	int			pl_cnt;
	int			sp_cnt;
	int			cy_cnt;
	int			l_cnt;
	float		ag;
	t_plane		*pl;
	t_sphere	*sp;
	t_cylinder	*cy;
	t_light		*l;
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
	t_camera		cam;
}	t_canvas;

typedef struct s_view
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			anti_scalar;
	int			low_scalar;
	int			quality_scalar;
	int			flag;
	t_camera	cam;
	t_canvas	can;
	t_texture	back;
}	t_view;



#endif
