/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:34:28 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/07/31 20:12:51 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_canvas	tmp_parse(void)
{
	t_canvas	canvas;

	canvas.width = 1920;
	canvas.height = 1080;
	canvas.ratio = 1920.0 / 1080.0;
	canvas.amb_bright = 0.7;
	canvas.amb_col[0] = 120;
	canvas.amb_col[1] = 200;
	canvas.amb_col[2] = 10;
	canvas.cam_orig = vec3(0.0, 0.0, 0.0);
	canvas.cam_dir = norm_vec(vec3(1.0, 0.0, 0.0));
	canvas.fov = 40;
	canvas.light_orig = vec3(2.0, 2.0, 1.0);
	canvas.light_bright = 0.5;
	canvas.light_col[0] = 255;
	canvas.light_col[1] = 255;
	canvas.light_col[2] = 255;
	canvas.obj = (t_volume *)malloc(sizeof(t_volume));
	canvas.obj->cy_cnt = 0;
	canvas.obj->pl_cnt = 0;
	canvas.obj->sp_cnt = 2;
	canvas.obj->cy = NULL;
	canvas.obj->pl = NULL;
	canvas.obj->sp = (t_sphere *)malloc(sizeof(t_sphere) * canvas.obj->sp_cnt);
	canvas.obj->sp[0].center = vec3(5.0, 0.0, -0.5);
	canvas.obj->sp[0].radius = 1.2;
	canvas.obj->sp[0].color[0] = 255;
	canvas.obj->sp[0].color[1] = 0;
	canvas.obj->sp[0].color[2] = 0;
	canvas.obj->sp[1].center = vec3(5.0, 0.0, 1.0);
	canvas.obj->sp[1].radius = 1.0;
	canvas.obj->sp[1].color[0] = 0;
	canvas.obj->sp[1].color[1] = 255;
	canvas.obj->sp[1].color[2] = 0;
	return (canvas);
}
