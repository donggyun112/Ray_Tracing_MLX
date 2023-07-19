/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 16:20:14 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/22 16:23:08 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	idx;

	idx = 0;
	while (idx < n)
	{
		if (*(unsigned char *)s == (unsigned char)c)
			return ((void *)s);
		s++;
		idx++;
	}
	return (0);
}
