/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 22:33:51 by mkhan             #+#    #+#             */
/*   Updated: 2023/02/20 07:49:28 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"


t_color	get_shape_color(t_intersection *itx)
{
	return (itx->shape->props.color);
}

bool	get_specular_and_diffuse(t_scene *scene, int light_idx,
	t_intersection *itx, t_phong *phong)
{
	double		light_dot_normal;
	t_vector	light_v;
	t_vector	reflect_v;

	sub_vec(&light_v, &scene->lights[light_idx].position, &itx->over_point);
	normalize_vec(&light_v);
	itx->normal.w = 0;
	light_dot_normal = dot_product(&light_v, &itx->normal);
	if (light_dot_normal < 0 || is_shadowed(scene, light_idx, &itx->over_point))
		return (false);
	mult_color(&phong->diffuse, &phong->effective_color, light_dot_normal
		* itx->shape->props.diffuse * scene->lights[light_idx].intensity);
	negate_vec(&light_v, &light_v);
	reflect_vector(&reflect_v, &light_v, &itx->normal);
	calculate_specular(&reflect_v, itx, phong, &scene->lights[light_idx]);
	return (true);
}
