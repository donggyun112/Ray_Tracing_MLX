/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 22:00:03 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 13:40:40 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	error_print(char *tmp, int expected, int input_count)
{
	fprintf(stderr, \
	"Error: [%s] Expected \
%d arguments, but got %d. Please check your input.\n"\
	, tmp, expected, input_count);
	exit(1);
}

void	find_problem2(char **tmp, int count)
{
	if (count != 7 && (!ft_strcmp(tmp[0], "C") || !ft_strcmp(tmp[0], "tpl")))
		error_print(tmp[0], 7, count);
	else if (count != 4 && !ft_strcmp(tmp[0], "A"))
		error_print(tmp[0], 4, count);
	else if (count != 1 && !ft_strcmp(tmp[0], "bg"))
		error_print(tmp[0], 1, count);
}

void	find_problem(char **tmp, int count)
{
	if (!ft_strcmp(tmp[0], "L") && count != 7)
		error_print(tmp[0], 7, count);
	else if (count != 11 && !ft_strcmp(tmp[0], "cy"))
		error_print(tmp[0], 11, count);
	else if (count != 8 && !ft_strcmp(tmp[0], "ccy"))
		error_print(tmp[0], 8, count);
	else if (count != 10 && !ft_strcmp(tmp[0], "tcy"))
		error_print(tmp[0], 10, count);
	if (count != 7 && !ft_strcmp(tmp[0], "sp"))
		error_print(tmp[0], 7, count);
	else if ((count != 6 && count != 5) && !ft_strcmp(tmp[0], "tsp"))
		error_print(tmp[0], 6, count);
	else if (count != 4 && !ft_strcmp(tmp[0], "csp"))
		error_print(tmp[0], 4, count);
	else if (!ft_strcmp(tmp[0], "rsp") && count != 12)
		error_print(tmp[0], 12, count);
	else if (count != 9 && !ft_strcmp(tmp[0], "pl"))
		error_print(tmp[0], 9, count);
	else if (count != 6 && !ft_strcmp(tmp[0], "cpl"))
		error_print(tmp[0], 6, count);
	else if (count != 2 && !ft_strcmp(tmp[0], "R"))
		error_print(tmp[0], 2, count);
	find_problem2(tmp, count);
}
