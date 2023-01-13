/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patterns.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 13:23:32 by mkhan             #+#    #+#             */
/*   Updated: 2023/01/13 11:55:34 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_color	get_texture_color(t_intersection *itx)
{
	t_vector	shape_point;
	int			u;
	int			v;

	// shape_point = itx->point;
	mat_vec_multiply(&shape_point, &itx->shape->inv_transf, &itx->point);
	// if (fabs(shape_point.x) < 0.01 && fabs(shape_point.y) < 0.01)
	// {
	// 	// u = (int)((shape_point.x )* itx->shape->tex_width);
	// 	// v = (int)((shape_point.y ) * itx->shape->tex_height);
	// 	// print_color(&itx->shape->texture[0][0]);
	// 	return (int_to_color(0x00ff00));
	// }
	if (shape_point.x > 1 || shape_point.y > 1 || shape_point.x < -1 || shape_point.y < -1)
		return (itx->shape->props.color);
	
	// printf("point.x = %f\n", shape_point.x);
	// printf("point.y = %f\n", shape_point.y);
	// printf("point.z = %f\n", shape_point.z);
	shape_point.x += 1;
	shape_point.y += 1;
	shape_point.x /= 2;
	shape_point.y /= 2;
	if (shape_point.x < 0|| shape_point.y < 0)
		return (itx->shape->props.color);
	if (itx->shape->type == SPHERE)
	{
	}
	u = (int)(shape_point.x* itx->shape->tex_height) % itx->shape->tex_height;
	v = (int)(shape_point.y * itx->shape->tex_width) % itx->shape->tex_width;
	// u = (int)((shape_point.x) * 300);
	// v = (int)((shape_point.y) * 300);
	// printf("u = %d\n", u);
	// printf("v = %d\n", v);
	if (u >= itx->shape->tex_height || v >= itx->shape->tex_width)
		return (itx->shape->props.color);
	
	return (itx->shape->texture[u][v]);
}

t_color	get_shape_color(t_intersection *itx)
{
	if (itx->shape->texture != NULL)
		return (get_texture_color(itx));
	if (itx->shape->props.pattern_type == NONE)
		return (itx->shape->props.color);
	if (itx->shape->props.pattern_type == STRIPE)
		return (stripe_pattern(itx, itx->over_point, int_to_color(0xffffff),
				int_to_color(0xff0000)));
	else if (itx->shape->props.pattern_type == CHECKER_BOARD)
		return (checker_pattern(itx, itx->over_point, int_to_color(0x00ff00),
				int_to_color(0xff0000)));
	else if (itx->shape->props.pattern_type == GRADIENT)
		return (gradient_pattern(itx, itx->over_point, int_to_color(0xff0000),
				int_to_color(0x0000ff)));
	else if (itx->shape->props.pattern_type == RING)
		return (ring_pattern(itx, itx->over_point, int_to_color(0xff0000),
				int_to_color(0x0000ff)));
	return (itx->shape->props.color);
	
}

t_color	stripe_pattern(t_intersection *itx, t_vector point, t_color a, t_color b)
{
	t_vector	transf_point;
	t_mat4		pattern_transf;

	scaling_matrix(&pattern_transf, itx->shape->props.scale.x *4., itx->shape->props.scale.y *4., itx->shape->props.scale.z*4.0);
	mat_vec_multiply(&transf_point, &itx->shape->inv_transf, &point);
	mat_vec_multiply(&transf_point, &pattern_transf, &transf_point);
	transf_point.x += 0.5;
	transf_point.y += 0.5;
	transf_point.z += 0.5;
	if ((int) floorf(transf_point.x) % 2 == 0)
		return (b);
	return (a);
}

t_color	ring_pattern(t_intersection *itx, t_vector point, t_color a, t_color b)
{
	t_vector	transf_point;
	t_mat4		pattern_transf;
	
	scaling_matrix(&pattern_transf, itx->shape->props.scale.x *4., itx->shape->props.scale.y *4., itx->shape->props.scale.z*4.0);
	mat_vec_multiply(&transf_point, &itx->shape->inv_transf, &point);
	mat_vec_multiply(&transf_point, &pattern_transf, &transf_point);
	if ((int) floorf(sqrtf(((transf_point.x * transf_point.x) + (transf_point.z * transf_point.z)))) % 2 == 0)
		return (b);
	return (a);
}

t_color	gradient_pattern(t_intersection *itx, t_vector point, t_color a, t_color b)
{
	t_color		color;
	float		fraction;
	t_vector	transf_point;
	t_mat4		pattern_transf;
	
	scaling_matrix(&pattern_transf, 0.5, 0.5, 0.5);
	mat_vec_multiply(&transf_point, &itx->shape->inv_transf, &point);
	mat_vec_multiply(&transf_point, &pattern_transf, &transf_point);
	transf_point.x += 0.5;
	transf_point.y += 0.5;
	transf_point.z += 0.5;
	sub_colors(&color, &b, &a);
	fraction = transf_point.x - floorf(transf_point.x);
	mult_color(&color, &color, fraction);	
	add_colors(&color, &color, &a);
	return (color);
}

t_color	checker_pattern(t_intersection *itx, t_vector point, t_color a, t_color b)
{
	t_vector	transf_point;
	t_mat4		pattern_transf;
	
	scaling_matrix(&pattern_transf, itx->shape->props.scale.x *4., itx->shape->props.scale.y *4., itx->shape->props.scale.z*4.0);
	mat_vec_multiply(&transf_point, &itx->shape->inv_transf, &point);
	mat_vec_multiply(&transf_point, &pattern_transf, &transf_point);
	transf_point.x += 0.5;
	transf_point.y += 0.5;
	transf_point.z += 0.5;
	if ((int)(floorf(transf_point.x) + floorf(transf_point.y) + floorf(transf_point.z)) % 2 == 0)
		return (b);
	return (a);
}