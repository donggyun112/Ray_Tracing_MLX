/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 11:34:38 by jinhyeop          #+#    #+#             */
/*   Updated: 2022/11/22 19:31:34 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_head;
	t_list	*new_list;
	void	*con;

	if (lst == NULL)
		return (0);
	new_head = NULL;
	while (lst)
	{
		con = f(lst->content);
		new_list = ft_lstnew(con);
		if (new_list == NULL)
		{
			del(con);
			ft_lstclear(&new_head, del);
			return (0);
		}
		ft_lstadd_back(&new_head, new_list);
		lst = lst->next;
	}
	return (new_head);
}
