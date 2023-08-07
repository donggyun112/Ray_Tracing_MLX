/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 09:55:23 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/03/21 17:51:16 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	idx;
	unsigned int	len1;
	unsigned int	len2;
	char			*new;

	idx = 0;
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	new = (char *)malloc(len1 + len2 +1);
	if (new == NULL)
		return (0);
	while (s1 != NULL && s1[idx])
	{
		new[idx] = s1[idx];
		idx++;
	}
	idx = 0;
	while (s2 != NULL && s2[idx])
		new[len1++] = s2[idx++];
	new[len1] = '\0';
	return (new);
}
