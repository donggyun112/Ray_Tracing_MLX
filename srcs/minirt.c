/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:48:10 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/14 01:50:57 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int	is_valid_file_type(char *file_path)
{
	char	**path;
	int		answer;
	int		i;

	answer = 0;
	i = 0;
	path = ft_split(file_path, ".");
	if (!path)
		return (0);
	while (path[i] != 0)
		i++;
	if (ft_strcmp(path[i - 1], "rt") == 0)
		answer = 1;
	i = 0;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
	return (answer);
}

void	mlx_engine(t_view *view)
{
	mlx_hook(view->win, 2, 1L << 0, key_hook, view);
	mlx_hook(view->win, 4, 1L << 2, mouse_press, view);
	mlx_hook(view->win, 3, 1L << 1, key_release, view);
	mlx_hook(view->win, 17, 1L << 5, win_destroy, view);
	mlx_hook(view->win, 4, 1L << 2, mouse_press, view);
	mlx_hook(view->win, 5, 1L << 3, mouse_release, view);
	mlx_hook(view->win, 6, 1L << 7, mouse_motion, view);
	mlx_loop_hook(view->mlx, loop_hook, view);
}

int	main(int argc, char *argv[])
{
	t_view		view;
	t_canvas	canvas;

	if (argc != 2 || !is_valid_file_type(argv[1]))
	{
		printf("Error\nInput mapfile(*.rt) as argument\n");
		return (1);
	}
	srand((unsigned int)time(NULL));
	canvas = parse(argv);
	make_obj_cap(canvas.obj);
	view.cam = camera(canvas);
	view.can = canvas;
	view.mlx = mlx_init();
	view.win = mlx_new_window(view.mlx, canvas.width, canvas.height, "miniRT");
	view.img = mlx_new_image(view.mlx, canvas.width, canvas.height);
	view.addr = mlx_get_data_addr(view.img, &view.bits_per_pixel, \
		&view.line_length, &view.endian);
	init_view_scale(&view);
	set_texture(&view, canvas.obj);
	multi_rend(&view);
	mlx_put_image_to_window(view.mlx, view.win, view.img, 0, 0);
	mlx_engine(&view);
	mlx_loop(view.mlx);
	exit(0);
}
