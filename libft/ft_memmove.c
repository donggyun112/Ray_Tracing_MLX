/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 10:53:23 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/22 15:31:37 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	idx;

	if (dst == NULL && src == NULL)
		return (dst);
	if (dst < src)
	{
		idx = 0;
		while (idx < len)
		{
			*(unsigned char *)(dst + idx) = *(unsigned char *)(src + idx);
			idx++;
		}
	}
	else if (dst >= src)
	{
		idx = len;
		while (idx-- > 0)
			*(unsigned char *)(dst + idx) = *(unsigned char *)(src + idx);
	}
	return (dst);
}
