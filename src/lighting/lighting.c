/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 12:39:00 by mkhan             #+#    #+#             */
/*   Updated: 2023/01/02 14:37:07 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	reflect(t_vector *res, t_vector *in_vector, t_vector *normal)
{
	scale_vec(res, normal, dot_product(in_vector, normal) * 2);
	sub_vec(res, in_vector, res);
}

t_color	get_ambient(t_color *effective_color,
	t_light *light, t_scene *scene)
{
	t_color	ambient;

	ambient.a = 0;
	mult_color(&ambient, effective_color,
		scene->ambient.intensity * light->intensity);
	blend_colors(&ambient, &ambient, &scene->ambient.color);
	return (ambient);
}

// light attenuation example code
// float	distance_from_light = distance from itx_point to light pos
// mult_color(diffuse, diffuse, (50 - distance_from_light) / (50 - 1));
// mult_color(specular, specular, (50 - distance_from_light) / (50 - 1));
bool	get_specular_and_diffuse(t_scene *scene, int light_idx,
	t_intersection *itx, t_color *diffuse, t_color *effective_color,
	t_color *specular)
{
	float		reflect_dot_eye;
	float		light_dot_normal;
	t_vector	light_v;
	t_vector	reflect_v;

	sub_vec(&light_v, &scene->lights[light_idx].position, &itx->over_point);
	light_v.w = 0;
	normalize_vec(&light_v);
	itx->normal.w = 0;
	light_dot_normal = dot_product(&light_v, &itx->normal);
	if (light_dot_normal < 0 || is_shadowed(scene, light_idx, &itx->over_point))
		return (false);
	mult_color(diffuse, effective_color,
		itx->shape->props.diffuse * light_dot_normal
		* scene->lights[light_idx].intensity);
	negate_vec(&light_v, &light_v);
	reflect(&reflect_v, &light_v, &itx->normal);
	reflect_dot_eye = dot_product(&reflect_v, &itx->eye);
	if (reflect_dot_eye <= 0)
		ft_bzero(specular, sizeof(t_color));
	else
		mult_color(specular, &scene->lights[light_idx].color,
			itx->shape->props.specular * \
			pow(reflect_dot_eye, itx->shape->props.shininess)
			* scene->lights[light_idx].intensity);
	return (true);
}

t_color	lighting(t_intersection *itx, t_scene *scene, int light_idx)
{
	t_color		effective_color;
	t_color		diffuse;
	t_color		specular;
	t_color		result;

	blend_colors(&effective_color, &itx->shape->props.color,
		&scene->lights[light_idx].color);
	if (get_specular_and_diffuse(scene, light_idx, itx, &diffuse,
			&effective_color, &specular) == false)
		return (get_ambient(&effective_color,
				&scene->lights[light_idx], scene));
	result = get_ambient(&effective_color, &scene->lights[light_idx], scene);
	result.r += diffuse.r + specular.r;
	result.g += diffuse.g + specular.g;
	result.b += diffuse.b + specular.b;
	return (result);
}

static t_color	get_lighting(t_scene *scene, t_intersection *itx, int light_idx)
{
	t_color	surface_color;
	t_color	reflected;
	t_color	refracted;
	t_color	color;
	float	reflectance;

	ft_bzero(&color, sizeof(t_color));
	surface_color = lighting(itx, scene, light_idx);
	reflected = reflected_color(scene, itx, scene->settings.reflection_depth,
			light_idx);
	refracted = refracted_color(scene, itx, scene->settings.refraction_depth,
			light_idx);
	if (itx->shape->props.reflectiveness > 0
		&& itx->shape->props.transparency > 0)
	{
		reflectance = schlick(itx);
		mult_color(&reflected, &reflected, reflectance);
		mult_color(&refracted, &refracted, (1 - reflectance));
	}
	add_colors(&color, &color, &surface_color);
	add_colors(&color, &color, &reflected);
	add_colors(&color, &color, &refracted);
	return (color);
}

t_color	calculate_lighting(t_intersections *arr, t_scene *scene, t_ray *ray)
{
	t_intersection	*itx;
	int				light_idx;
	t_color			final_color;
	t_color			light_color;

	if (scene->settings.refraction_depth != 0)
	{
		sort_intersections(arr);
		itx = hit_sorted(arr);
	}
	else
		itx = hit(arr);
	ft_bzero(&final_color, sizeof(t_color));
	if (itx != NULL)
	{
		prepare_computations(scene, itx, ray, arr);
		light_idx = 0;
		while (light_idx < scene->count.lights)
		{
			light_color = get_lighting(scene, itx, light_idx);
			add_colors(&final_color, &final_color, &light_color);
			light_idx++;
		}
	}
	return (final_color);
}
