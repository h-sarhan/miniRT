/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:12:14 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/14 14:06:58 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	char	to_find;

	i = 0;
	if (s == NULL)
		return (NULL);
	to_find = (char) c;
	while (s[i] != '\0')
	{
		if (s[i] == to_find)
			return ((char *) &s[i]);
		i++;
	}
	if (s[i] == to_find)
		return ((char *) &s[i]);
	return (NULL);
}
