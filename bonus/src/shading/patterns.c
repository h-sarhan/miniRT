/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patterns.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 13:23:32 by mkhan             #+#    #+#             */
/*   Updated: 2023/02/19 22:34:48 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_color	stripe_pattern(t_intersection *itx, t_vector point,
			t_color a, t_color b)
{
	t_vector	transf_point;
	t_mat4		pattern_transf;

	scaling_matrix(&pattern_transf, itx->shape->props.scale.x * 4.,
		itx->shape->props.scale.y * 4., itx->shape->props.scale.z * 4.0);
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

	scaling_matrix(&pattern_transf, itx->shape->props.scale.x * 4.,
		itx->shape->props.scale.y * 4., itx->shape->props.scale.z * 4.0);
	mat_vec_multiply(&transf_point, &itx->shape->inv_transf, &point);
	mat_vec_multiply(&transf_point, &pattern_transf, &transf_point);
	if ((int) floor(sqrt(((transf_point.x * transf_point.x) + \
		(transf_point.z * transf_point.z)))) % 2 == 0)
		return (b);
	return (a);
}

t_color	gradient_pattern(t_intersection *itx, t_vector point, t_color a,
			t_color b)
{
	t_color		color;
	double		fraction;
	t_vector	transf_point;
	t_mat4		pattern_transf;

	scaling_matrix(&pattern_transf, 0.5, 0.5, 0.5);
	mat_vec_multiply(&transf_point, &itx->shape->inv_transf, &point);
	mat_vec_multiply(&transf_point, &pattern_transf, &transf_point);
	transf_point.x += 0.5;
	transf_point.y += 0.5;
	transf_point.z += 0.5;
	sub_colors(&color, &b, &a);
	fraction = transf_point.x - floor(transf_point.x);
	mult_color(&color, &color, fraction);
	add_colors(&color, &color, &a);
	return (color);
}

void	spherical_map(double *u, double *v, t_vector *point)
{
	double		theta;
	t_vector	vec;
	double		radius;
	double		phi;

	vec = *point;
	vec.w = 0;
	theta = atan2(point->x, point->z);
	radius = vec_magnitude(&vec);
	phi = acos(point->y / radius);
	*u = 1 - ((theta / (2 * M_PI)) + 0.5);
	*v = 1 - (phi / M_PI);
}

t_color	checker_pattern(t_intersection *itx, t_vector *point)
{
	double		u2;
	double		v2;
	double		u;
	double		v;
	t_vector	transf_point;

	mat_vec_multiply(&transf_point, &itx->shape->inv_transf, point);
	if (itx->shape->type == CYLINDER || itx->shape->type == CONE)
		cylindrical_map(&u, &v, &transf_point);
	else if (itx->shape->type == SPHERE)
		spherical_map(&u, &v, &transf_point);
	else
		cubical_map(&u, &v, &transf_point);
	u2 = floor(u * 40);
	v2 = floor(v * 20);
	if ((int)(u2 + v2) % 2 == 0)
		return (int_to_color(0x0));
	return (int_to_color(0xffffff));
}
