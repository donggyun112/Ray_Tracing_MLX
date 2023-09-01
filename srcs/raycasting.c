/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 02:07:30 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/01 23:00:14 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vec3	left_upper(t_camera cam, float viewport[])
{
	t_vec3	left_upper;
	t_vec3	r_half;
	t_vec3	v_half;
	t_vec3	tmp;

	r_half = multiple_vector(0.5 * viewport[WIDTH], cam.r_norm);
	v_half = multiple_vector(0.5 * viewport[HEIGHT], cam.v_norm);
	tmp = sub_vector(multiple_vector(cam.focal_len, cam.dir), v_half);
	left_upper = sub_vector(tmp, r_half);
	return (left_upper);
}

t_camera	camera(t_canvas canvas)
{
	t_camera	cam;
	t_vec3		up;
	float		fov_radians;
	float		viewport[2];

	cam.origin = canvas.cam_orig;
	fov_radians = (float)canvas.fov * M_PI / 180.0;
	cam.focal_len = 1.0 / tan(0.5 * fov_radians);
	viewport[HEIGHT] = 2.0;
	viewport[WIDTH] = viewport[HEIGHT] * canvas.ratio;
	cam.dir = canvas.cam_dir;
	up = vec3(0.0, 1.0, 0.0);
	cam.r_norm = vector_product(cam.dir, up);
	cam.v_norm = vector_product(cam.dir, cam.r_norm);
	cam.left_upper = left_upper(cam, viewport);
	return (cam);
}

t_ray3	create_ray(t_camera cam, float u, float v)
{
	t_ray3	ray;
	t_vec3	on_vp;

	ray.origin = cam.origin;
	on_vp = add_three_vector(cam.left_upper, multiple_vector(u, cam.r_norm), \
	multiple_vector(v, cam.v_norm));
	ray.dir = norm_vec(on_vp);
	ray.t = -1.0;
	return (ray);
}
