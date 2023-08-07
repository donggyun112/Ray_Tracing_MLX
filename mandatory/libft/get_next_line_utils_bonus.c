/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 18:21:18 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/04/23 18:37:50 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stddef.h>
#include "get_next_line_bonus.h"

t_fd	*set_init(t_fd **head, int fd)
{
	t_fd	*tmp;

	tmp = (t_fd *)malloc(sizeof(t_fd));
	if (tmp == NULL)
		return (NULL);
	tmp->fd = fd;
	tmp->len = 0;
	tmp->stat = 1;
	tmp->str = NULL;
	tmp->next = (*head);
	(*head) = tmp;
	return (*head);
}

int	check_str(char *str, int len, int start)
{
	if (str == NULL)
		return (-1);
	while (start < len)
	{
		if (str[start] == '\n')
			return (start);
		start++;
	}
	return (-1);
}

char	*gnl_strndup(const char *s1, int len)
{
	int		idx;
	char	*dup;

	idx = 0;
	dup = (char *)malloc(len + 1);
	if (dup == NULL)
		return (NULL);
	while (idx < len)
	{
		dup[idx] = s1[idx];
		idx++;
	}
	dup[idx] = '\0';
	return (dup);
}

void	free_node(t_fd **tmp, t_fd **head)
{
	t_fd	*fr;

	fr = (*head);
	while (fr != NULL && fr->next != (*tmp))
		fr = fr->next;
	if (fr == NULL)
	{
		(*head) = (*tmp)->next;
		if ((*tmp)->str != NULL)
			free((*tmp)->str);
		free(*tmp);
	}
	else
	{
		fr->next = (*tmp)->next;
		if ((*tmp)->str != NULL)
			free((*tmp)->str);
		free(*tmp);
	}
}
