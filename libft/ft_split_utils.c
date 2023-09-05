/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 03:17:20 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/06 03:19:32 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_charset(char tmp, char *c)
{
	while (*c)
	{
		if (tmp == *c)
			return (1);
		c++;
	}
	return (0);
}

int	string_len(char const *s, char *c)
{
	int	len;

	len = 0;
	while (s[len] && !is_charset(s[len], c))
		len++;
	return (len);
}

char	**ft_clearall__________(int j, char **arr)
{
	while (j > 0)
		free(arr[--j]);
	free(arr);
	return (NULL);
}

char	**ft_putstring(char const *s, char *c, char **arr)
{
	int	len;
	int	j;
	int	z;

	j = 0;
	while (*s)
	{
		if (!is_charset(*s, c))
		{
			len = string_len(s, c);
			arr[j] = (char *)malloc(len + 1);
			if (!arr[j])
				return (ft_clearall__________(j, arr));
			z = 0;
			while (len-- > 0)
				arr[j][z++] = *s++;
			arr[j][z] = '\0';
			j++;
		}
		else
			s++;
	}
	return (arr);
}

int	word_count(char const *s, char *c)
{
	int	count;

	count = 0;
	while (*s)
	{
		if (!is_charset(*s, c))
		{
			count++;
			while (*s && !is_charset(*s, c))
				s++;
		}
		else
			s++;
	}
	return (count);
}
