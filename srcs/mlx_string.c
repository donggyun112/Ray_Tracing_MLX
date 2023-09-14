/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_string.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 22:21:15 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 08:58:33 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	win_put_str(t_view *view, int line, char *str)
{
	const int	pos = view->mini_size + 30;
	const int	color = 0X0FFFFF;

	mlx_string_put(view->mlx, view->win, pos, line, color, str);
}

void	string_put_status(t_view *view)
{
	char	*tmp;
	char	*tmp2;

	if (view->change_dir)
		tmp = ft_strjoin("Editing Mode : ", "TRUE");
	else
		tmp = ft_strjoin("Editing Mode : ", "FALSE");
	win_put_str(view, 100, tmp);
	free(tmp);
	win_put_str(view, 130, "Remove = F  Undo = G");
	win_put_str(view, 160, "Random copy : Z");
	win_put_str(view, 190, "Ropy : C");
	win_put_str(view, 220, "Default render : T");
	tmp2 = ft_itoa(view->quality_scalar);
	tmp = ft_strjoin("Current Quality : ", tmp2);
	free(tmp2);
	win_put_str(view, 250, tmp);
	free(tmp);
}

void	string_put_grep_status(t_view *view)
{
	if (view->grep.type == SP && view->click_status)
		win_put_str(view, 280, "Grep Obj Type : SP");
	else if (view->grep.type == CY && view->click_status)
		win_put_str(view, 280, "Grep Obj Type : CY");
	else if (view->click_status)
		win_put_str(view, 280, "Grep Obj Type : PL");
	else
		win_put_str(view, 280, "Grep Obj Type :   ");
}

void	string_put(t_view *view)
{
	char	*tmp;
	char	*tmp2;

	if (!view->show_mouse)
	{
		string_put_status(view);
		string_put_grep_status(view);
		win_put_str(view, 310, "Quality up : 1 | [");
		win_put_str(view, 340, "Quality down : 2 | ]");
		win_put_str(view, 370, "Save image to ppm : 3");
		win_put_str(view, 400, "Export .rt : 4");
		win_put_str(view, 430, "Pause : P");
		win_put_str(view, 460, "View mouse point : M");
		tmp2 = ft_itoa(view->lnum);
		tmp = ft_strjoin("lidx : ", tmp2);
		win_put_str(view, 490, tmp);
		free(tmp);
		free(tmp2);
	}
}
