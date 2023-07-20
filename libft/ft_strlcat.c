/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 13:13:54 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/22 15:44:21 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	idx;
	size_t	dstlen;
	size_t	srclen;
	size_t	rtn;

	idx = 0;
	dstlen = 0;
	srclen = 0;
	while (src[srclen])
		srclen++;
	if (dst == NULL && dstsize == 0)
		return (srclen + dstsize);
	while (dst[dstlen])
		dstlen++;
	if (dstsize <= dstlen)
		return (srclen + dstsize);
	rtn = dstlen + srclen;
	while (src[idx] && dstlen < dstsize - 1)
		dst[dstlen++] = src[idx++];
	dst[dstlen] = '\0';
	return (rtn);
}
