/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phong.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:49:56 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/05 19:13:29 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_color	get_ambient(t_phong *phong, t_light *light, t_scene *scene)
{
	t_color	ambient;

	ambient.a = 0;
	mult_color(&ambient, &phong->effective_color,
		scene->ambient.intensity * light->intensity);
	blend_colors(&ambient, &ambient, &scene->ambient.color);
	return (ambient);
}

// light attenuation example code
// float	distance_from_light = vec_distance(&itx->point, &scene->lights[light_idx].position);
// mult_color(&phong->diffuse, &phong->diffuse, (30 - distance_from_light) / (30 - 1));
// mult_color(&phong->specular, &phong->specular, (30 - distance_from_light) / (30 - 1));
bool	get_specular_and_diffuse(t_scene *scene, int light_idx,
	t_intersection *itx, t_phong *phong)
{
	float		reflect_dot_eye;
	float		light_dot_normal;
	t_vector	light_v;
	t_vector	reflect_v;

	sub_vec(&light_v, &scene->lights[light_idx].position, &itx->over_point);
	normalize_vec(&light_v);
	itx->normal.w = 0;
	light_dot_normal = dot_product(&light_v, &itx->normal);
	if (light_dot_normal < 0 || is_shadowed(scene, light_idx, &itx->over_point))
		return (false);
	mult_color(&phong->diffuse, &phong->effective_color,
		itx->shape->props.diffuse * light_dot_normal
		* scene->lights[light_idx].intensity);
	negate_vec(&light_v, &light_v);
	reflect_vector(&reflect_v, &light_v, &itx->normal);
	reflect_dot_eye = dot_product(&reflect_v, &itx->eye);
	if (reflect_dot_eye <= 0)
		ft_bzero(&phong->specular, sizeof(t_color));
	else
		mult_color(&phong->specular, &scene->lights[light_idx].color,
			itx->shape->props.specular * \
			pow(reflect_dot_eye, itx->shape->props.shininess)
			* scene->lights[light_idx].intensity);
	return (true);
}

t_color	phong(t_intersection *itx, t_scene *scene, int light_idx)
{
	t_phong		phong;
	t_color		result;

	blend_colors(&phong.effective_color, &itx->shape->props.color,
		&scene->lights[light_idx].color);
	if (get_specular_and_diffuse(scene, light_idx, itx, &phong) == false)
		return (get_ambient(&phong,
				&scene->lights[light_idx], scene));
	result = get_ambient(&phong, &scene->lights[light_idx], scene);
	result.r += phong.diffuse.r + phong.specular.r;
	result.g += phong.diffuse.g + phong.specular.g;
	result.b += phong.diffuse.b + phong.specular.b;
	return (result);
}

t_color	shade_point(t_intersections *arr, t_scene *scene, t_ray *ray)
{
	t_intersection	*itx;
	int				light_idx;
	t_color			final_color;
	t_color			surface_color;
	t_color			reflected;

	itx = hit(arr);
	ft_bzero(&final_color, sizeof(t_color));
	if (itx != NULL)
	{
		prepare_computations(itx, ray);
		light_idx = 0;
		while (light_idx < scene->count.lights)
		{
			surface_color = phong(itx, scene, light_idx);
			reflected = cast_reflection_ray(scene, itx,
					scene->settings.reflection_depth, light_idx);
			add_colors(&final_color, &final_color, &surface_color);
			add_colors(&final_color, &final_color, &reflected);
			light_idx++;
		}
	}
	return (final_color);
}

bool	is_shadowed(t_scene *scene, int light_idx, t_vector *itx_point)
{
	float			distance;
	t_ray			ray;
	t_intersections	arr;
	int				i;
	t_intersection	*intersection;

	sub_vec(&ray.direction, &scene->lights[light_idx].position, itx_point);
	distance = vec_magnitude(&ray.direction);
	scale_vec(&ray.direction, &ray.direction, 1 / distance);
	ray.origin = *itx_point;
	i = -1;
	arr.count = 0;
	while (++i < scene->count.shapes)
		intersect(&scene->shapes[i], &ray, &arr);
	intersection = hit(&arr);
	if (intersection && intersection->time < distance)
		return (true);
	return (false);
}
