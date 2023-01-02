/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflections.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:47:11 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/02 18:06:20 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	reflect_vector(t_vector *res, t_vector *in_vector, t_vector *normal)
{
	scale_vec(res, normal, dot_product(in_vector, normal) * 2);
	sub_vec(res, in_vector, res);
}

t_color	calculate_reflected_color(t_intersections *arr, t_scene *scene,
		t_ray *ray, int remaining, int light_idx)
{
	t_intersection	*itx;
	t_color			final_color;
	t_color			light_color;
	t_color			reflected;

	ft_bzero(&final_color, sizeof(t_color));
	itx = hit(arr);
	if (itx != NULL)
	{
		prepare_computations(itx, ray);
		light_color = phong(itx, scene, light_idx);
		reflected = reflected_color(scene, itx, remaining - 1, light_idx);
		add_colors(&final_color, &final_color, &light_color);
		add_colors(&final_color, &final_color, &reflected);
	}
	return (final_color);
}

t_color	reflected_color(t_scene *scene, t_intersection *intersection,
		int remaining, int light_idx)
{
	t_color			color;
	t_ray			ray;
	t_intersections	arr;
	int				shape_idx;
	t_color			reflected;

	if (intersection->shape->props.reflectiveness == 0 || remaining == 0)
	{
		ft_bzero(&color, sizeof(t_color));
		return (color);
	}
	ray.origin = intersection->over_point;
	ray.direction = intersection->reflect_vec;
	shape_idx = -1;
	arr.count = 0;
	while (++shape_idx < scene->count.shapes)
		intersect(&scene->shapes[shape_idx], &ray, &arr);
	reflected = calculate_reflected_color(&arr, scene, &ray, remaining,
			light_idx);
	mult_color(&reflected, &reflected,
		intersection->shape->props.reflectiveness);
	return (reflected);
}
