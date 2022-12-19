/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflections.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 16:59:06 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/19 17:56:02 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_color	calculate_reflected_color(t_intersections *arr, t_scene *scene, t_ray *ray, int remaining, int light_idx)
{
	t_intersect		*itx;
	t_color			final_color;
	t_color			light_color;

	ft_bzero(&final_color, sizeof(t_color));
	sort_intersections(arr);
	itx = hit(arr);
	if (itx != NULL)
	{
		prepare_computations(itx, ray, arr);
		light_color = lighting(itx, scene, light_idx);
		t_color	reflected = reflected_color(scene, itx, remaining - 1, light_idx);
		add_colors(&final_color, &final_color, &reflected);
		add_colors(&final_color, &final_color, &light_color);
	}
	return (final_color);
}

t_color	reflected_color(t_scene *scene, t_intersect *intersection, int remaining, int light_idx)
{
	t_color			color;
	t_ray			ray;
	t_intersections	arr;
	unsigned int	shape_idx;

	if (intersection->shape->reflectiveness == 0 || remaining == 0)
	{
		ft_bzero(&color, sizeof(t_color));
		return (color);
	}
	ft_memcpy(&ray.origin, &intersection->over_point, sizeof(t_vector));
	ft_memcpy(&ray.direction, &intersection->reflect_vec, sizeof(t_vector));
	shape_idx = -1;
	arr.count = 0;
	while (++shape_idx < scene->count.shape_count)
		intersect(&scene->shapes[shape_idx], &ray, &arr);
	t_color reflected = calculate_reflected_color(&arr, scene, &ray, remaining, light_idx);
	mult_color(&reflected, &reflected, intersection->shape->reflectiveness);
	return (reflected);
}

void	refracted_ray(t_ray	*refract_ray, t_intersect *intersection, double n_ratio, double cos_i)
{
	t_vector normal_v;
	t_vector eye_v;
	double 	cos_t;
	double	sin2_t;

	sin2_t = (n_ratio * n_ratio) * (1 - (cos_i * cos_i));
	cos_t = sqrt(1.0 - sin2_t);
	scale_vec(&normal_v, &intersection->normal, (n_ratio * (cos_i - cos_t)));
	scale_vec(&eye_v, &intersection->eye, n_ratio);
	sub_vec(&refract_ray->direction, &normal_v, &eye_v);
	refract_ray->origin = intersection->under_point;
}

t_color	calculate_refracted_color(t_intersections *arr, t_scene *scene, t_ray *ray, int remaining, int light_idx)
{
	t_intersect		*itx;
	t_color			final_color;
	t_color			light_color;

	ft_bzero(&final_color, sizeof(t_color));
	sort_intersections(arr);
	itx = hit(arr);
	if (itx != NULL)
	{
		prepare_computations(itx, ray, arr);
		light_color = lighting(itx, scene, light_idx);
		t_color	refracted = refracted_color(scene, itx, remaining - 1, light_idx);
		add_colors(&final_color, &final_color, &refracted);
		add_colors(&final_color, &final_color, &light_color);
	}
	return (final_color);
}

t_color	refracted_color(t_scene *scene, t_intersect *intersection, int	remaining, int light_idx)
{
	t_color	color;
	double	n_ratio;
	double	cos_i;
	double	sin2_t;
	t_intersections	arr;
	unsigned int	shape_idx;
	t_ray	refract_ray;

	if (intersection->shape->transparency == 0 || remaining == 0)
	{
		ft_bzero(&color, sizeof(t_color));
		return (color);
	}
	color.r = 1;
	color.g = 1;
	color.b = 1;
	n_ratio = intersection->n1 / intersection->n2;
	cos_i = dot_product(&intersection->eye, &intersection->normal);
	sin2_t = (n_ratio * n_ratio) * (1 - (cos_i * cos_i));
	if (sin2_t > 1)
	{
		ft_bzero(&color, sizeof(t_color));
		return (color);
	}
	refracted_ray(&refract_ray, intersection, n_ratio, cos_i);
	shape_idx = -1;
	arr.count = 0;
	while (++shape_idx < scene->count.shape_count)
		intersect(&scene->shapes[shape_idx], &refract_ray, &arr);
	color = calculate_refracted_color(&arr, scene, &refract_ray, remaining, light_idx);
	mult_color(&color, &color, intersection->shape->transparency);
	return(color);
}