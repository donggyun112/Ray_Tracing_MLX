/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 02:07:30 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/07/27 21:19:27 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include ".includes/minirt.h"
#include "includes/vector.h"
#include "includes/struct.h"

t_ray3	create_ray(t_point3 camera, int i, int j)
{
	t_ray3	ray;
	double	width;
	double	height;

	width = 3;
	height = 2;
	//i range [0, 1500], j range [0, 1000]
	//left_up vector 에서 출발해서 width * i / 가로픽셀(1500)
	//height * j / 세로픽셀(1000) 을 더해주면 카메라의 기준에서
	//viewport로 가는 벡터를 구할 수 있다.
}

t_camera	cammera(t_point3 origin, t_vec3 dir, t_screen canvas)
{
	t_camera	cam; 
	t_vec3		up;
	double		fov_radians;
	double		viewport_height;

	// 카메라 좌표
	cam.origin = origin;

	// fov각도에 따른 viewport와 카메라의 거리	
	fov_radians = canvas.fov * M_PI / 180;
	cam.focal_len = 0.5 * canvas.height / tan(0.5 * fov_radians);
	
	// 정규화 되어있는 viewport 가로 세로 길이
	cam.vp_height = 2.0;
	cam.vp_width = cam.vp_height * canvas.aspect_ratio;

	// viewport를 향하는 direction vector
	cam.dir = dir;

	up = vec3(0.0, 1.0, 0.0);
	cam.r_norm = cross_product(cam.dir, up); // right = dir x up
	cam.v_norm = cross_product(cam.r_norm, cam.dir); // up = right x dir
	cam.left_lower = sub_vector(sub_vector(\
			sub_vector(cam.origin, multiply_vector(0.5 * cam.vp_width, cam.r_norm)), \
					multiply_vector(0.5 * cam.vp_height, cam.v_norm)), multiply_vector(cam.focal_len, cam.dir));
	return (cam);
}


/* void render(
    const Options &options,
    const std::vector<std::unique_ptr<Object>> &objects,
    const std::vector<std::unique_ptr<Light>> &lights)
{
    Matrix44f cameraToWorld;
    Vec3f *framebuffer = new Vec3f[options.width * options.height];
    Vec3f *pix = framebuffer;
    float scale = tan(deg2rad(options.fov * 0.5));
    float imageAspectRatio = options.width / (float)options.height;
    Vec3f orig;
    cameraToWorld.multVecMatrix(Vec3f(0), orig);

    for (uint32_t j = 0; j < options.height; ++j) {
        for (uint32_t i = 0; i < options.width; ++i) {
            // Calculate pixel position on the image plane
            float x = (2 * (i + 0.5) / (float)options.width - 1) * imageAspectRatio * scale;
            float y = (1 - 2 * (j + 0.5) / (float)options.height) * scale;

            // Calculate the direction of the ray in world coordinates
            float Px = (2 * ((x + 0.5) / options.width) - 1) * tan(deg2rad(options.fov * 0.5));
            float Py = (1 - 2 * ((y + 0.5) / options.height)) * tan(deg2rad(options.fov * 0.5));

            Vec3f rayOrigin = Point3(0, 0, 0);
            Vec3f rayPWorld(Px, Py, -1);
            cameraToWorld.multVecMatrix(rayOrigin, orig);
            cameraToWorld.multVecMatrix(rayPWorld, rayPWorld);
            Vec3f rayDirection = rayPWorld - orig;
            rayDirection.normalize();

            // Cast the ray and get the pixel color
            *(pix++) = castRay(orig, rayDirection, objects, lights, options, 0);
        }
    }

    // Save result to a PPM image (keep these flags if you compile under Wi
    std::ofstream ofs("./out.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << options.width << " " << options.height << "\n255\n";
    for (uint32_t i = 0; i < options.height * options.width; ++i) {
        char r = (char)(255 * clamp(0, 1, framebuffer[i].x));
        char g = (char)(255 * clamp(0, 1, framebuffer[i].y));
        char b = (char)(255 * clamp(0, 1, framebuffer[i].z));
        ofs << r << g << b;
    }
    ofs.close();
    delete[] framebuffer;
}
 */