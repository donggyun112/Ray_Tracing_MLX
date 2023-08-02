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


int	init_view(char **tmp, t_canvas *canvas, int count)
{
	if (count == 2 && !ft_strcmp(tmp[0], "R"))
	{
		canvas->width = ft_strtod(tmp[1]);
		canvas->height = ft_strtod(tmp[2]);
		canvas->ratio = (double)canvas->width / (double)canvas->height;
	}
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

int	init_plane(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (count == 9 && !ft_strcmp(tmp[0], "pl"))
	{
		canvas->obj->pl[idx].on_plane.x = ft_strtod(tmp[1]);
		canvas->obj->pl[idx].on_plane.y = ft_strtod(tmp[2]);
		canvas->obj->pl[idx].on_plane.z = ft_strtod(tmp[3]);
		canvas->obj->pl[idx].norm.x = ft_strtod(tmp[4]);
		canvas->obj->pl[idx].norm.y = ft_strtod(tmp[5]);
		canvas->obj->pl[idx].norm.z = ft_strtod(tmp[6]);
		canvas->obj->pl[idx].color[RED] = ft_strtod(tmp[7]);
		canvas->obj->pl[idx].color[GREEN] = ft_strtod(tmp[8]);
		canvas->obj->pl[idx].color[BLUE] = ft_strtod(tmp[9]);
		idx++;
	}
	else
		return (-1);
	return (0);
}

int	init_sphere(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (count == 7 && !ft_strcmp(tmp[0], "sp"))
	{
		canvas->obj->sp[idx].center.x = ft_strtod(tmp[1]);
		canvas->obj->sp[idx].center.y = ft_strtod(tmp[2]);
		canvas->obj->sp[idx].center.z = ft_strtod(tmp[3]);
		canvas->obj->sp[idx].radius = ft_strtod(tmp[4]);
		canvas->obj->sp[idx].color[RED] = ft_strtod(tmp[5]);
		canvas->obj->sp[idx].color[GREEN] = ft_strtod(tmp[6]);
		canvas->obj->sp[idx].color[BLUE] = ft_strtod(tmp[7]);
		idx++;
	}
	else
		return (-1);
	return (0);
}

int	init_cylinder(char **tmp, t_canvas *canvas, int count)
{
	static int	idx;

	if (count == 7 && !ft_strcmp(tmp[0], "cy"))
	{
		canvas->obj->pl[idx].on_plane.x = ft_strtod(tmp[1]);
		canvas->obj->pl[idx].on_plane.y = ft_strtod(tmp[2]);
		canvas->obj->pl[idx].on_plane.z = ft_strtod(tmp[3]);
		canvas->obj->pl[idx].norm.x = ft_strtod(tmp[4]);
		canvas->obj->pl[idx].norm.y = ft_strtod(tmp[5]);
		canvas->obj->pl[idx].norm.z = ft_strtod(tmp[6]);
		idx++;
	}
	else
		return (-1);
	return (0);
}

int	init_light(char **tmp, t_canvas *canvas, int count)
{
	if (!ft_strcmp(tmp[0], "l") && count == 7)
	{
		canvas->light_orig.x = ft_strtod(tmp[1]);
		canvas->light_orig.y = ft_strtod(tmp[2]);
		canvas->light_orig.z = ft_strtod(tmp[3]);
		canvas->light_bright = ft_strtod(tmp[4]);
		canvas->light_col[RED] = ft_strtod(tmp[5]);
		canvas->light_col[GREEN] = ft_strtod(tmp[6]);
		canvas->light_col[BLUE] = ft_strtod(tmp[7]);
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

void	ft_obj_count(char **av, t_volume *volume)
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
		{
			if (!ft_strcmp(tmp[0], "sp"))
				volume->sp_cnt++;
			else if (!ft_strcmp(tmp[0], "pl"))
				volume->pl_cnt++;
			else if (!ft_strcmp(tmp[0], "cy"))
				volume->cy_cnt++;
		}
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
	obj->cy = NULL;
	obj->pl = NULL;
	obj->sp = NULL;
	ft_obj_count(av, obj);
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
