/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hassanAsarhan@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:06:24 by hsarhan           #+#    #+#             */
/*   Updated: 2022/05/10 09:07:05 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	int	uppercase_diff;

	uppercase_diff = 'a' - 'A';
	if (c >= 'A' && c <= 'Z')
		return (c + uppercase_diff);
	return (c);
}
