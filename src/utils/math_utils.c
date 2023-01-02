/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:21:50 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/02 17:22:52 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

float	max3(float n1, float n2, float n3)
{
	if (n1 >= n2 && n1 >= n3)
		return (n1);
	if (n2 >= n1 && n2 >= n3)
		return (n2);
	return (n3);
}

float	min3(float n1, float n2, float n3)
{
	if (n1 <= n2 && n1 <= n3)
		return (n1);
	if (n2 <= n1 && n2 <= n3)
		return (n2);
	return (n3);
}
