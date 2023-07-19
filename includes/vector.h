/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 11:49:07 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/07/19 14:37:21 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include <math.h>

//vector
t_vec3	init_vec(double x, double y, double z);
t_vec3	norm_vec(t_vec3 vec);
double	scalar_product(t_vec3 v1, t_vec3 v2);
t_vec3	vector_product(t_vec3 v1, t_vec3 v2);

//simple_vec_cal
double	size_of_vec(double x, double y, double z);
void	multiple_vector(double t, t_vec3 *vec);
t_vec3	add_vector(t_vec3 v1, t_vec3 v2);
t_vec3	sub_vector(t_vec3 v1, t_vec3 v2);

#endif
