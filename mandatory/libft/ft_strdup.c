/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 17:16:15 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/22 17:24:50 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strdup(const char *s1)
{
	size_t	len;
	size_t	idx;
	char	*dup;

	len = 0;
	idx = 0;
	while (s1[len])
		len++;
	dup = (char *)malloc(len + 1);
	if (dup == NULL)
		return (0);
	while (s1[idx])
	{
		dup[idx] = s1[idx];
		idx++;
	}
	dup[idx] = '\0';
	return (dup);
}
