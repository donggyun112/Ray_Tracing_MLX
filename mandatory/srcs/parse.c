/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 21:44:31 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/06 00:57:08 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

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
