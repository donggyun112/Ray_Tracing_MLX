/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 16:52:05 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/22 17:23:14 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	idx;

	idx = 0;
	ptr = (void *)malloc(count * size);
	if (ptr == NULL)
		return (0);
	while (idx < count * size)
	{
		*(unsigned char *)(ptr + idx) = 0;
		idx++;
	}
	return (ptr);
}
