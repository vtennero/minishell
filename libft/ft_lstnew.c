/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:15:04 by vitenner          #+#    #+#             */
/*   Updated: 2024/01/10 11:27:55 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*ft_lstempty(t_list *lst)
{
	lst->content = NULL;
	lst->content_size = 0;
	return (lst);
}

static t_list	*ft_lstfill(t_list *lst, void const *content, size_t size)
{
	lst->content = ft_memcpy(lst->content, content, size);
	lst->content_size = size;
	return (lst);
}

t_list	*ft_lstnew(void const *content, size_t content_size)
{
	t_list	*lst_new;

	lst_new = ft_calloc(1, sizeof(t_list));
	if (lst_new)
	{
		if (content)
		{
			lst_new->content = ft_calloc(1, content_size);
			if (lst_new->content)
				ft_lstfill(lst_new, content, content_size);
			else
			{
				free((void *)lst_new);
				return (NULL);
			}
		}
		else
			ft_lstempty(lst_new);
		lst_new->next = NULL;
	}
	return (lst_new);
}
