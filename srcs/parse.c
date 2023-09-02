/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 20:24:29 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/03 01:35:46 by seodong-gyu      ###   ########.fr       */
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
	canvas->width = ft_strtod(tmp[1]);
	canvas->height = ft_strtod(tmp[2]);
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
	canvas->obj->pl[idx].norm = norm_vec(canvas->obj->pl[idx].norm);
}

void	init_checker_palne(char **tmp, t_canvas *canvas, int idx)
{
	canvas->obj->pl[idx].type = CPL;
	canvas->obj->pl[idx].on_plane.x = ft_strtod(tmp[1]);
	canvas->obj->pl[idx].on_plane.y = ft_strtod(tmp[2]);
	canvas->obj->pl[idx].on_plane.z = ft_strtod(tmp[3]);
	canvas->obj->pl[idx].norm.x = ft_strtod(tmp[4]);
	canvas->obj->pl[idx].norm.y = ft_strtod(tmp[5]);
	canvas->obj->pl[idx].norm.z = ft_strtod(tmp[6]);
	canvas->obj->pl[idx].norm = norm_vec(canvas->obj->pl[idx].norm);
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
}

int	init_plane(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (count == 9 && !ft_strcmp(tmp[0], "pl"))
		init_nomal_plane(tmp, canvas, idx);
	else if (count == 6 && !ft_strcmp(tmp[0], "cpl"))
		init_checker_palne(tmp, canvas, idx);
	else if ((count == 7 || count == 8) && !ft_strcmp(tmp[0], "tpl"))
		init_texture_plane(tmp, canvas, idx);
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
	canvas->obj->sp[idx].angle = 0.0;
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
	canvas->obj->sp[idx].angle = 0.0;
}

void	init_checker_sphere(t_canvas *canvas, char **tmp, int idx)
{
	canvas->obj->sp[idx].type = CSP;
	canvas->obj->sp[idx].center.x = ft_strtod(tmp[1]);
	canvas->obj->sp[idx].center.y = ft_strtod(tmp[2]);
	canvas->obj->sp[idx].center.z = ft_strtod(tmp[3]);
	canvas->obj->sp[idx].radius = ft_strtod(tmp[4]);
	canvas->obj->sp[idx].angle = 0.0;
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
	else
		return (-1);
	idx++;
	return (0);
}

int	init_cylinder(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (count == 11 && !ft_strcmp(tmp[0], "cy"))
	{
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
		canvas->obj->cy[idx].ucap = NULL;
		canvas->obj->cy[idx].lcap = NULL;
		idx++;
	}
	else
		return (-1);
	return (0);
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
		idx++;
	}
	else
		return (-1);
	return (0);
}

int	init_data(char **tmp, t_canvas *canvas)
{
	int	count;

	count = 0;
	if (!tmp || !*tmp)
		return (1);
	count = argument_count(tmp);
	if (init_view(tmp, canvas, count - 1) == -1)
		if (init_light(tmp, canvas, count - 1) == -1)
			if (init_plane(tmp, canvas, count - 1) == -1)
				if (init_sphere(tmp, canvas, count - 1) == -1)
					if (init_cylinder(tmp, canvas, count - 1) == -1)
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
	if (!ft_strcmp(tmp[0], "sp"))
		obj->sp_cnt++;
	else if (!ft_strcmp(tmp[0], "pl"))
		obj->pl_cnt++;
	else if (!ft_strcmp(tmp[0], "cy"))
		obj->cy_cnt++;
	else if (!ft_strcmp(tmp[0], "l"))
		obj->l_cnt++;
	else if (!ft_strcmp(tmp[0], "tsp"))
		obj->sp_cnt++;
	else if (!ft_strcmp(tmp[0], "csp"))
		obj->sp_cnt++;
	else if (!ft_strcmp(tmp[0], "tpl"))
		obj->pl_cnt++;
	else if (!ft_strcmp(tmp[0], "cpl"))
		obj->pl_cnt++;
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
	obj->cy = NULL;
	obj->pl = NULL;
	obj->sp = NULL;
	ft_obj_count(av, obj);
	obj->l = (t_light *)malloc(sizeof(t_light) * obj->l_cnt);
	obj->sp = (t_sphere *)malloc(sizeof(t_sphere) * obj->sp_cnt);
	obj->pl = (t_plane *)malloc(sizeof(t_plane) * obj->pl_cnt);
	obj->cy = (t_cylinder *)malloc(sizeof(t_cylinder) * obj->cy_cnt);
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
		init_data(tmp, &data);
		free(line);
		free_split(tmp);
	}
	return (data);
}
