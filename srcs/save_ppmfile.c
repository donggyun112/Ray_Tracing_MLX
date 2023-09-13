/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_ppmfile.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:30:06 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 01:30:21 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	init_ppm(FILE *f, unsigned int tmp, unsigned char color[3])
{
	color[RED] = (tmp >> 16) & 0xFF;
	color[GREEN] = (tmp >> 8) & 0xFF;
	color[BLUE] = tmp & 0xFF;
	fwrite(&color[RED], 1, 1, f);
	fwrite(&color[GREEN], 1, 1, f);
	fwrite(&color[BLUE], 1, 1, f);
}

void	save_image_to_ppm(char *filename, t_view *view)
{
	FILE			*f;
	int				xy[2];
	char			*dst;
	unsigned int	tmp;
	unsigned char	color[3];

	f = fopen(filename, "w");
	if (!f)
		return ;
	fprintf(f, "P6\n%d %d\n255\n", view->can.width, view->can.height);
	xy[1] = -1;
	while (++xy[1] < view->can.height)
	{
		xy[0] = -1;
		while (++xy[0] < view->can.width)
		{
			dst = view->addr + (xy[1] * view->line_length + xy[0] * \
			(view->bits_per_pixel / 8));
			tmp = *(unsigned int *)dst;
			init_ppm(f, tmp, color);
		}
	}
	fclose(f);
}
