/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 15:38:59 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/22 17:00:49 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	size_t	idx;

	if (n == 0)
		return (0);
	idx = 0;
	while ((unsigned char)str1[idx] == (unsigned char)str2[idx]
		&& idx < n && str1[idx] && str2[idx])
		idx++;
	if (idx == n)
		return (0);
	else
		return ((unsigned char)str1[idx] - (unsigned char)str2[idx]);
}
