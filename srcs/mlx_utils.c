/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:17:38 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/10 21:59:18 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */

#include "../includes/minirt.h"

void	leaks(void) //remove
{
	system("leaks --list minirt");
}

int	win_destroy(t_view *view)
{
	mlx_destroy_window(view->mlx, view->win);
	// atexit(leaks); //remove
	exit(0);
	return (0);
}

void	newwin(t_view *view)
{
	if (!view->stop)
		return ;
	mlx_clear_window(view->mlx, view->win);
	multi_rend(view);
	mlx_put_image_to_window(view->mlx, view->win, view->img, 0, 0);
	if (!view->show_mouse)
	{
		char *tmp;
		char *tmp2;
		if (view->change_dir)
			tmp = ft_strjoin("Editing Mode : ", "TRUE");
		else
			tmp = ft_strjoin("Editing Mode : ", "FALSE");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 100, 0XFFFFF, tmp);
		free(tmp);
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 130, 0XFFFFF, "remove = F  back = G");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 160, 0XFFFFF, "random create : Z");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 190, 0XFFFFF, "copy : C");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 220, 0XFFFFF, "defult rander : T");
		tmp2 = ft_itoa(view->quality_scalar);
		tmp = ft_strjoin("Current Qulity : ", tmp2);
		free(tmp2);
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 250, 0XFFFFF, tmp);
		free(tmp);
		if (view->grep.type == SP && view->clik_status)
			mlx_string_put(view->mlx, view->win, view->mini_size + 30, 280, 0XFFFFF, "Grep Obj Type: SP");
		else if (view->grep.type == CY && view->clik_status)
			mlx_string_put(view->mlx, view->win, view->mini_size + 30, 280, 0XFFFFF, "Grep Obj Type: CY");
		else if (view->clik_status)
			mlx_string_put(view->mlx, view->win, view->mini_size + 30, 280, 0XFFFFF, "Grep Obj Type: PL");
		else
			mlx_string_put(view->mlx, view->win, view->mini_size + 30, 280, 0XFFFFF, "Grep Obj Type:   ");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 310, 0XFFFFF, "qulity up : 1 | [ ");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 340, 0XFFFFF, "qulity down : 2 | ] ");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 370, 0XFFFFF, "Print ppm : 3");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 400, 0XFFFFF, "Make .rt : 4");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 430, 0XFFFFF, "Pause : p");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 460, 0XFFFFF, "Mouse contol : m");
		tmp2 = ft_itoa(view->lnum);
		tmp = ft_strjoin("lidx : ", tmp2);
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 490, 0XFFFFF, tmp);
		free(tmp);
		free(tmp2);

	}
}

void	foward_back(int keycode, t_view *view)
{
	if (keycode == S)
	{
		view->can.cam_orig = sub_vector(view->can.cam_orig, view->can.cam_dir);
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		if (view->grep.grep == ON && view->clik_status)
			move_obj(keycode, view);
		newwin(view);
	}
	else if (keycode == W)
	{
		view->can.cam_orig = add_vector(view->can.cam_orig, view->can.cam_dir);
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		if (view->grep.grep == ON && view->clik_status)
			move_obj(keycode, view);
		newwin(view);
	}	
}

void	move_obj_vec(t_vec3 *vec, t_vec3 campos, int keycode)
{
	if (keycode == A || keycode == S)
		*vec = sub_vector(*vec, campos);
	else if (keycode == D || keycode == W)
		*vec = add_vector(*vec, campos);
}

void	move_obj(int keycode, t_view *view)
{
	t_sphere	*sp;
	t_cylinder	*cy;
	t_plane		*pl;

	if (view->grep.type == SP)
	{
		sp = (t_sphere *)view->grep.obj;
		if (keycode == A || keycode == D)
			move_obj_vec(&sp->center, view->cam.r_norm, keycode);
		else
			move_obj_vec(&sp->center, view->cam.dir, keycode);			
	}
	else if (view->grep.type == CY)
	{
		cy = (t_cylinder *)view->grep.obj;
		if (keycode == A || keycode == D)
			move_obj_vec(&cy->center, view->cam.r_norm, keycode);
		else
			move_obj_vec(&cy->center, view->cam.dir, keycode);
		make_cylinder_cap2(cy);
	}
	else if (view->grep.type == PL)
	{
		pl = (t_plane *)view->grep.obj;
		if (keycode == A || keycode == D)
			move_obj_vec(&pl->on_plane, view->cam.r_norm, keycode);
		else
			move_obj_vec(&pl->on_plane, view->cam.dir, keycode);
	}
}

void	left_right(int keycode, t_view *view)
{
	if (keycode == A)
	{
		view->can.cam_orig = sub_vector(view->can.cam_orig, view->cam.r_norm);
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		if (view->grep.grep == ON && view->clik_status)
			move_obj(keycode, view);
		newwin(view);
	}
	else if (keycode == D)
	{
		view->can.cam_orig = add_vector(view->can.cam_orig, view->cam.r_norm);
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		if (view->grep.grep == ON && view->clik_status)
			move_obj(keycode, view);
		newwin(view);
	}	
}

void	up_down(int keycode, t_view *view)
{
	if (keycode == 24)
	{
		view->can.cam_orig.y += 1;
		if (view->quality_scalar >= -4)
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 27)
	{
		view->can.cam_orig.y -= 1;
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
}

void	rotate_horizontal(int keycode, t_view *view)
{
	if (keycode == 123)
	{
		view->can.cam_dir = sub_vector(view->can.cam_dir, \
			multiple_vector(0.1, view->cam.r_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 124)
	{
		view->can.cam_dir = add_vector(view->can.cam_dir, \
			multiple_vector(0.1, view->cam.r_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
}

void	rotate_vertical(int keycode, t_view *view)
{
	if (keycode == 125)
	{
		view->can.cam_dir = add_vector(view->can.cam_dir, \
			multiple_vector(0.09, view->cam.v_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 126)
	{
		view->can.cam_dir = sub_vector(view->can.cam_dir, \
			multiple_vector(0.09, view->cam.v_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
}

void	quality(int keycode, t_view *view)
{
	if (keycode == Q_UP)
	{
		if (view->quality_scalar >= 6)
			view->quality_scalar = 6;
		view->quality_scalar += 1;
	}
	else if (keycode == Q_DOWN)
		view->quality_scalar -= 1;
	else if (keycode == T)
		view->quality_scalar = 1;
	else if (keycode == Q2)
		view->quality_scalar = -10;
	if (keycode == Q1)
	{
		view->quality_scalar = 6;
		newwin(view);
		pause_system(view);
	}
	else
		newwin(view);
}

t_vec3	rotate_around_axis(t_vec3 vec, t_vec3 axis, float angle)
{
	const t_vec3	k = norm_vec(axis);
	const float		cos_angle = cos(angle);
	const float		sin_angle = sin(angle);
	t_vec3			rotated_vec;

	rotated_vec.x = vec.x * (cos_angle + (1 - cos_angle) * k.x * k.x) + \
					vec.y * ((1 - cos_angle) * k.x * k.y - sin_angle * k.z) + \
					vec.z * ((1 - cos_angle) * k.x * k.z + sin_angle * k.y);
	rotated_vec.y = vec.x * ((1 - cos_angle) * k.y * k.x + sin_angle * k.z) + \
					vec.y * (cos_angle + (1 - cos_angle) * k.y * k.y) + \
					vec.z * ((1 - cos_angle) * k.y * k.z - sin_angle * k.x);
	rotated_vec.z = vec.x * ((1 - cos_angle) * k.z * k.x - sin_angle * k.y) + \
					vec.y * ((1 - cos_angle) * k.z * k.y + sin_angle * k.x) + \
					vec.z * (cos_angle + (1 - cos_angle) * k.z * k.z);
	return (rotated_vec);
}

void	grep_obj(int x, int y, t_view *view)
{
	float	vp_idx[2];
	t_ray3 ray;

	vp_idx[0] = view->can.ratio * 2.0 * ((float)x) / (float)view->can.width;
	vp_idx[1] = 2.0 * ((float)y) / (float)view->can.height;
	ray = create_ray(view->cam, vp_idx[0], vp_idx[1]);
	ray.type = -1;
	ray.pix[0] = x;
	ray.pix[1] = y;
	intersection(&ray, view->can.obj);
	if (ray.type == SP)
	{
		view->grep.obj = ray.obj;
		view->grep.type = SP;
		view->grep.grep = ON;
	}
	else if (ray.type == CY)
	{
		view->grep.obj = ray.obj;
		view->grep.type = CY;
		view->grep.grep = ON;
	}
	else if (ray.type == PL)
	{
		view->grep.obj = ray.obj;
		view->grep.type = PL;
		view->grep.grep = ON;
	}
	else
		view->grep.grep = OFF;
}

void	make_cylinder_cap2(t_cylinder *cy)
{
	free(cy->ucap);
	free(cy->lcap);
	make_cylinder_cap(cy);
}

void	move_focus(int scalra, t_view *view, float sensitivity)
{
	static int	count;
	int			xy[2];
	float		yaw;
	float		pitch;
	t_vec3		right;

	if (count > scalra)
	{
		mlx_mouse_get_pos(view->win, &xy[0], &xy[1]);
		yaw = (xy[0] - view->can.width / 2) * sensitivity;
		pitch = (xy[1] - view->can.height / 2) * sensitivity;
		right = vector_product(view->can.cam_dir, (t_vec3){0.0f, 1.0f, 0.0f});
		view->can.cam_dir = rotate_around_axis(view->can.cam_dir, \
		(t_vec3){0.0f, 1.0f, 0.0f}, -yaw);
		view->can.cam_dir = \
		rotate_around_axis(view->can.cam_dir, right, -pitch);
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->cam = camera(view->can);
		if (view->clik_status && view->grep.grep == ON)
		{
			t_sphere	*sp;
			t_cylinder	*cy;
			t_plane		*pl;
			if (view->grep.type == SP)
			{
				sp = (t_sphere *)view->grep.obj;
				sp->center = sub_vector(sp->center, view->cam.origin);
				sp->center = rotate_around_axis(sp->center, (t_vec3){0.0f, 1.0f, 0.0f}, -yaw);
				sp->center = rotate_around_axis(sp->center, right, -pitch);
				sp->center = add_vector(sp->center, view->cam.origin);
				
			}
			else if (view->grep.type == CY)
			{
				cy = (t_cylinder *)view->grep.obj;
				if (view->change_dir)
				{
					cy->dir = rotate_around_axis(cy->dir, (t_vec3){0.0f, 1.0f, 0.0f}, yaw * 2);
					cy->dir = rotate_around_axis(cy->dir, right, pitch * 2);
					norm_vec(cy->dir);
				}
				else
				{
					cy->center = sub_vector(cy->center, view->cam.origin);
					cy->center = rotate_around_axis(cy->center, (t_vec3){0.0f, 1.0f, 0.0f}, -yaw);
					cy->center = rotate_around_axis(cy->center, right, -pitch);
					cy->center = add_vector(cy->center, view->cam.origin);
				}
				make_cylinder_cap2(cy);
			}
			else if (view->grep.type == PL)
			{
				pl = (t_plane *)view->grep.obj;
				pl->on_plane = 	sub_vector(pl->on_plane, view->cam.origin);
				pl->on_plane = rotate_around_axis(pl->on_plane, (t_vec3){0.0f, 1.0f, 0.0f}, yaw);
				pl->on_plane = rotate_around_axis(pl->on_plane, right, -pitch);
				pl->on_plane = 	add_vector(pl->on_plane, view->cam.origin);
			}
		}
		newwin(view);
		mlx_mouse_move(view->win, view->can.width / 2, view->can.height / 2);
		count = 0;
	}
	count++;
}
void	pause_system(t_view *view)
{

	view->stop = !view->stop;
	if (view->stop)
	{
		if (!view->show_mouse)
		{
			mlx_mouse_hide();
			view->show_mouse = !view->show_mouse;
		}
		mlx_mouse_move(view->win, view->can.width / 2, view->can.height / 2);
	}
	else
	{
		if (view->show_mouse)
		{
			mlx_mouse_show();
			view->show_mouse = !view->show_mouse;
		}
	}
}

void	move_hook(int keycode, t_view *view)
{
	if (keycode == A || keycode == D)
		left_right(keycode, view);
	else if (keycode == W || keycode == S)
		foward_back(keycode, view);
}

void	rotate_hook(int keycode, t_view *view)
{
	if (keycode == 125 || keycode == 126)
		rotate_vertical(keycode, view);
	else if (keycode == 124 || keycode == 123)
		rotate_horizontal(keycode, view);
}

void	init_ppm(FILE *f, unsigned int tmp, unsigned char color[3])
{
	color[RED] = (tmp >> 16) & 0xFF;
	color[GREEN] = (tmp >> 8) & 0xFF;
	color[BLUE] = tmp & 0xFF;
	fwrite(&color[RED], 1, 1, f);
	fwrite(&color[GREEN], 1, 1, f);
	fwrite(&color[BLUE], 1, 1, f);
}

void save_image_to_ppm(char *filename, t_view *view)
{
	FILE			*f;
	int				xy[2];
	char			*dst;
	unsigned int	tmp;
	unsigned char	color[3];

	f = fopen(filename, "w");
	if (!f)
	{
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		return;
	}
	fprintf(f, "P6\n%d %d\n255\n", view->can.width, view->can.height);
	xy[1] = -1;
	while (++xy[1] < view->can.height)
	{
		xy[0] = -1;
		while (++xy[0] < view->can.width)
		{
			dst = view->addr + (xy[1] * view->line_length + xy[0] * (view->bits_per_pixel / 8));
			tmp = *(unsigned int *)dst;
			init_ppm(f, tmp, color);
		}
	}
	fclose(f);
}

void	write_rt_color(int color[3], FILE *f)
{
	fprintf(f, " %d,%d,%d\n", color[RED], color[GREEN], color[BLUE]);
}
void	write_rt_vec(t_vec3 vec, FILE *f)
{
	fprintf(f, " %f,%f,%f ", vec.x, vec.y, vec.z);
}

void	write_rt_camera(t_view *view, FILE *f)
{
	fprintf(f, "c	");
	write_rt_vec(view->cam.origin, f);
	write_rt_vec(view->cam.dir, f);
	fprintf(f, " %d \n", view->can.fov);
}

void	write_rt_light(t_view *view, FILE *f)
{
	int		i;
	t_light	tmp;

	i = -1;
	while (++i < view->can.obj->rl_cnt)
	{
		fprintf(f, "rl	");	
		write_rt_vec(view->can.obj->rl[i].light->light_orig, f);
		fprintf(f, "%f	", view->can.obj->rl[i].light->light_bright);
		write_rt_vec(view->can.obj->rl[i].r_center, f);
		write_rt_vec(view->can.obj->rl[i].r_axis, f);
		write_rt_color(view->can.obj->rl[i].light->light_col, f);
		view->can.obj->l[view->can.obj->rl[i].light_idx].rotate_idx = -1;
	}
	i = -1;
	while (++i < view->can.obj->l_cnt)
	{
		if (view->can.obj->l->rotate_idx == -1)
			continue ;
		tmp = view->can.obj->l[i];
		fprintf(f, "l	");
		write_rt_vec(tmp.light_orig, f);
		fprintf(f, " %f	", tmp.light_bright);
		write_rt_color(tmp.light_col, f);
	}

}

void	write_rt_sphere(t_view *view, FILE *f)
{
	int	i;

	i = -1;
	while (++i < view->can.obj->rsp_cnt)
	{
		if (view->can.obj->rsp[i].sp->type == NONE)
			continue;
		fprintf(f, "rsp	");
		write_rt_vec(view->can.obj->rsp[i].sp->center, f);
		fprintf(f, " %f ", view->can.obj->rsp[i].sp->radius);
		write_rt_vec(view->can.obj->rsp[i].r_center, f);
		write_rt_vec(view->can.obj->rsp[i].r_axis, f);
		fprintf(f, " %s %s\n", view->can.obj->rsp[i].sp->filepath, \
								view->can.obj->rsp[i].sp->bumppath);
		view->can.obj->rsp[i].sp->type = NONE;
	}
	i = -1;
	while (++i < view->can.obj->sp_cnt)
	{
		if (view->can.obj->sp[i].type == NONE)
			continue ;
		if (view->can.obj->sp[i].type == CSP)
			fprintf(f, "csp	");
		else if (view->can.obj->sp[i].type == TSP)
			fprintf(f, "tsp	");
		else
			fprintf(f, "sp	");
		write_rt_vec(view->can.obj->sp[i].center, f);
		fprintf(f, " %f ", view->can.obj->sp[i].radius);
		if (view->can.obj->sp[i].type == CSP)
			fprintf(f, "\n");
		else if (view->can.obj->sp[i].type == TSP)
			fprintf(f, " %s %s\n", view->can.obj->sp[i].filepath, \
								view->can.obj->sp[i].bumppath);
		else
			write_rt_color(view->can.obj->sp[i].color, f);
	}
}

void	write_rt_cylinder(t_view *view, FILE *f)
{
	int	i;

	i = -1;
	while (++i < view->can.obj->cy_cnt)
	{
		if (view->can.obj->cy[i].type == NONE)
			continue;
		if (view->can.obj->cy[i].type == TCY)
			fprintf(f, "tcy ");
		else if (view->can.obj->cy[i].type == CCY)
			fprintf(f, "ccy ");
		else
			fprintf(f, "cy ");
		write_rt_vec(view->can.obj->cy[i].center, f);
		write_rt_vec(view->can.obj->cy[i].dir, f);
		fprintf(f, "%f ", view->can.obj->cy[i].radius);
		fprintf(f, "%f ", view->can.obj->cy[i].height);
		if (view->can.obj->cy[i].type == TCY)
			fprintf(f, "%s %s\n", view->can.obj->cy[i].filepath, \
								view->can.obj->cy[i].bumppath);
		else if (view->can.obj->cy[i].type == CCY)
			fprintf(f, "\n");
		else
			write_rt_color(view->can.obj->cy[i].color, f);
	}
}

void	write_rt_plane(t_view *view, FILE *f)
{
	int	i;

	i = -1;
	while (++i < view->can.obj->pl_cnt)
	{
		if (view->can.obj->pl[i].type == NONE)
			continue ;
		if (view->can.obj->pl[i].type == TPL)
			fprintf(f, "tpl ");
		else if (view->can.obj->pl[i].type == CPL)
			fprintf(f, "cpl ");
		else
			fprintf(f, "pl ");
		write_rt_vec(view->can.obj->pl[i].on_plane, f);
		write_rt_vec(view->can.obj->pl[i].norm, f);
		if (view->can.obj->pl[i].type == TPL)
			fprintf(f, "%s \n", view->can.obj->pl[i].filepath);
		else if (view->can.obj->pl[i].type == CPL)
			fprintf(f, " \n");
		else
			write_rt_color(view->can.obj->pl[i].color, f);
	}
}

void	save_image_to_rtfile(char *filename, t_view *view)
{
	FILE	*f;

	f = fopen(filename, "w");
	if (!f)
	{
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		return;
	}
	fprintf(f, "R	%d %d\n", view->real_size, view->can.height);
	fprintf(f, "A	%f ", view->can.amb_bright);
	write_rt_color(view->can.amb_col, f);
	write_rt_camera(view, f);
	write_rt_light(view, f);
	write_rt_sphere(view, f);
	write_rt_cylinder(view, f);
	write_rt_plane(view, f);
	if (view->can.bgt_filepath)
		fprintf(f, "bg %s", view->can.bgt_filepath);
	mlx_string_put(view->mlx, view->win, view->can.width/2, view->can.height/2, 0xFF00FF, "Success make .rt file");
}

t_vec3	init_copy_vec(t_vec3 *vec, t_vec3 tar)
{
	vec->x = tar.x + 1;
	vec->y = tar.y + 1;
	vec->z = tar.z + 1;
	return (*vec);
}

float get_float_rand_l(float a, float b)
{
	return (a + (b - a) * (float)rand() / (float)RAND_MAX);
}

void	set_rsp(t_view *view, size_t t, t_sphere *sp, int keycode)
{
	int			i;
	const int	size = view->can.obj->rsp_cnt;
	t_rsphere	rsp;

	i = -1;
	while (++i < size)
		view->can.obj->rsp[i].sp = &view->can.obj->sp[view->can.obj->rsp[i].sp_idx];
	if (sp->rsp_type == 1)
	{
		i = -1;
		while (++i < size)
		{
			if (view->can.obj->rsp[i].sp_idx == sp->idx)
			{
				rsp = view->can.obj->rsp[i];
				rsp.sp_idx = t;
				memcpy(&view->can.obj->rsp[size], &rsp, sizeof(rsp));
				if (keycode == Z)
				{
					view->can.obj->rsp[size].r_axis.x = get_float_rand_l(0.0f, 3.0f);
					view->can.obj->rsp[size].r_axis.y = get_float_rand_l(0.0f, 3.0f);
					view->can.obj->rsp[size].r_axis.z = get_float_rand_l(0.0f, 3.0f);
					view->can.obj->rsp[size].r_axis = norm_vec(view->can.obj->rsp[size].r_axis);
				}
				view->can.obj->rsp[size].sp = &view->can.obj->sp[t];
				view->can.obj->rsp_cnt++;
				break ;
			}
		}
	}
}

void	copy_sphere(t_view *view, int keycode)
{
	t_sphere		sp;
	const size_t	t = view->can.obj->sp_cnt;
	const size_t	y = view->can.obj->rsp_cnt;

	sp = *(t_sphere *)view->grep.obj;
	if (sp.rsp_type == 1)
		view->can.obj->rsp = (t_rsphere *)realloc(view->can.obj->rsp, sizeof(t_rsphere) * (y + 1));
	view->can.obj->sp = (t_sphere *)reallocf(view->can.obj->sp, sizeof(t_sphere) * (t + 1));
	memcpy(&view->can.obj->sp[t], &sp, sizeof(sp));
	if (keycode == Z)
	{
		view->can.obj->sp[t].color[RED] = rand() % 255;
		view->can.obj->sp[t].color[GREEN] = rand() % 255;
		view->can.obj->sp[t].color[BLUE] = rand() % 255;
	}
	view->can.obj->sp[t].idx = t;
	view->can.obj->sp[t].center.x += 1;
	view->can.obj->sp[t].center.y += 1;
	view->can.obj->sp[t].center.z += 1;
	set_rsp(view, t, &sp, keycode);
	view->can.obj->sp_cnt++;
}

void	copy_normal_cylinder(t_view *view, size_t t, int keycode)
{
	if (keycode == Z)
	{
		view->can.obj->cy[t].color[RED] = rand() % 255;
		view->can.obj->cy[t].color[GREEN] = rand() % 255;
		view->can.obj->cy[t].color[BLUE] = rand() % 255;
	}
}

void	push_front(t_backup **backup, void *obj, int type)
{
	t_backup	*tmp;
	t_backup	*head;

	tmp = (t_backup *)malloc(sizeof(t_backup));
	tmp->obj = obj;
	tmp->type = type;
	head = *backup;
	if (head == NULL)
	{
		head = tmp;
		head->next = NULL;
		*backup = head;
	}
	else
	{
		tmp->next = head;
		head = tmp;
		*backup = head;
	}
}

void	copy_cylinder(t_view *view, int keycode)
{
	t_cylinder		cy;
	const size_t	t = view->can.obj->cy_cnt;

	cy = *(t_cylinder *)view->grep.obj;
	view->can.obj->cy = (t_cylinder *)reallocf(view->can.obj->cy, sizeof(t_cylinder) * (t + 1));
	memcpy(&view->can.obj->cy[t], &cy, sizeof(cy));
	view->can.obj->cy[t].center.x += 1;
	view->can.obj->cy[t].center.y += 1;
	view->can.obj->cy[t].center.z += 1;
	if (keycode == Z)
	{
		view->can.obj->cy[t].height = get_float_rand_l(0.5, 3);
		view->can.obj->cy[t].radius = get_float_rand_l(0.5, 3);
		view->can.obj->cy[t].dir.x = get_float_rand_l(-1.0f, 1.0f);
		view->can.obj->cy[t].dir.y = get_float_rand_l(-1.0f, 1.0f);
		view->can.obj->cy[t].dir.z = get_float_rand_l(-1.0f, 1.0f);
		view->can.obj->cy[t].dir = norm_vec(view->can.obj->cy[t].dir);
		view->can.obj->cy[t].type= CY;
	}
	if (view->can.obj->cy[t].type == CY)
		copy_normal_cylinder(view, t, keycode);
	make_cylinder_cap(&view->can.obj->cy[t]);
	view->can.obj->cy_cnt++;
}

void	copy_normal_plane(t_view *view, size_t t, int keycode)
{
	if (keycode == Z)
	{
		view->can.obj->pl[t].type = PL;
		view->can.obj->pl[t].color[RED] = rand() % 255;
		view->can.obj->pl[t].color[GREEN] = rand() % 255;
		view->can.obj->pl[t].color[BLUE] = rand() % 255;
	}
}



void	copy_plane(t_view *view, int keycode)
{
	t_plane			pl;
	const size_t	t = view->can.obj->pl_cnt;
	pl = *(t_plane *)view->grep.obj;
	view->can.obj->pl = (t_plane *)realloc(view->can.obj->pl, sizeof(t_plane) * (t + 1));
	memcpy(&view->can.obj->pl[t], &pl, sizeof(pl));
	copy_normal_plane(view, t, keycode);
	view->can.obj->pl_cnt++;
}

void	obj_copy(t_view *view, int keycode)
{
	t_sphere	*sp;
	t_cylinder	*cy;
	t_plane		*pl;

	if (view->grep.type == SP)
	{
		if (keycode == F)
		{
			sp = (t_sphere *)view->grep.obj;
			push_front(&view->backup, view->grep.obj, sp->type);
			sp->type = NONE;
		}
		else
			copy_sphere(view, keycode);
	}
	else if (view->grep.type == CY)
	{
		if (keycode == F)
		{
			cy = (t_cylinder *)view->grep.obj;
			push_front(&view->backup, view->grep.obj, cy->type);
			cy->type = NONE;
		}
		else
			copy_cylinder(view, keycode);
	}
	else if (view->grep.type == PL)
	{
		if (keycode == F)
		{
			pl = (t_plane *)view->grep.obj;
			push_front(&view->backup, view->grep.obj, pl->type);
			pl->type = NONE;
		}
		else
			copy_plane(view, keycode);
			
	}
	view->grep.grep = OFF;
	newwin(view);
}

int	clear_backup(t_backup **backup)
{
	t_backup	*tmp;
	t_sphere	*sp;
	t_plane		*pl;
	t_cylinder	*cy;

	if (*backup == NULL)
		return (0);
	if ((*backup)->type == SP || (*backup)->type == TSP || (*backup)->type == CSP)
	{
		sp = (t_sphere *)(*backup)->obj;
		sp->type = (*backup)->type;
	}
	else if ((*backup)->type == CY || (*backup)->type == CCY || (*backup)->type == TCY)
	{
		cy = (t_cylinder *)(*backup)->obj;
		cy->type = (*backup)->type;
	}
	else if ((*backup)->type == PL || (*backup)->type == CPL || (*backup)->type == TPL)
	{
		pl = (t_plane *)(*backup)->obj;
		pl->type = (*backup)->type;
	}
	tmp = (*backup)->next;
	free((*backup));
	(*backup) = tmp;
	return (1);
}

int	key_hook(int keycode, t_view *view)
{
	static int	lidx;

	if (keycode == 125 || keycode == 126 || keycode == 124 || keycode == 123)
		rotate_hook(keycode, view);
	else if (keycode == A || keycode == S || keycode == D || keycode == W)
		move_hook(keycode, view);
	else if (keycode == 24 || keycode == 27)
		up_down(keycode, view);
	else if (keycode == Q_UP || keycode == Q_DOWN || keycode == T || keycode == Q1 || \
			keycode == Q2)
		quality(keycode, view);
	else if (keycode == H)
		view->flag = !view->flag;
	else if (keycode == 35)
		pause_system(view);
	else if (keycode == 53)
		win_destroy(view);
	if (view->stop && (keycode == W || keycode == S \
		|| keycode == A || keycode == D))
	{
		view->focus = 1;
		move_focus(0, view, 0.005);
	}
	else if (keycode == M && view->stop)
	{
		view->show_mouse = !view->show_mouse;
		if (view->show_mouse)
			mlx_mouse_hide();
		else
			mlx_mouse_show();
		newwin(view);
		mlx_mouse_move(view->win, view->can.width / 2, view->can.height / 2);
	}
	else if (keycode == PRINT)
		save_image_to_ppm("outfile.ppm", view);
	else if (keycode == MAKE)
		save_image_to_rtfile("outfile.rt", view);
	if (view->grep.grep == ON && view->clik_status)
		if (keycode == C || keycode == Z || keycode == F)
			obj_copy(view, keycode);
	if (keycode == G)
		if (clear_backup(&view->backup))
			newwin(view);
	if (keycode == Q)
	{
		view->change_dir = !view->change_dir;
		newwin(view);
	}
	if (keycode == PGUP)
	{
		view->can.obj->l[lidx].light_bright += 0.1;
		newwin(view);
	}
	else if (keycode == PGDN)
	{
		view->can.obj->l[lidx].light_bright -= 0.1;
		newwin(view);
	}
	else if (keycode == HOME)
	{
		view->can.obj->l[lidx].light_col[BLUE] += 5;
		if (view->can.obj->l[lidx].light_col[BLUE] >= 255)
		{
			view->can.obj->l[lidx].light_col[BLUE] = 255;
			view->can.obj->l[lidx].light_col[GREEN] += 5;
			if (view->can.obj->l[lidx].light_col[GREEN] >= 255)
			{
				view->can.obj->l[lidx].light_col[GREEN] = 255;
				view->can.obj->l[lidx].light_col[RED] += 5;
				if (view->can.obj->l[lidx].light_col[RED] >= 255)
					view->can.obj->l[lidx].light_col[RED] = 255;
			}
		}
		newwin(view);
	}
	else if (keycode == END)
	{
		// view->can.obj->l[0].light_col[BLUE] -= 5;
		// if (view->can.obj->l[0].light_col[BLUE] <= 0)
		// {
		// 	view->can.obj->l[0].light_col[BLUE] = 0;
		// 	view->can.obj->l[0].light_col[GREEN] -= 5;
		// 	if (view->can.obj->l[0].light_col[GREEN] <= 0)
		// 	{
		// 		view->can.obj->l[0].light_col[GREEN] = 0;
		// 		view->can.obj->l[0].light_col[RED] -= 5;
		// 		if (view->can.obj->l[0].light_col[RED] <= 0)
		// 			view->can.obj->l[0].light_col[RED] = 0;
		// 	}
		// }
		view->can.obj->l[lidx].light_col[RED] = rand() % 255;
		view->can.obj->l[lidx].light_col[GREEN] = rand() % 255;
		view->can.obj->l[lidx].light_col[BLUE] = rand() % 255;
		newwin(view);
	}
	if (keycode == NEXT && lidx < view->can.obj->l_cnt - 1)
	{
		lidx++;
		view->lnum = lidx;
		newwin(view);
	}
	else if (keycode == PRIV && lidx > 0)
	{
		lidx--;
		view->lnum = lidx;
		newwin(view);
	}
	// printf("%d\n", keycode);
	return (0);
}

void	my_mlx_pixel_put(t_view *mlx, int x, int y, unsigned int color)
{
	char	*dst;

	dst = mlx->addr + (y * mlx->line_length + x * (mlx->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	rgb_to_int(int color[])
{
	return ((color[RED] << 16) | (color[GREEN] << 8) \
	| color[BLUE]);
}
