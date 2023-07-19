/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 18:58:04 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/04/25 23:52:38 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <stdlib.h>

typedef struct s_fd
{
	int			fd;
	int			len;
	int			stat;
	char		*str;
	struct s_fd	*next;
}	t_fd;

char	*get_next_line(int fd);
t_fd	*set_init(t_fd **head, int fd);
int		check_str(char *str, int len, int start);
char	*gnl_strndup(const char *s1, int len);
void	free_node(t_fd **tmp, t_fd **head);

#endif
