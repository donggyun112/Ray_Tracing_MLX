/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 15:25:20 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/22 18:49:18 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static unsigned int	get_len(int n)
{
	unsigned int	len;

	if (n == 0)
		return (1);
	len = 0;
	while (n != 0)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

static void	fill_nbr(int n, char *arr, unsigned int len, int sign)
{
	char	nbr;

	arr[len] = '\0';
	len = len - 1;
	while (len > 0)
	{
		nbr = sign * (n % 10) + '0';
		n = n / 10;
		arr[len--] = nbr;
	}
	if (sign == -1)
		arr[0] = '-';
	else
		arr[0] = n + '0';
}

char	*ft_itoa(int n)
{
	unsigned int	len;
	int				sign;
	char			*arr;

	sign = 1;
	len = get_len(n);
	if (n < 0)
	{
		len = len + 1;
		sign = -1;
	}
	arr = (char *)malloc(len + 1);
	if (arr == NULL)
		return (0);
	fill_nbr(n, arr, len, sign);
	return (arr);
}
