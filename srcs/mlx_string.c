/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_string.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 22:21:15 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 08:31:06 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	string_put_status(t_view *view)
{
	char	*tmp;
	char	*tmp2;

	if (view->change_dir)
		tmp = ft_strjoin("Editing Mode : ", "TRUE");
	else
		tmp = ft_strjoin("Editing Mode : ", "FALSE");
	mlx_string_put(view->mlx, view->win, view->mini_size + 30, 100, 0XFFFFF, tmp);
	free(tmp);
	mlx_string_put(view->mlx, view->win, view->mini_size + 30, 130, 0XFFFFF, "remove = F  back = G");
	mlx_string_put(view->mlx, view->win, view->mini_size + 30, 160, 0XFFFFF, "random create : Z");
	mlx_string_put(view->mlx, view->win, view->mini_size + 30, 190, 0XFFFFF, "copy : C");
	mlx_string_put(view->mlx, view->win, view->mini_size + 30, 220, 0XFFFFF, "defult rander : T");
	tmp2 = ft_itoa(view->quality_scalar);
	tmp = ft_strjoin("Current Qulity : ", tmp2);
	free(tmp2);
	mlx_string_put(view->mlx, view->win, view->mini_size + 30, 250, 0XFFFFF, tmp);
	free(tmp);
}

void	string_put_grep_status(t_view *view)
{
	if (view->grep.type == SP && view->click_status)
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 280, 0XFFFFF, "Grep Obj Type: SP");
	else if (view->grep.type == CY && view->click_status)
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 280, 0XFFFFF, "Grep Obj Type: CY");
	else if (view->click_status)
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 280, 0XFFFFF, "Grep Obj Type: PL");
	else
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 280, 0XFFFFF, "Grep Obj Type:   ");
}

void	string_put(t_view *view)
{
	char	*tmp;
	char	*tmp2;

	if (!view->show_mouse)
	{
		string_put_status(view);
		string_put_grep_status(view);
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 310, 0XFFFFF, "qulity up : 1 | [ ");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 340, 0XFFFFF, "qulity down : 2 | ] ");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 370, 0XFFFFF, "Print ppm : 3");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 400, 0XFFFFF, "Make .rt : 4");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 430, 0XFFFFF, "Pause : p");
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 460, 0XFFFFF, "Mouse contol : m");
		tmp2 = ft_itoa(view->lnum);
		tmp = ft_strjoin("lidx : ", tmp2);
		mlx_string_put(view->mlx, view->win, view->mini_size + 30, 490, 0XFFFFF, tmp);
		free(tmp);
		free(tmp2);
	}
}
