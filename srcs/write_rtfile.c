/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_rtfile.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 22:35:05 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/13 22:41:22 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	write_rt_vec(t_vec3 vec, FILE *f)
{
	fprintf(f, " %f,%f,%f ", vec.x, vec.y, vec.z);
}

void	write_rt_color(int color[3], FILE *f)
{
	fprintf(f, " %d,%d,%d\n", color[RED], color[GREEN], color[BLUE]);
}

void	save_image_to_rtfile(char *filename, t_view *view)
{
	FILE	*f;

	f = fopen(filename, "w");
	if (!f)
	{
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		return ;
	}
	fprintf(f, "R	%d %d\n", view->real_size, view->can.height);
	fprintf(f, "A	%f ", view->can.amb_bright);
	write_rt_color(view->can.amb_col, f);
	write_rt_camera(view, f);
	write_rt_light(view, f);
	write_rt_sphere(view, f);
	write_rt_cylinder(view, f);
	write_rt_plane(view, f);
	if (view->can.bgt_filepath)
		fprintf(f, "bg %s \n", view->can.bgt_filepath);
	mlx_string_put(view->mlx, view->win, view->can.width / 2, view->can.height \
	/ 2, 0xFF00FF, "Success make .rt file");
}