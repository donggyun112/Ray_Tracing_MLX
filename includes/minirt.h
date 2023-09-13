/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 20:59:30 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 09:27:04 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*   Updated: 2023/09/05 00:02:44 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <signal.h>
# include "struct.h"
# include "vector.h"
# include "../mlx/mlx.h"
# include "../libft/libft.h"

# define DIFF 0
# define REF 1
# define RED 0
# define GREEN 1
# define BLUE 2
# define HEIGHT 0
# define WIDTH 1
# define PL 0
# define SP 1
# define CY 2
# define TSP 3
# define CSP 4
# define CPL 5
# define TPL 6
# define TRI 7
# define CAP 8
# define SHADOW 100
# define TCY 9
# define CCY 10
# define NONE 11
# define NUM_OF_THREAD 7
# define ON 1
# define OFF 0

// mlx_key
# define W 13
# define A 0
# define S 1
# define D 2
# define T 17
# define H 4
# define Q_UP 33
# define Q_DOWN 30
# define Q1 18
# define Q2 19
# define PRINT 20
# define MAKE 21
# define M 46
# define C 8
# define Z 6
# define F 3
# define G 5
# define Q 12
# define E 14
# define PGUP 116
# define PGDN 121
# define HOME 115
# define END 119
# define NEXT 47
# define PRIV 43

//parse
t_canvas	parse(char *av[]);
t_volume	*init_volume(char **av);
void		ft_obj_count(char **av, t_volume *obj);
void		init_count(t_volume *obj, char **tmp);
void		make_obj_cap(t_volume *obj);
void		make_cylinder_cap(t_cylinder *cy);
void		free_split(char **tmp);
int			init_data(char **tmp, t_canvas *canvas);
void		find_problem(char **tmp, int count);
void		find_problem2(char **tmp, int count);
void		error_print(char *tmp, int expected, int input_count);
int			init_light(char **tmp, t_canvas *canvas, int count);
int			init_cylinder(char **tmp, t_canvas *canvas, int count);
void		init_normal_cylinder(char **tmp, t_canvas *canvas, int idx);
void		init_texture_cylinder(char **tmp, \
t_canvas *canvas, int idx, int count);
int			init_sphere(char **tmp, t_canvas *canvas, int count);
void		init_rotate_sphere(t_canvas *canvas, char **tmp, int idx);
void		init_checker_sphere(t_canvas *canvas, char **tmp, int idx);
void		init_texture_sphere(t_canvas *canvas, char **tmp, int idx, \
int count);
void		init_nomal_sphere(t_canvas *canvas, char **tmp, int idx);
int			init_plane(char **tmp, t_canvas *canvas, int count);
void		init_texture_plane(char **tmp, t_canvas *canvas, int idx);
void		init_checker_palne(char **tmp, t_canvas *canvas, int idx);
void		init_nomal_plane(char **tmp, t_canvas *canvas, int idx);
int			argument_count(char **tmp);
int			init_view(char **tmp, t_canvas *canvas, int count);
void		init_view2(t_canvas *canvas, char **tmp);

//utils
int			is_valid_file_type(char *file_path);

//mlx_utils
int			win_destroy(t_view *view);
int			key_hook(int keycode, t_view *view);
void		my_mlx_pixel_put(t_view *mlx, int x, int y, unsigned int color);
int			rgb_to_int(int color[]);
t_vec3		rotate_around_axis(t_vec3 vec, t_vec3 axis, float angle);
void		rotate_vertical(int keycode, t_view *view);
void		rotate_horizontal(int keycode, t_view *view);
void		up_down(int keycode, t_view *view);
void		left_right(int keycode, t_view *view);
void		foward_back(int keycode, t_view *view);
void		quality(int keycode, t_view *view);
void		move_focus(int scalar, t_view *view, float sensitivity);
void		newwin(t_view *view);
void		mlx_engine(t_view *view);
t_vec3		rotate_around_specific_point(t_vec3 vec, \
t_vec3 center, float angle);
void		change_angle(t_view *view);
int			mouse_motion(int x, int y, t_view *view);
int			key_release(int keycode, t_view *view);
void		pause_system(t_view *view);
void		move_obj(int keycode, t_view *view);
void		save_image_to_ppm(char *filename, t_view *img);
int			ft_strcmp(char *s1, char *s2);
void		grep_obj(int x, int y, t_view *view);
void		make_cylinder_cap2(t_cylinder *cy);

//RENDERING
t_color		anti_aliasing(int pix[2], \
float vp_idx[2], t_view *view, t_ray3 *ray);
void		low_quality(int scalar, int pix[2], t_ray3 ray, t_view *view);
void		multi_rend(t_view *view);
void		init_view_scale(t_view *view);
void		set_texture(t_view *view, t_volume *obj);
void		make_image2(void *m);
void		set_thread_st_point(int *anti, int pix[2], t_thread *t);
void		init_background(t_view *view, int pix[2], t_ray3 *ray);
t_thread	*init_thread(t_view *view);

//minirt
void		intersection(t_ray3 *ray, t_volume *obj);

//norminette fix done
//raycasting
t_ray3		create_ray(t_camera cam, float u, float v);
t_camera	camera(t_canvas canvas);

//bumpmap
t_vec3		bump_sphere(t_sphere *sphere, t_texture bp, t_vec3 hit);
t_vec3		bump_cylinder(t_cylinder *cy, t_texture bp, \
			t_vec3 hit, t_vec3 normal);

//diffuse
float		get_hit_height(t_cylinder *cy, t_vec3 hit);
float		cos_sp(t_sphere *sp, t_ray3 *ray, t_canvas canvas, int light);
float		cos_pl(t_plane *pl, t_ray3 *ray, t_canvas canvas, int light);
float		cos_cy(t_cylinder *cy, t_ray3 *ray, t_canvas canvas, int light);

//reflection
float		ref_sp(t_sphere *sp, t_ray3 *ray, t_canvas canvas, int light);
float		ref_pl(t_plane *pl, t_ray3 *ray, t_canvas canvas, int light);
float		ref_cy(t_cylinder *cy, t_ray3 *ray, t_canvas canvas, int light);

//color
int			amb_light(t_canvas canvas, t_ray3 *ray, int idx);
void		ray_color(t_canvas canvas, t_ray3 *ray, int light);

//sphere
void		hit_sphere(t_ray3 *ray, t_sphere *sp);

//sphere_texture
void		spherical_map(t_vec3 p, float *u, float *v, t_sphere *sp);
t_color		image_texture_on_sphere(t_vec3 point, t_sphere *sp, \
			t_texture *texture);
t_color		grid_texture_on_sphere(t_vec3 point, t_checker pattern, \
			t_sphere *sp);

//plane
int			in_triangle(t_vec3 hit, t_plane *pl);
t_vec3		check_plane_direction(t_plane *pl, t_ray3 *ray);
void		hit_plane(t_ray3 *ray, t_plane *pl);

//plane_texture
void		init_pl_color(t_ray3 *ray, t_plane *pl);
void		init_pltexture(t_ray3 *ray, t_plane *pl);

//cylinder
int			cy_in_range(t_ray3 *ray, float t, t_cylinder *cy);
void		hit_cylinder(t_ray3 *ray, t_cylinder *cy);

//cylinder_texture1
void		cylinder_texture(t_ray3 *ray, t_cylinder *cy, float tmp);
void		cap_texture(t_vec3 point, t_cylinder *cy, t_plane *cap, \
			t_ray3 *ray);

//cylinder_texture2
void		cylindrical_map(t_vec3 p, float *u, float *v, t_cylinder *cy);

//texture
void		init_texture(t_texture *texture, t_view *view, char *path);
t_vec3		get_u_v_axis(t_vec3 norm);
t_color		get_texture_color(t_texture texture, float u, float v);
t_color		checkertexture(t_vec3 point, float scale, t_plane *pl);

//shadow
int			hit_shadow(t_ray3 *ray, t_canvas canvas, int light);

//shadow_obj
void		shadow_sphere(t_ray3 *ray, t_sphere *sp);
void		shadow_plane(t_ray3 *ray, t_plane *pl);
void		shadow_cylinder(t_ray3 *ray, t_cylinder *cy);

//utils
int			discriminant(float a, float b, float c);
float		quad_formula(float a, float b, float c);

#endif
