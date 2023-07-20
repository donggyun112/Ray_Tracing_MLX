/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 13:42:10 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/14 15:19:47 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	idx;
	unsigned int	len;
	char			*map;

	idx = 0;
	len = 0;
	while (s[len])
		len++;
	map = (char *)malloc(len + 1);
	if (map == NULL)
		return (0);
	while (idx < len)
	{
		map[idx] = f(idx, s[idx]);
		idx++;
	}
	map[idx] = '\0';
	return (map);
}
