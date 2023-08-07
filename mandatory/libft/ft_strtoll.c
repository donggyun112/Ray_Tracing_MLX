/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 01:17:38 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/03/30 01:18:16 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

long long	ft_strtoll(const char *str)
{
	long long	sign;
	long long	sum;

	sign = 1;
	sum = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		sum = (10 * sum) + (sign * (*str - '0'));
		str++;
	}
	return (sum);
}
