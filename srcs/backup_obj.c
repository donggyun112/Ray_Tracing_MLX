/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backup_obj.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:23:29 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 01:23:58 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	push_front(t_backup **backup, void *obj, int type)
{
	t_backup	*tmp;
	t_backup	*head;

	tmp = (t_backup *)malloc(sizeof(t_backup));
	tmp->obj = obj;
	tmp->type = type;
	head = *backup;
	if (head == NULL)
	{
		head = tmp;
		head->next = NULL;
		*backup = head;
	}
	else
	{
		tmp->next = head;
		head = tmp;
		*backup = head;
	}
}

void	push_obj(t_sphere *sp, t_cylinder *cy, t_plane *pl, t_view *view)
{
	if (sp)
	{
		push_front(&view->backup, view->grep.obj, sp->type);
		sp->type = NONE;
	}
	else if (cy)
	{
		push_front(&view->backup, view->grep.obj, cy->type);
		cy->type = NONE;
	}
	else if (pl)
	{
		push_front(&view->backup, view->grep.obj, pl->type);
		pl->type = NONE;
	}
}

void	type_def(void *obj, int type)
{
	t_sphere	*sp;
	t_plane		*pl;
	t_cylinder	*cy;

	if (type == SP || type == TSP || type == CSP)
	{
		sp = (t_sphere *)obj;
		sp->type = type;
	}
	else if (type == CY || type == TCY || type == CCY)
	{
		cy = (t_cylinder *)obj;
		cy->type = type;
	}
	else if (type == PL || type == TPL || type == CPL)
	{
		pl = (t_plane *)obj;
		pl->type = type;
	}
}

int	clear_backup(t_backup **backup)
{
	t_backup	*tmp;

	if (*backup == NULL)
		return (0);
	if ((*backup)->type == SP || (*backup)->type == TSP \
	|| (*backup)->type == CSP)
		type_def((*backup)->obj, (*backup)->type);
	else if ((*backup)->type == CY || (*backup)->type == CCY \
	|| (*backup)->type == TCY)
		type_def((*backup)->obj, (*backup)->type);
	else if ((*backup)->type == PL || (*backup)->type == CPL \
	|| (*backup)->type == TPL)
		type_def((*backup)->obj, (*backup)->type);
	tmp = (*backup)->next;
	free((*backup));
	(*backup) = tmp;
	return (1);
}
