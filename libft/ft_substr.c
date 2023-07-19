/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 17:46:48 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/22 18:34:06 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	idx;
	size_t	s_len;

	idx = 0;
	s_len = 0;
	while (s[s_len])
		s_len++;
	if (s_len < start || s_len == 0)
	{
		sub = (char *)malloc(1);
		if (sub == NULL)
			return (0);
		sub[0] = '\0';
		return (sub);
	}
	if (s_len < len + start)
		len = s_len - start;
	sub = (char *)malloc(len + 1);
	if (sub == NULL)
		return (0);
	while (idx < len)
		sub[idx++] = s[start++];
	sub[idx] = '\0';
	return (sub);
}
