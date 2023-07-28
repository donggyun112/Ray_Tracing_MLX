/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:48:10 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/07/28 19:48:44 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	intersection(t_ray3 *ray, t_volume *volume)
{
	int	idx;

	idx = 0;
	while (idx < volume.pl_cnt)
	{
		pl_inter(volume.pl[idx], ray);
	}
}

void	make_image(t_view *view, t_canvas canvas)
{
	/*
	for (int i = 0; i < canvas.height; i++)
	{
		for (int j = 0; j < canvas.width; j++)
		{
			u,v값을 i,j에 맞게 변환
			ray = create_ray();
			intersection(&ray, canvas.volume);
			color_cal(view, canvas, &ray);	-> my_pixel_put_mlx()
		}
	}
	*/
}

int	main(int argc, char *argv[])
{
	t_view	view;

	(void)argc;
	(void)argv;
	//parse() + 오브젝트 배열 생성
	//make_camera() 파싱 데이터 받아서 카메라 생성
	view.mlx = mlx_init();
	view.win = mlx_new_window(view.mlx, 1920, 1080, "miniRT");
	view.img = mlx_new_image(view.mlx, 1920, 1080);
	view.addr = mlx_get_data_addr(view.img, view.bits_per_pixel, \
		view.line_length, view.endian);
	make_image(&view, canvas); // viewport를 향해서 반복문 사용하여 ray 발사
	mlx_hook(view.win, 17, 1L << 5, win_destroy, &view);
	mlx_hook(view.win, 2, 1L << 0, key_hook, &view);
	mlx_loop(view.mlx);
	return (0);
}
