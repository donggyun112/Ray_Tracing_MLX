/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 14:24:52 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/22 19:00:59 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	idx;
	unsigned int	len;

	idx = 0;
	len = 0;
	while (s[len])
		len++;
	while (idx < len)
	{
		f(idx, &s[idx]);
		idx++;
	}
}
