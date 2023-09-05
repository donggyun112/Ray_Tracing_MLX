/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:42:41 by dongkseo          #+#    #+#             */
/*   Updated: 2023/09/06 03:18:18 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_split(char const *s, char *c)
{
	char	**arr;
	int		count;

	if (!s)
		return (NULL);
	count = word_count(s, c);
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	arr = ft_putstring(s, c, arr);
	if (!arr)
	{
		free(arr);
		return (NULL);
	}
	arr[count] = NULL;
	return (arr);
}
