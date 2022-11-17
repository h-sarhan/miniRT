/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 21:08:42 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/29 13:16:49 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*concat_str;
	size_t	len1;
	size_t	len2;
	size_t	i;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	concat_str = malloc(sizeof(char) * (len1 + len2 + 1));
	if (concat_str == NULL)
		return (NULL);
	i = 0;
	while (i < len1)
	{
		concat_str[i] = s1[i];
		i++;
	}
	while (i < len1 + len2)
	{
		concat_str[i] = s2[i - len1];
		i++;
	}
	concat_str[i] = '\0';
	return (concat_str);
}
