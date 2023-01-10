/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patterns.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 13:23:32 by mkhan             #+#    #+#             */
/*   Updated: 2023/01/10 16:22:25 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_color	check_pattern_type(t_intersection *itx)
{
	t_vector	transf_point;
	t_mat4		pattern_transf;
	
	if (itx->shape->props.pattern_type == NONE)
		return (itx->shape->props.color);
	scaling_matrix(&pattern_transf, 0.5, 0.5, 0.5);
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
	else if (itx->shape->props.pattern_type == GRADIENT)
		return (gradient_pattern(transf_point, int_to_color(0xff0000),
				int_to_color(0x0000ff)));
	return (itx->shape->props.color);
	
}

t_color	stripe_pattern(t_vector point, t_color a, t_color b)
{
	if ((int) floorf(point.x) % 2 == 0)
		return (b);
	return (a);
}

t_color	gradient_pattern(t_vector point, t_color a, t_color b)
{
	t_color		color;
	float		fraction;

	sub_colors(&color, &b, &a);
	fraction = point.x - floorf(point.x);
	mult_color(&color, &color, fraction);	
	add_colors(&color, &color, &a);
	return (color);
}

t_color	checker_pattern(t_vector point, t_color a, t_color b)
{
	if ((int)(floorf(point.x) + floorf(point.y) + floorf(point.z)) % 2 == 0)
		return (b);
	return (a);
}