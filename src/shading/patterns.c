/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patterns.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 13:23:32 by mkhan             #+#    #+#             */
/*   Updated: 2023/01/10 15:13:30 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_color	check_pattern_type(t_intersection *itx)
{
	t_vector	transf_point;
	t_mat4		pattern_transf;
	
	if (itx->shape->props.pattern_type == NONE)
		return (itx->shape->props.color);
	ft_bzero(&transf_point, sizeof(t_vector));
	scaling_matrix(&pattern_transf, 2.0, 2.0, 2.0);
	transf_point = itx->over_point;
	mat_vec_multiply(&transf_point, &itx->shape->inv_transf, &itx->over_point);
	mat_vec_multiply(&transf_point, &pattern_transf, &transf_point);
	if (itx->shape->props.pattern_type == STRIPE)
		return (stripe_pattern(transf_point, itx->shape->props.color,
				int_to_color(0xff0000)));
	else if (itx->shape->props.pattern_type == CHECKER_BOARD)
		return (checker_pattern(transf_point, itx->shape->props.color,
				int_to_color(0xff0000)));
	return (itx->shape->props.color);
	
}

t_color	stripe_pattern(t_vector point, t_color a, t_color b)
{
	if ((int) floorf(point.x) % 2 == 0)
		return (a);
	return (b);
}

t_color	checker_pattern(t_vector point, t_color a, t_color b)
{
	if ((int)(floorf(point.x) + floorf(point.y) + floorf(point.z)) % 2 == 0)
		return (a);
	return (b);
}