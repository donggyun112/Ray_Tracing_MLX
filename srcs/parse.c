/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 20:24:29 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/14 15:10:41 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

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

void	init_count(t_volume *obj, char **tmp)
{
	if (!ft_strcmp(tmp[0], "rsp"))
		obj->rsp_cnt++;
	if (!ft_strcmp(tmp[0], "RL"))
		obj->rl_cnt++;
	if (!ft_strcmp(tmp[0], "sp") || !ft_strcmp(tmp[0], "tsp") || \
	!ft_strcmp(tmp[0], "csp") || !ft_strcmp(tmp[0], "rsp"))
		obj->sp_cnt++;
	else if (!ft_strcmp(tmp[0], "cy") || !ft_strcmp(tmp[0], "ccy") || \
	!ft_strcmp(tmp[0], "tcy"))
		obj->cy_cnt++;
	else if (!ft_strcmp(tmp[0], "pl") || !ft_strcmp(tmp[0], "tr"))
		obj->pl_cnt++;
	else if (!ft_strcmp(tmp[0], "L") || !ft_strcmp(tmp[0], "RL"))
		obj->l_cnt++;
	else if (!ft_strcmp(tmp[0], "tpl"))
		obj->pl_cnt++;
	else if (!ft_strcmp(tmp[0], "cpl"))
		obj->pl_cnt++;
	else if (!ft_strcmp(tmp[0], "R"))
		obj->error_flag[0] = 1;
	else if (!ft_strcmp(tmp[0], "A"))
		obj->error_flag[1] = 1;
	else if (!ft_strcmp(tmp[0], "C"))
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
		fprintf(stderr, "Error: Missing required argument. [ R, A, C ]\n");
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

	fd = get_fd(av[1]);
	obj = init_volume(av);
	data.obj = obj;
	data.bgt_filepath = NULL;
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
