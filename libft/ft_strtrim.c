/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hassanAsarhan@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 22:39:27 by hsarhan           #+#    #+#             */
/*   Updated: 2022/05/15 13:08:37 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*trimmed;
	int		i;

	start = 0;
	end = ft_strlen(s1);
	while (ft_strchr(set, s1[start]) != NULL)
		start++;
	while (ft_strchr(set, s1[end]) != NULL)
		end--;
	if (start > end)
		trimmed = malloc(sizeof(char));
	else
		trimmed = malloc(sizeof(char) * (end - start + 2));
	if (trimmed == NULL)
		return (NULL);
	i = 0;
	while (i + start <= end)
	{
		trimmed[i] = s1[start + i];
		i++;
	}
	trimmed[i] = '\0';
	return (trimmed);
}
