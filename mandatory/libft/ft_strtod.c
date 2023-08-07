/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtod.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 15:31:09 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/01 01:14:49 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>

double	ft_strtod(char *nptr)
{
	double	res;
	int		arr[3];
	int		scale;
	char	**tmp;
	int		idx;

	res = 0.0;
	idx = 0;
	arr[0] = 1;
	if (*nptr == '-')
		arr[0] = -1;
	tmp = ft_split(nptr, ".");
	arr[1] = ft_atoi(tmp[0]);
	if (tmp[1] != NULL)
		arr[2] = ft_atoi(tmp[1]);
	else
		arr[2] = 0;
	scale = ft_strlen(tmp[1]);
	res = (double)arr[1] + arr[0] * ((double)arr[2] * pow(1.0 / 10.0, scale));
	while (tmp[idx] != NULL)
		free(tmp[idx++]);
	free(tmp);
	return (res);
}
