/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 02:02:12 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 02:32:46 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	error_print(char *tmp, int expected, int input_count)
{
	ft_putstr_fd("Error: [", 2);
	ft_putstr_fd(tmp, 2);
	ft_putstr_fd("] Expected ", 2);
	ft_putnbr_fd(expected, 2);
	ft_putstr_fd(" arguments, but got ", 2);
	ft_putnbr_fd(input_count, 2);
	ft_putstr_fd(". Please check your input.\n", 2);
	exit(1);
}

void	find_problem(char **tmp, int count)
{
	if (!ft_strcmp(tmp[0], "L") && count != 7)
		error_print(tmp[0], 7, count);
	else if (count != 11 && !ft_strcmp(tmp[0], "cy"))
		error_print(tmp[0], 11, count);
	if (count != 7 && !ft_strcmp(tmp[0], "sp"))
		error_print(tmp[0], 7, count);
	else if (count != 9 && !ft_strcmp(tmp[0], "pl"))
		error_print(tmp[0], 9, count);
	else if (count != 2 && !ft_strcmp(tmp[0], "R"))
		error_print(tmp[0], 2, count);
	else if (count != 7 && (!ft_strcmp(tmp[0], "C")))
		error_print(tmp[0], 7, count);
	else if (count != 4 && !ft_strcmp(tmp[0], "A"))
		error_print(tmp[0], 4, count);
}
