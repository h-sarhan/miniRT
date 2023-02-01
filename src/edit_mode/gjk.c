/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gjk.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 14:23:32 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/01 17:40:58 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

extern t_vector *point_to_draw_1;
extern t_vector *point_to_draw_2;

int	sign(float num)
{
	if (num < 0)
		return (-1);
	return (1);
}


// from: https://www.cas.mcmaster.ca/~carette/SE3GB3/2006/notes/gjk1_pres.pdf
t_vector	cylinder_support_function(const t_vector *dir, const t_shape *cyl)
{
	t_vector	support_point;
	t_vector	w;
	t_vector	cyl_normal;
	t_vector	up_vector;
	
	ft_bzero(&up_vector, sizeof(t_vector));
	up_vector.y = 1;
	mat_vec_multiply(&cyl_normal, &cyl->transf, &up_vector);
	normalize_vec(&cyl_normal);
	float	u_d = dot_product(&cyl_normal, dir);
	scale_vec(&w, &cyl_normal, -u_d);
	add_vec(&w, &w, dir);
	t_vector	rhs;
	scale_vec(&rhs, &cyl_normal, sign(dot_product(&cyl_normal, dir)) * cyl->props.height / 2);
	add_vec(&support_point, &cyl->origin, &rhs);
	if (vec_magnitude(&w) < 0.01)
	{
		support_point.w = 1;
		return (support_point);
	}
	normalize_vec(&w);
	scale_vec(&w, &w, cyl->props.radius);
	add_vec(&support_point, &support_point, &w);
	support_point.w = 1;
	return (support_point);
}
