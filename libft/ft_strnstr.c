/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 15:16:45 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/22 16:59:56 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	check;
	size_t	idx;
	size_t	n_len;

	check = 0;
	n_len = 0;
	idx = 0;
	if (*needle == '\0')
		return ((char *)haystack);
	while (needle[n_len])
		n_len++;
	while (check < len && *haystack)
	{
		while (haystack[idx] == needle[idx]
			&& haystack[idx] && needle[idx] && check + idx < len)
			idx++;
		if (idx == n_len)
			return ((char *)haystack);
		haystack++;
		check++;
		idx = 0;
	}
	return (0);
}
