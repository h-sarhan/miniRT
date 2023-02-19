/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflections.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:47:11 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/19 15:12:00 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "miniRT.h"
#include "scene.h"

void	reflect_vector(t_vector *res, t_vector *in_vector, t_vector *normal)
{
	scale_vec(res, normal, dot_product(in_vector, normal) * 2);
	sub_vec(res, in_vector, res);
}

t_color	reflection_color(t_intersection *itx, t_scene *scene,
		int remaining, int light_idx)
{
	t_color			final_color;
	t_color			light_color;
	t_color			reflected;

	ft_bzero(&final_color, sizeof(t_color));
	light_color = phong(itx, scene, light_idx);
	reflected = cast_reflection_ray(scene, itx, remaining - 1, light_idx);
	add_colors(&final_color, &final_color, &light_color);
	add_colors(&final_color, &final_color, &reflected);
	return (final_color);
}

t_color	cast_reflection_ray(t_scene *scene, t_intersection *intersection,
		int remaining, int light_idx)
{
	t_ray			ray;
	t_intersections	arr;
	int				shape_idx;
	t_color			reflected;
	t_intersection	*itx;

	ft_bzero(&reflected, sizeof(t_color));
	if (intersection->shape->props.reflectiveness == 0 || remaining == 0)
		return (reflected);
	ray.origin = intersection->over_point;
	ray.dir = intersection->reflect_vec;
	arr.count = 0;
	shape_idx = -1;
	while (++shape_idx < scene->count.shapes)
		intersect(&scene->shapes[shape_idx], &ray, &arr);
	itx = hit(&arr);
	if (itx != NULL)
	{
		prepare_computations(itx, &ray);
		reflected = reflection_color(itx, scene, remaining, light_idx);
	}
	mult_color(&reflected, &reflected,
		intersection->shape->props.reflectiveness);
	return (reflected);
}
