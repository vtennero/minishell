/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:14:48 by vitenner          #+#    #+#             */
/*   Updated: 2024/01/10 11:32:57 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*newlist;

	newlist = ft_calloc(1, sizeof(t_list));
	if (newlist)
	{
		while (lst && f)
		{
			newlist = f(lst);
			lst = lst->next;
		}
	}
	return (newlist);
}
