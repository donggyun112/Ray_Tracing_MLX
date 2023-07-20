/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 15:17:29 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/22 16:03:07 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strrchr(const char *str, int c)
{
	size_t	idx;

	idx = 0;
	while (str[idx])
		idx++;
	while (idx > 0)
	{
		if (str[idx] == (char)c)
			return ((char *)(str + idx));
		idx--;
	}
	if (str[idx] == (char)c)
		return ((char *)(str + idx));
	return (0);
}
