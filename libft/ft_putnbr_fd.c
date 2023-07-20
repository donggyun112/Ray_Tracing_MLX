/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 14:39:42 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/12 16:28:08 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putnbr_fd(int n, int fd)
{
	char	nbr;

	if (n > -10 && n < 10)
	{
		if (n < 0)
		{
			nbr = '0' - n;
			write(fd, "-", 1);
		}
		else
			nbr = '0' + n;
		write(fd, &nbr, 1);
	}
	else
	{
		if (n < 0)
			nbr = '0' - (n % 10);
		else
		{
			nbr = '0' + (n % 10);
		}
		n = n / 10;
		ft_putnbr_fd(n, fd);
		write(fd, &nbr, 1);
	}
}
