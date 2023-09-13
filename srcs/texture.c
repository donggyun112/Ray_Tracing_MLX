/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 07:46:05 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 07:58:34 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int	checker_value(float coordinate)
{
	if (coordinate < 0)
	{
		coordinate -= 1;
	}
	return ((int)coordinate % 2);
}

t_vec3	get_u_v_axis(t_vec3 norm)
{
	if (fabs(norm.x) > fabs(norm.y) && fabs(norm.x) > fabs(norm.z))
		return ((t_vec3){0, 1, 0});
	else if (fabs(norm.y) > fabs(norm.x) && fabs(norm.y) > fabs(norm.z))
		return ((t_vec3){1, 0, 0});
	else
		return ((t_vec3){1, 0, 0});
}

t_color	checkertexture(t_vec3 point, float scale, t_plane *pl)
{
	t_vec3	u_axis;
	t_vec3	v_axis;
	t_vec3	relative_pos;
	float	u;
	float	v;

	relative_pos = sub_vector(point, pl->on_plane);
	u_axis = get_u_v_axis(pl->norm);
	v_axis = vector_product(pl->norm, u_axis);
	u = scalar_product(relative_pos, u_axis) * scale;
	v = scalar_product(relative_pos, v_axis) * scale;
	if ((checker_value(u) + checker_value(v)) % 2 == 0)
		return ((t_color){0, 0, 0});
	else
		return ((t_color){255, 255, 255});
}

t_color	get_texture_color(t_texture texture, float u, float v)
{
	const int	x = (int)(u * (float)texture.width) % texture.width;
	const int	y = (int)(v * (float)texture.height) % texture.height;
	const int	offset = (x + y * texture.width) * (texture.bpp / 8);
	const int	color = *(int *)(texture.data + offset);
	t_color		c;

	c.r = (color >> 16) & 0xFF;
	c.g = (color >> 8) & 0xFF;
	c.b = color & 0xFF;
	return (c);
}

void	init_texture(t_texture *texture, t_view *view, char *path)
{
	texture->img = mlx_xpm_file_to_image(view->mlx, path, \
	&texture->width, &texture->height);
	if (!texture->img)
	{
		fprintf(stderr, "Failed to load texture: %s\n", path);
		exit(1);
	}
	texture->data = mlx_get_data_addr(texture->img, \
	&texture->bpp, &texture->size_line, &texture->endian);
}
