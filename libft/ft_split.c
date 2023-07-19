/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 12:06:59 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/03/29 22:14:21 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static unsigned int	cnt_words(char const *s, char c)
{
	unsigned int	cnt;
	unsigned int	idx;
	unsigned int	flag;

	cnt = 0;
	idx = 0;
	flag = 0;
	while (s[idx])
	{
		if (flag == 0 && s[idx] != c)
		{
			flag = 1;
			cnt++;
		}
		else if (flag == 1 && s[idx] == c && s[idx] != '\0')
			flag = 0;
		idx++;
	}
	return (cnt);
}

static void	free_all(char **array, unsigned int cnt)
{
	unsigned int	idx;

	idx = 0;
	while (idx <= cnt)
		free(array[idx++]);
	free(array);
}

static char	**put_words(char **array, unsigned int cnt, char const *s, char c)
{
	unsigned int	len;
	unsigned int	idx;

	len = 0;
	idx = 0;
	while (s[len] != c && s[len] != '\0')
		len++;
	array[cnt] = (char *)malloc(len + 1);
	if (array[cnt] == NULL)
	{
		free_all(array, cnt);
		array = NULL;
		return (0);
	}
	while (s[idx] != c && s[idx] != '\0')
	{
		array[cnt][idx] = s[idx];
		idx++;
	}
	array[cnt][idx] = '\0';
	return (array);
}

static size_t	get_len(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len - 1);
}

char	**ft_split(char const *s, char c)
{
	unsigned int	cnt;
	char			**array;

	if (s == NULL)
		return (0);
	cnt = cnt_words(s, c);
	array = (char **)malloc(sizeof(char *) * (cnt + 1));
	if (array == NULL)
		return (0);
	cnt = 0;
	while (*s)
	{
		if (*s != c)
		{
			array = put_words(array, cnt, s, c);
			if (array == NULL)
				return (0);
			s = s + get_len(array[cnt]);
			cnt++;
		}
		s++;
	}
	array[cnt] = NULL;
	return (array);
}
