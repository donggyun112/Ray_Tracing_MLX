#include "../includes/minirt.h"


void	init_data(char **tmp, t_canvas *canvas, int num)
{
	if (num == 0)
	{
		canvas->width = ft_strtod(tmp[1]);
		canvas->height = ft_strtod(tmp[2]);
	}
	else if (num == 1)
	{
		canvas->amb_bright = ft_strtod(tmp[1]);
		canvas->amb_col[RED] = ft_strtod(tmp[2]);
		canvas->amb_col[GREEN] = ft_strtod(tmp[2]);
		canvas->amb_col[BLUE] = ft_strtod(tmp[2]);
	}
	else if (num == 2)
	{
		canvas->cam_orig.x = ft_strtod(tmp[1]);
		canvas->cam_orig.y = ft_strtod(tmp[2]);
		canvas->cam_orig.z = ft_strtod(tmp[3]);
		canvas->cam_dir.x = ft_strtod(tmp[4]);
		canvas->cam_dir.y = ft_strtod(tmp[5]);
		canvas->cam_dir.z = ft_strtod(tmp[6]);
		canvas->fov = ft_strtod(tmp[7]);
	}
	else if (num == 3)
	{
		canvas->light_orig.x = ft_strtod(tmp[1]);
		canvas->light_orig.y = ft_strtod(tmp[2]);
		canvas->light_orig.z = ft_strtod(tmp[3]);
		canvas->light_bright = ft_strtod(tmp[4]);
		canvas->light_col[RED] = ft_strtod(tmp[5]);
		canvas->light_col[GREEN] = ft_strtod(tmp[6]);
		canvas->light_col[BLUE] = ft_strtod(tmp[7]);
	}
	else if (num == 4)
	{
		canvas->obj->pl_cnt = 1;
		canvas->obj->pl->on_plane.x = ft_strtod(tmp[1]);
		canvas->obj->pl->on_plane.y = ft_strtod(tmp[2]);
		canvas->obj->pl->on_plane.z = ft_strtod(tmp[3]);
		canvas->obj->pl->norm.x = ft_strtod(tmp[4]);
		canvas->obj->pl->norm.y = ft_strtod(tmp[5]);
		canvas->obj->pl->norm.z = ft_strtod(tmp[6]);
	}
	else if (num == 5)
	{
		canvas->obj->sp_cnt = 1;
		canvas->obj->sp->center.x = ft_strtod(tmp[1]);
		canvas->obj->sp->center.y = ft_strtod(tmp[2]);
		canvas->obj->sp->center.z = ft_strtod(tmp[3]);
		canvas->obj->sp->radius = ft_strtod(tmp[4]);
		canvas->obj->sp->color[RED] = ft_strtod(tmp[5]);
		canvas->obj->sp->color[GREEN] = ft_strtod(tmp[6]);
		canvas->obj->sp->color[BLUE] = ft_strtod(tmp[7]);
	}
}

void	free_split(char **tmp)
{
	int	i;

	i = -1;
	while (tmp[++i])
		free(tmp[i]);
	free(tmp);
}

t_canvas	parse(char *av[])
{
	t_canvas	data;
	t_volume	*obj;
	int			fd;
	char		*line;
	char		**tmp;
	int			i;

	i = 0;
	obj = (t_volume *)malloc(sizeof(t_volume));
	obj->sp = (t_sphere *)malloc(sizeof(t_sphere));
	obj->pl = (t_plane *)malloc(sizeof(t_plane));
	obj->cy = (t_cylinder *)malloc(sizeof(t_cylinder));
	data.obj = obj;
	fd = open(av[1], O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (!line || !*line)
			break ;
		tmp = ft_split(line, " \t\n,");
		if (*tmp)
			init_data(tmp, &data, i);
		free(line);
		free_split(tmp);
		i++;
	}
	return (data);
}

void	leaks()
{
	system("leaks a.out");
}

int main(int ac, char *av[])
{
	(void)ac;
	parse(av);
	int a = 3;
	atexit(leaks);
	return (0);
}

