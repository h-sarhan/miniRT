/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hassanAsarhan@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:00:08 by hsarhan           #+#    #+#             */
/*   Updated: 2022/05/31 11:19:51 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*memory;

	if (count != 0 && size != 0 && count > SIZET_MAX / size)
		return (NULL);
	memory = malloc(count * size);
	if (memory == NULL)
		return (NULL);
	ft_bzero(memory, count * size);
	return (memory);
}
