/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 20:24:29 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 01:34:19 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 || *s2)
	{
		if (*s1 != *s2)
			return (1);
		s1++;
		s2++;
	}
	return (0);
}

void	init_view2(t_canvas *canvas, char **tmp)
{
	canvas->width = fabs(ft_strtod(tmp[1]));
	canvas->height = fabs(ft_strtod(tmp[2]));
	canvas->ratio = (float)canvas->width / (float)canvas->height;
}

int	init_view(char **tmp, t_canvas *canvas, int count)
{
	if (count == 2 && !ft_strcmp(tmp[0], "R"))
		init_view2(canvas, tmp);
	else if (count == 1 && !ft_strcmp(tmp[0], "bg"))
		canvas->bgt_filepath = ft_strdup(tmp[1]);
	else if (count == 4 && !ft_strcmp(tmp[0], "A"))
	{
		canvas->amb_bright = ft_strtod(tmp[1]);
		canvas->amb_col[RED] = ft_strtod(tmp[2]);
		canvas->amb_col[GREEN] = ft_strtod(tmp[3]);
		canvas->amb_col[BLUE] = ft_strtod(tmp[4]);
	}
	else if (count == 7 && !ft_strcmp(tmp[0], "c"))
	{
		canvas->cam_orig.x = ft_strtod(tmp[1]);
		canvas->cam_orig.y = ft_strtod(tmp[2]);
		canvas->cam_orig.z = ft_strtod(tmp[3]);
		canvas->cam_dir.x = ft_strtod(tmp[4]);
		canvas->cam_dir.y = ft_strtod(tmp[5]);
		canvas->cam_dir.z = ft_strtod(tmp[6]);
		canvas->fov = ft_strtod(tmp[7]);
		canvas->cam_dir = norm_vec(canvas->cam_dir);
	}
	else
		return (-1);
	return (0);
}

int	argument_count(char **tmp)
{
	int	i;

	i = 0;
	while (tmp[i])
		i++;
	return (i);
}

void	init_nomal_plane(char **tmp, t_canvas *canvas, int idx)
{
	canvas->obj->pl[idx].type = PL;
	canvas->obj->pl[idx].on_plane.x = ft_strtod(tmp[1]);
	canvas->obj->pl[idx].on_plane.y = ft_strtod(tmp[2]);
	canvas->obj->pl[idx].on_plane.z = ft_strtod(tmp[3]);
	canvas->obj->pl[idx].norm.x = ft_strtod(tmp[4]);
	canvas->obj->pl[idx].norm.y = ft_strtod(tmp[5]);
	canvas->obj->pl[idx].norm.z = ft_strtod(tmp[6]);
	canvas->obj->pl[idx].color[RED] = ft_strtod(tmp[7]);
	canvas->obj->pl[idx].color[GREEN] = ft_strtod(tmp[8]);
	canvas->obj->pl[idx].color[BLUE] = ft_strtod(tmp[9]);
	canvas->obj->pl[idx].tr_v1 = (t_vec3){0.0, 0.0, 0.0};
	canvas->obj->pl[idx].tr_v2 = (t_vec3){0.0, 0.0, 0.0};
	canvas->obj->pl[idx].norm = norm_vec(canvas->obj->pl[idx].norm);
}

void	init_checker_plane(char **tmp, t_canvas *canvas, int idx)
{
	canvas->obj->pl[idx].type = CPL;
	canvas->obj->pl[idx].on_plane.x = ft_strtod(tmp[1]);
	canvas->obj->pl[idx].on_plane.y = ft_strtod(tmp[2]);
	canvas->obj->pl[idx].on_plane.z = ft_strtod(tmp[3]);
	canvas->obj->pl[idx].norm.x = ft_strtod(tmp[4]);
	canvas->obj->pl[idx].norm.y = ft_strtod(tmp[5]);
	canvas->obj->pl[idx].norm.z = ft_strtod(tmp[6]);
	canvas->obj->pl[idx].norm = norm_vec(canvas->obj->pl[idx].norm);
	canvas->obj->pl[idx].tr_v1 = (t_vec3){0.0, 0.0, 0.0};
	canvas->obj->pl[idx].tr_v2 = (t_vec3){0.0, 0.0, 0.0};
}

void	init_texture_plane(char **tmp, t_canvas *canvas, int idx)
{
	canvas->obj->pl[idx].type = TPL;
	canvas->obj->pl[idx].on_plane.x = ft_strtod(tmp[1]);
	canvas->obj->pl[idx].on_plane.y = ft_strtod(tmp[2]);
	canvas->obj->pl[idx].on_plane.z = ft_strtod(tmp[3]);
	canvas->obj->pl[idx].norm.x = ft_strtod(tmp[4]);
	canvas->obj->pl[idx].norm.y = ft_strtod(tmp[5]);
	canvas->obj->pl[idx].norm.z = ft_strtod(tmp[6]);
	canvas->obj->pl[idx].filepath = ft_strdup(tmp[7]);
	canvas->obj->pl[idx].norm = norm_vec(canvas->obj->pl[idx].norm);
	canvas->obj->pl[idx].tr_v1 = (t_vec3){0.0, 0.0, 0.0};
	canvas->obj->pl[idx].tr_v2 = (t_vec3){0.0, 0.0, 0.0};
}

void	init_triangle_plane(char **tmp, t_canvas *canvas, int idx)
{
	canvas->obj->pl[idx].type = TRI;
	canvas->obj->pl[idx].on_plane.x = ft_strtod(tmp[1]);
	canvas->obj->pl[idx].on_plane.y = ft_strtod(tmp[2]);
	canvas->obj->pl[idx].on_plane.z = ft_strtod(tmp[3]);
	canvas->obj->pl[idx].tr_v1.x = ft_strtod(tmp[4]) - ft_strtod(tmp[1]);
	canvas->obj->pl[idx].tr_v1.y = ft_strtod(tmp[5]) - ft_strtod(tmp[2]);
	canvas->obj->pl[idx].tr_v1.z = ft_strtod(tmp[6]) - ft_strtod(tmp[3]);
	canvas->obj->pl[idx].tr_v2.x = ft_strtod(tmp[7]) - ft_strtod(tmp[1]);
	canvas->obj->pl[idx].tr_v2.y = ft_strtod(tmp[8]) - ft_strtod(tmp[2]);
	canvas->obj->pl[idx].tr_v2.z = ft_strtod(tmp[9]) - ft_strtod(tmp[3]);
	canvas->obj->pl[idx].color[RED] = ft_strtod(tmp[10]);
	canvas->obj->pl[idx].color[GREEN] = ft_strtod(tmp[11]);
	canvas->obj->pl[idx].color[BLUE] = ft_strtod(tmp[12]);
	canvas->obj->pl[idx].norm = norm_vec(vector_product(\
		canvas->obj->pl[idx].tr_v1, canvas->obj->pl[idx].tr_v2));
	canvas->obj->pl[idx].filepath = NULL;
}

int	init_plane(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (count == 9 && !ft_strcmp(tmp[0], "pl"))
		init_nomal_plane(tmp, canvas, idx);
	else if (count == 6 && !ft_strcmp(tmp[0], "cpl"))
		init_checker_plane(tmp, canvas, idx);
	else if ((count == 7 || count == 8) && !ft_strcmp(tmp[0], "tpl"))
		init_texture_plane(tmp, canvas, idx);
	else if (count == 12 && !ft_strcmp(tmp[0], "tr"))
		init_triangle_plane(tmp, canvas, idx);
	else
		return (-1);
	idx++;
	return (0);
}

void	init_nomal_sphere(t_canvas *canvas, char **tmp, int idx)
{
	canvas->obj->sp[idx].type = SP;
	canvas->obj->sp[idx].center.x = ft_strtod(tmp[1]);
	canvas->obj->sp[idx].center.y = ft_strtod(tmp[2]);
	canvas->obj->sp[idx].center.z = ft_strtod(tmp[3]);
	canvas->obj->sp[idx].radius = ft_strtod(tmp[4]);
	canvas->obj->sp[idx].color[RED] = ft_strtod(tmp[5]);
	canvas->obj->sp[idx].color[GREEN] = ft_strtod(tmp[6]);
	canvas->obj->sp[idx].color[BLUE] = ft_strtod(tmp[7]);
	canvas->obj->sp[idx].angle = 0.000000;
	canvas->obj->sp[idx].rsp_type = 0;
	canvas->obj->sp[idx].idx = idx;
}

void	init_texture_sphere(t_canvas *canvas, char **tmp, int idx, int count)
{
	canvas->obj->sp[idx].type = TSP;
	canvas->obj->sp[idx].center.x = ft_strtod(tmp[1]);
	canvas->obj->sp[idx].center.y = ft_strtod(tmp[2]);
	canvas->obj->sp[idx].center.z = ft_strtod(tmp[3]);
	canvas->obj->sp[idx].radius = ft_strtod(tmp[4]);
	canvas->obj->sp[idx].filepath = ft_strdup(tmp[5]);
	if (count == 6)
		canvas->obj->sp[idx].bumppath = ft_strdup(tmp[6]);
	else
		canvas->obj->sp[idx].bumppath = NULL;
	canvas->obj->sp[idx].angle = 0.000000;
	canvas->obj->sp[idx].rsp_type = 0;
	canvas->obj->sp[idx].idx = idx;
}

void	init_checker_sphere(t_canvas *canvas, char **tmp, int idx)
{
	canvas->obj->sp[idx].type = CSP;
	canvas->obj->sp[idx].center.x = ft_strtod(tmp[1]);
	canvas->obj->sp[idx].center.y = ft_strtod(tmp[2]);
	canvas->obj->sp[idx].center.z = ft_strtod(tmp[3]);
	canvas->obj->sp[idx].radius = ft_strtod(tmp[4]);
	canvas->obj->sp[idx].angle = 0.000000;
	canvas->obj->sp[idx].rsp_type = SP;
	canvas->obj->sp[idx].idx = idx;
}

void	init_rotate_sphere(t_canvas *canvas, char **tmp, int idx)
{
	static int	ridx;

	init_checker_sphere(canvas, tmp, idx);
	canvas->obj->sp[idx].type = TSP;
	canvas->obj->rsp[ridx].r_center.x = ft_strtod(tmp[5]);
	canvas->obj->rsp[ridx].r_center.y = ft_strtod(tmp[6]);
	canvas->obj->rsp[ridx].r_center.z = ft_strtod(tmp[7]);
	canvas->obj->rsp[ridx].r_axis.x = ft_strtod(tmp[8]);
	canvas->obj->rsp[ridx].r_axis.y = ft_strtod(tmp[9]);
	canvas->obj->rsp[ridx].r_axis.z = ft_strtod(tmp[10]);
	canvas->obj->sp[idx].filepath = ft_strdup(tmp[11]);
	canvas->obj->sp[idx].bumppath = ft_strdup(tmp[12]);
	canvas->obj->rsp[ridx].sp = &canvas->obj->sp[idx];
	canvas->obj->rsp[ridx].r_axis = norm_vec(canvas->obj->rsp[ridx].r_axis);
	canvas->obj->rsp[ridx].r_radius = \
	size_of_vec2(sub_vector(canvas->obj->sp[idx].center, \
	canvas->obj->rsp[ridx].r_center));
	canvas->obj->sp[idx].angle = 0.000000;
	canvas->obj->rsp[ridx].sp_idx = idx;
	canvas->obj->sp[idx].rsp_type = 1;
	canvas->obj->sp[idx].idx = idx;
	ridx++;
}

int	init_sphere(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (count == 7 && !ft_strcmp(tmp[0], "sp"))
		init_nomal_sphere(canvas, tmp, idx);
	else if ((count == 5 || count == 6) && !ft_strcmp(tmp[0], "tsp"))
		init_texture_sphere(canvas, tmp, idx, count);
	else if (count == 4 && !ft_strcmp(tmp[0], "csp"))
		init_checker_sphere(canvas, tmp, idx);
	else if (!ft_strcmp(tmp[0], "rsp") && count == 12)
		init_rotate_sphere(canvas, tmp, idx);
	else
		return (-1);
	idx++;
	return (0);
}

void	init_texture_cylinder(char **tmp, t_canvas *canvas, int idx, int count)
{
	canvas->obj->cy[idx].type = TCY;
	canvas->obj->cy[idx].center.x = ft_strtod(tmp[1]);
	canvas->obj->cy[idx].center.y = ft_strtod(tmp[2]);
	canvas->obj->cy[idx].center.z = ft_strtod(tmp[3]);
	canvas->obj->cy[idx].dir.x = ft_strtod(tmp[4]);
	canvas->obj->cy[idx].dir.y = ft_strtod(tmp[5]);
	canvas->obj->cy[idx].dir.z = ft_strtod(tmp[6]);
	canvas->obj->cy[idx].radius = ft_strtod(tmp[7]);
	canvas->obj->cy[idx].height = ft_strtod(tmp[8]);
	canvas->obj->cy[idx].filepath = ft_strdup(tmp[9]);
	if (count == 10)
		canvas->obj->cy[idx].bumppath = ft_strdup(tmp[9]);
	else
		canvas->obj->cy[idx].bumppath = NULL;
}

void	init_normal_cylinder(char **tmp, t_canvas *canvas, int idx)
{
	canvas->obj->cy[idx].type = CY;
	canvas->obj->cy[idx].center.x = ft_strtod(tmp[1]);
	canvas->obj->cy[idx].center.y = ft_strtod(tmp[2]);
	canvas->obj->cy[idx].center.z = ft_strtod(tmp[3]);
	canvas->obj->cy[idx].dir.x = ft_strtod(tmp[4]);
	canvas->obj->cy[idx].dir.y = ft_strtod(tmp[5]);
	canvas->obj->cy[idx].dir.z = ft_strtod(tmp[6]);
	canvas->obj->cy[idx].radius = ft_strtod(tmp[7]);
	canvas->obj->cy[idx].height = ft_strtod(tmp[8]);
	canvas->obj->cy[idx].color[RED] = ft_strtod(tmp[9]);
	canvas->obj->cy[idx].color[GREEN] = ft_strtod(tmp[10]);
	canvas->obj->cy[idx].color[BLUE] = ft_strtod(tmp[11]);
	canvas->obj->cy[idx].dir = norm_vec(canvas->obj->cy[idx].dir);
}

void	init_checker_cylinder(char **tmp, t_canvas *canvas, int idx)
{
	canvas->obj->cy[idx].type = CCY;
	canvas->obj->cy[idx].center.x = ft_strtod(tmp[1]);
	canvas->obj->cy[idx].center.y = ft_strtod(tmp[2]);
	canvas->obj->cy[idx].center.z = ft_strtod(tmp[3]);
	canvas->obj->cy[idx].dir.x = ft_strtod(tmp[4]);
	canvas->obj->cy[idx].dir.y = ft_strtod(tmp[5]);
	canvas->obj->cy[idx].dir.z = ft_strtod(tmp[6]);
	canvas->obj->cy[idx].radius = ft_strtod(tmp[7]);
	canvas->obj->cy[idx].height = ft_strtod(tmp[8]);
}

int	init_cylinder(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (count == 11 && !ft_strcmp(tmp[0], "cy"))
		init_normal_cylinder(tmp, canvas, idx);
	else if (count == 8 && !ft_strcmp(tmp[0], "ccy"))
		init_checker_cylinder(tmp, canvas, idx);
	else if ((count == 9 || count == 10) && !ft_strcmp(tmp[0], "tcy"))
		init_texture_cylinder(tmp, canvas, idx, count);
	else
		return (-1);
	canvas->obj->cy[idx].dir = norm_vec(canvas->obj->cy[idx].dir);
	canvas->obj->cy[idx].ucap = NULL;
	canvas->obj->cy[idx].lcap = NULL;
	canvas->obj->cy[idx].angle = 0.0;
	idx++;
	return (0);
}

void	init_rlight(char **tmp, t_canvas *canvas, int idx)
{
	static int	ridx;

	canvas->obj->l[idx].light_orig.x = ft_strtod(tmp[1]);
	canvas->obj->l[idx].light_orig.y = ft_strtod(tmp[2]);
	canvas->obj->l[idx].light_orig.z = ft_strtod(tmp[3]);
	canvas->obj->l[idx].light_bright = ft_strtod(tmp[4]);
	canvas->obj->rl[ridx].r_center.x = ft_strtod(tmp[5]);
	canvas->obj->rl[ridx].r_center.y = ft_strtod(tmp[6]);
	canvas->obj->rl[ridx].r_center.z = ft_strtod(tmp[7]);
	canvas->obj->rl[ridx].r_axis.x = ft_strtod(tmp[8]);
	canvas->obj->rl[ridx].r_axis.y = ft_strtod(tmp[9]);
	canvas->obj->rl[ridx].r_axis.z = ft_strtod(tmp[10]);
	canvas->obj->l[idx].light_col[RED] = ft_strtod(tmp[11]);
	canvas->obj->l[idx].light_col[GREEN] = ft_strtod(tmp[12]);
	canvas->obj->l[idx].light_col[BLUE] = ft_strtod(tmp[13]);
	canvas->obj->rl[ridx].light_idx = idx;
	canvas->obj->rl[ridx].light = &canvas->obj->l[idx];
	ridx++;
}

int	init_light(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (!ft_strcmp(tmp[0], "l") && count == 7)
	{
		canvas->obj->l[idx].light_orig.x = ft_strtod(tmp[1]);
		canvas->obj->l[idx].light_orig.y = ft_strtod(tmp[2]);
		canvas->obj->l[idx].light_orig.z = ft_strtod(tmp[3]);
		canvas->obj->l[idx].light_bright = ft_strtod(tmp[4]);
		canvas->obj->l[idx].light_col[RED] = ft_strtod(tmp[5]);
		canvas->obj->l[idx].light_col[GREEN] = ft_strtod(tmp[6]);
		canvas->obj->l[idx].light_col[BLUE] = ft_strtod(tmp[7]);
	}
	else if (!ft_strcmp(tmp[0], "rl") && count == 13)
		init_rlight(tmp, canvas, idx);
	else
		return (-1);
	idx++;
	return (0);
}

void	error_print(char *tmp, int expected, int input_count)
{
	fprintf(stderr, \
	"Error: [%s] Expected \
%d arguments, but got %d. Please check your input.\n"\
	, tmp, expected, input_count);
	exit(1);
}

void	find_problem2(char **tmp, int count)
{
	if (count != 7 && (!ft_strcmp(tmp[0], "c") || !ft_strcmp(tmp[0], "tpl")))
		error_print(tmp[0], 7, count);
	else if (count != 4 && !ft_strcmp(tmp[0], "A"))
		error_print(tmp[0], 4, count);
	else if (count != 1 && !ft_strcmp(tmp[0], "bg"))
		error_print(tmp[0], 1, count);
}

void	find_problem(char **tmp, int count)
{
	if (!ft_strcmp(tmp[0], "l") && count != 7)
		error_print(tmp[0], 7, count);
	else if (count != 11 && !ft_strcmp(tmp[0], "cy"))
		error_print(tmp[0], 11, count);
	else if (count != 8 && !ft_strcmp(tmp[0], "ccy"))
		error_print(tmp[0], 8, count);
	else if (count != 10 && !ft_strcmp(tmp[0], "tcy"))
		error_print(tmp[0], 10, count);
	if (count != 7 && !ft_strcmp(tmp[0], "sp"))
		error_print(tmp[0], 7, count);
	else if (count != 6 && !ft_strcmp(tmp[0], "tsp"))
		error_print(tmp[0], 6, count);
	else if (count != 4 && !ft_strcmp(tmp[0], "csp"))
		error_print(tmp[0], 4, count);
	else if (!ft_strcmp(tmp[0], "rsp") && count != 12)
		error_print(tmp[0], 12, count);
	else if (count != 9 && !ft_strcmp(tmp[0], "pl"))
		error_print(tmp[0], 9, count);
	else if (count != 6 && !ft_strcmp(tmp[0], "cpl"))
		error_print(tmp[0], 6, count);
	else if (count != 2 && !ft_strcmp(tmp[0], "R"))
		error_print(tmp[0], 2, count);
	find_problem2(tmp, count);
}

int	is_undefine_obj(char **tmp)
{
	if (**tmp == '#')
		return (0);
	return (-1);
}

int	init_data(char **tmp, t_canvas *canvas)
{
	int			count;

	count = 0;
	if (!tmp || !*tmp)
		return (1);
	count = argument_count(tmp);
	find_problem(tmp, count - 1);
	if (init_view(tmp, canvas, count - 1) == -1)
		if (init_light(tmp, canvas, count - 1) == -1)
			if (init_plane(tmp, canvas, count - 1) == -1)
				if (init_sphere(tmp, canvas, count - 1) == -1)
					if (init_cylinder(tmp, canvas, count - 1) == -1)
						if (is_undefine_obj(tmp) == -1)
							return (-1);
	return (0);
}

void	free_split(char **tmp)
{
	int	i;

	i = -1;
	if (!tmp)
		return ;
	while (tmp[++i])
		free(tmp[i]);
	free(tmp);
}

void	init_count(t_volume *obj, char **tmp)
{
	if (!ft_strcmp(tmp[0], "rsp"))
		obj->rsp_cnt++;
	if (!ft_strcmp(tmp[0], "rl"))
		obj->rl_cnt++;
	if (!ft_strcmp(tmp[0], "sp") || !ft_strcmp(tmp[0], "tsp") || \
	!ft_strcmp(tmp[0], "csp") || !ft_strcmp(tmp[0], "rsp"))
		obj->sp_cnt++;
	else if (!ft_strcmp(tmp[0], "cy") || !ft_strcmp(tmp[0], "ccy") || \
	!ft_strcmp(tmp[0], "tcy"))
		obj->cy_cnt++;
	else if (!ft_strcmp(tmp[0], "pl") || !ft_strcmp(tmp[0], "tr"))
		obj->pl_cnt++;
	else if (!ft_strcmp(tmp[0], "l") || !ft_strcmp(tmp[0], "rl"))
		obj->l_cnt++;
	else if (!ft_strcmp(tmp[0], "tpl"))
		obj->pl_cnt++;
	else if (!ft_strcmp(tmp[0], "cpl"))
		obj->pl_cnt++;
	else if (!ft_strcmp(tmp[0], "R"))
		obj->error_flag[0] = 1;
	else if (!ft_strcmp(tmp[0], "A"))
		obj->error_flag[1] = 1;
	else if (!ft_strcmp(tmp[0], "c"))
		obj->error_flag[2] = 1;
}

void	ft_obj_count(char **av, t_volume *obj)
{
	int		fd;
	char	*line;
	char	**tmp;

	fd = open(av[1], O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (!line || !*line)
			break ;
		tmp = ft_split(line, " \t\n,");
		if (tmp && *tmp)
			init_count(obj, tmp);
		free_split(tmp);
		free(line);
	}
	if (obj->error_flag[0] == 0 || \
	obj->error_flag[1] == 0 || obj->error_flag[2] == 0)
	{
		fprintf(stderr, "Error: Missing required argument. [ R, A, c ]\n");
		exit(1);
	}
	close(fd);
}

t_volume	*init_volume(char **av)
{
	t_volume	*obj;

	obj = (t_volume *)malloc(sizeof(t_volume));
	obj->cy_cnt = 0;
	obj->pl_cnt = 0;
	obj->sp_cnt = 0;
	obj->l_cnt = 0;
	obj->rsp_cnt = 0;
	obj->rl_cnt = 0;
	obj->error_flag[0] = 0;
	obj->error_flag[1] = 0;
	obj->error_flag[2] = 0;
	obj->cy = NULL;
	obj->pl = NULL;
	obj->sp = NULL;
	ft_obj_count(av, obj);
	obj->l = (t_light *)malloc(sizeof(t_light) * obj->l_cnt);
	obj->sp = (t_sphere *)malloc(sizeof(t_sphere) * obj->sp_cnt);
	obj->pl = (t_plane *)malloc(sizeof(t_plane) * obj->pl_cnt);
	obj->cy = (t_cylinder *)malloc(sizeof(t_cylinder) * obj->cy_cnt);
	obj->rsp = (t_rsphere *)malloc(sizeof(t_rsphere) * obj->rsp_cnt);
	obj->rl = (t_rlight *)malloc(sizeof(t_rlight) * obj->rl_cnt);
	return (obj);
}

t_canvas	parse(char *av[])
{
	t_canvas	data;
	t_volume	*obj;
	int			fd;
	char		*line;
	char		**tmp;

	obj = init_volume(av);
	data.obj = obj;
	data.bgt_filepath = NULL;
	fd = open(av[1], O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (!line || !*line)
			break ;
		tmp = ft_split(line, " \t\n,");
		if (init_data(tmp, &data) == -1)
			error_print(*tmp, -1, -1);
		free(line);
		free_split(tmp);
	}
	return (data);
}
