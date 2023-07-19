/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 17:33:08 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/22 15:14:48 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	ft_bzero(void *s, size_t n)
{
	size_t	idx;

	idx = 0;
	while (idx < n)
	{
		*(unsigned char *)(s + idx) = 0;
		idx++;
	}
}
