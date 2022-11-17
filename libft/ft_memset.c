/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hassanAsarhan@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 12:47:22 by hsarhan           #+#    #+#             */
/*   Updated: 2022/05/10 14:50:17 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*bytes;
	unsigned char	val;
	size_t			i;

	bytes = b;
	val = (unsigned char) c;
	i = 0;
	while (i < len)
	{
		bytes[i] = val;
		i++;
	}
	return (b);
}
