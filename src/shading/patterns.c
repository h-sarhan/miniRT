/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patterns.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 13:23:32 by mkhan             #+#    #+#             */
/*   Updated: 2023/01/10 15:59:11 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_color	check_pattern_type(t_intersection *itx)
{
	t_vector	transf_point;
	t_mat4		pattern_transf;
	
	if (itx->shape->props.pattern_type == NONE)
		return (itx->shape->props.color);
	scaling_matrix(&pattern_transf, 4, 4, 4);
	mat_vec_multiply(&transf_point, &itx->shape->inv_transf, &itx->over_point);
	mat_vec_multiply(&transf_point, &pattern_transf, &transf_point);
	transf_point.x += 0.5;
	transf_point.y += 0.5;
	transf_point.z += 0.5;
	if (itx->shape->props.pattern_type == STRIPE)
		return (stripe_pattern(transf_point, int_to_color(0xffffff),
				int_to_color(0xff0000)));
	else if (itx->shape->props.pattern_type == CHECKER_BOARD)
		return (checker_pattern(transf_point, int_to_color(0x00ff00),
				int_to_color(0xff0000)));
	return (itx->shape->props.color);
	
}

t_color	stripe_pattern(t_vector point, t_color a, t_color b)
{
	if ((int) floorf(point.x) % 2 == 0)
		return (b);
	return (a);
}

t_color	checker_pattern(t_vector point, t_color a, t_color b)
{
	if ((int)(floorf(point.x) + floorf(point.y) + floorf(point.z)) % 2 == 0)
		return (b);
	return (a);
}