/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hassanAsarhan@outlook.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 08:18:06 by hsarhan           #+#    #+#             */
/*   Updated: 2022/05/10 08:46:33 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	int	uppercase_diff;

	uppercase_diff = 'a' - 'A';
	if (c >= 'a' && c <= 'z')
		return (c - uppercase_diff);
	return (c);
}
