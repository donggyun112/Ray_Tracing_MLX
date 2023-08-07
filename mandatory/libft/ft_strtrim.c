/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 10:28:52 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/01 00:55:50 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	in_set(char const c, char const *set)
{
	unsigned int	idx;

	idx = 0;
	if (set[0] == '\0')
		return (0);
	while (set[idx])
	{
		if (c == set[idx])
			return (1);
		idx++;
	}
	return (0);
}

static int	get_size(char const *s1, char const *set, unsigned int len)
{
	unsigned int	idx;
	unsigned int	cnt;

	cnt = 0;
	idx = len - 1;
	if (set[0] == 0)
		return (len);
	while (s1[cnt] && in_set(s1[cnt], set))
		cnt++;
	if (cnt == len)
		return (0);
	while (idx >= 0 && in_set(s1[idx--], set))
		cnt++;
	return (len - cnt);
}

static char	*copy(char const *s1, char const *set, char *tr, unsigned int size)
{
	unsigned int	idx;
	unsigned int	cut;

	idx = 0;
	cut = 0;
	if (size == 0)
	{
		tr[0] = '\0';
		return (tr);
	}
	while (in_set(s1[cut], set))
		cut++;
	while (idx < size)
		tr[idx++] = s1[cut++];
	tr[idx] = '\0';
	return (tr);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	size;
	unsigned int	len;
	char			*tr;

	size = 0;
	len = 0;
	if (s1[0] == '\0')
	{
		tr = (char *)malloc(1);
		if (tr == NULL)
			return (0);
		tr[0] = '\0';
		return (tr);
	}
	while (s1[len])
		len++;
	size = get_size(s1, set, len);
	tr = (char *)malloc(size + 1);
	if (tr == NULL)
		return (0);
	copy(s1, set, tr, size);
	return (tr);
}
