/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:41:47 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 01:49:29 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	set_quality_scalar(t_view *view)
{
	if (view->quality_scalar > 0)
	{
		view->anti_scalar = view->quality_scalar;
		view->low_scalar = 1;
	}
	else if (view->quality_scalar < 0)
	{
		view->low_scalar = -view->quality_scalar;
		view->anti_scalar = 1;
	}
	else
	{
		view->low_scalar = 1;
		view->anti_scalar = 1;
	}
}

t_thread	*init_thread(t_view *view)
{
	t_thread	*m;
	int			x;

	m = (t_thread *)malloc(sizeof(t_thread) * NUM_OF_THREAD);
	set_quality_scalar(view);
	x = 0;
	while (x < NUM_OF_THREAD)
	{
		m[x].id = x;
		m[x].view = view;
		m[x].canvas = view->can;
		x++;
	}
	return (m);
}

void	multi_rend(t_view *view)
{
	t_thread	*m;
	int			x;

	x = 0;
	m = init_thread(view);
	while (x < NUM_OF_THREAD)
	{
		pthread_create(&m[x].thread, NULL, (void *)make_image2, &m[x]);
		x++;
	}
	x = 0;
	while (x < NUM_OF_THREAD)
	{
		pthread_join(m[x].thread, NULL);
		x++;
	}
	free(m);
}