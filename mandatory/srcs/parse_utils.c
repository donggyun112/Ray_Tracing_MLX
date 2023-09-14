/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 00:56:23 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 13:19:30 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int	argument_count(char **tmp)
{
	int	i;

	i = 0;
	while (tmp[i])
		i++;
	return (i);
}

void	init_view2(t_canvas *canvas, char **tmp)
{
	canvas->width = ft_strtod(tmp[1]);
	canvas->height = ft_strtod(tmp[2]);
	canvas->ratio = (double)canvas->width / (double)canvas->height;
}

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
