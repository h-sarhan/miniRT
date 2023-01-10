/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patterns.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 13:23:32 by mkhan             #+#    #+#             */
/*   Updated: 2023/01/10 13:52:07 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_color	check_pattern_type(t_intersection *itx)
{
	if (itx->shape->props.pattern_type == NONE)
		return (itx->shape->props.color);
	else if (itx->shape->props.pattern_type == STRIPE)
		return (stripe_pattern(itx->point, itx->shape->props.color,  int_to_color(0xff0000)));
	return (itx->shape->props.color);
	
}

t_color	stripe_pattern(t_vector point, t_color a, t_color b)
{
	if ((int)floor(point.x) % 2 == 0)
		return (a);
	return (b);
}