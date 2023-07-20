/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 17:05:49 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/04/25 23:52:13 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "get_next_line_bonus.h"

static char	*split(t_fd **tmp, int cut)
{
	char	*result;
	char	*backup;
	int		idx;

	idx = -1;
	result = (char *)malloc(sizeof(char) * (cut + 2));
	if (result == NULL)
		return (NULL);
	while (++idx <= cut)
		result[idx] = (*tmp)->str[idx];
	result[idx] = '\0';
	idx = -1;
	backup = (char *)malloc(sizeof(char) * ((*tmp)->len - cut));
	if (backup == NULL)
	{
		free(result);
		return (NULL);
	}
	while (++idx < ((*tmp)->len - cut - 1))
		backup[idx] = (*tmp)->str[idx + cut + 1];
	backup[idx] = '\0';
	free((*tmp)->str);
	(*tmp)->str = backup;
	return (result);
}

static char	*backup(t_fd **tmp, t_fd **head, int len)
{
	char	*result;
	int		cut;

	if (((*tmp)->len == 0 || len == -1) && (*tmp)->stat == 0)
	{
		free_node(tmp, head);
		return (NULL);
	}
	else if (check_str((*tmp)->str, (*tmp)->len, 0) == -1)
	{
		result = gnl_strndup((*tmp)->str, (*tmp)->len);
		free_node(tmp, head);
		return (result);
	}
	else
	{
		cut = check_str((*tmp)->str, (*tmp)->len, 0);
		result = split(tmp, cut);
		(*tmp)->len = (*tmp)->len - cut - 1;
		if (((*tmp)->len == 0 && (*tmp)->stat == 0) || result == NULL)
			free_node(tmp, head);
		return (result);
	}
}

static char	*str_link(char *buf, int len, t_fd **tmp)
{
	char	*line;
	int		idx;

	idx = 0;
	line = (char *)malloc(sizeof(char) * ((*tmp)->len + len + 1));
	if (line == NULL && (*tmp)->str != NULL)
		free((*tmp)->str);
	if (line == NULL)
		return (NULL);
	while ((*tmp)->str != NULL && idx < (*tmp)->len)
	{
		line[idx] = (*tmp)->str[idx];
		idx++;
	}
	idx = 0;
	while (idx < len)
	{
		line[(*tmp)->len + idx] = buf[idx];
		idx++;
	}
	line[(*tmp)->len + idx] = '\0';
	(*tmp)->len = (*tmp)->len + len;
	if ((*tmp)->str != NULL)
		free((*tmp)->str);
	return (line);
}

static char	*make_line(t_fd **tmp, t_fd **head, int fd)
{
	char	buf[BUFFER_SIZE];
	int		len;

	len = BUFFER_SIZE;
	if ((*tmp)->str != NULL && check_str((*tmp)->str, (*tmp)->len, 0) != -1)
		return (backup(tmp, head, len));
	while ((*tmp)->stat == 1)
	{
		len = read(fd, &buf, BUFFER_SIZE);
		if (len <= 0)
		{
			(*tmp)->stat = 0;
			break ;
		}
		(*tmp)->str = str_link(buf, len, tmp);
		if ((*tmp)->str == NULL)
		{
			free_node(tmp, head);
			return (NULL);
		}
		if (check_str((*tmp)->str, (*tmp)->len, (*tmp)->len - len) != -1)
			break ;
	}
	return (backup(tmp, head, len));
}

char	*get_next_line(int fd)
{
	static t_fd	*head;
	t_fd		*tmp;

	tmp = head;
	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	while (tmp != NULL)
	{
		if (tmp->fd == fd)
			break ;
		tmp = tmp->next;
	}
	if (tmp == NULL)
	{
		head = set_init(&head, fd);
		if (head == NULL)
			return (NULL);
		tmp = head;
	}
	return (make_line(&tmp, &head, fd));
}
