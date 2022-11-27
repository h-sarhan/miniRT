/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 12:39:00 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/27 14:08:40 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// function reflect(in, normal)
// return in - normal * 2 * dot(in, normal)

void	reflect(t_vector *res, t_vector *in_vector, t_vector *normal)
{
	double	dot_res;
	
	dot_res = dot_product(in_vector, normal);
	scale_vec(res, normal, dot_res * 2);
	sub_vec(res, in_vector, res);
	// res->w = 0;
}

t_color	lighting(t_intersect *intersection, t_scene *scene, int light_idx)
{
	t_color		effective_color;
	t_vector	light_v;
	t_color		ambient;
	t_color		diffuse;
	t_color		specular;
	double		light_dot_normal;
	double		reflect_dot_eye;
	t_vector	reflect_v;
	t_color		result;
	
	// effective color calculation
	blend_colors(&effective_color, &intersection->shape->color, &scene->lights[light_idx].color);
	// light_v calculation
	sub_vec(&light_v, &scene->lights[light_idx].position, &intersection->point);
	light_v.w = 0;
	normalize_vec(&light_v);
	// ambient calculation
	mult_color(&ambient, &effective_color, scene->ambient.intensity * scene->lights[light_idx].intensity);
	blend_colors(&ambient, &ambient, &scene->ambient.color);
	intersection->normal.w = 0;
	light_dot_normal = dot_product(&light_v, &intersection->normal);
	if (light_dot_normal < 0)
	{
		ft_bzero(&diffuse, sizeof(t_color));
		ft_bzero(&specular, sizeof(t_color));
	}
	else
	{
		// Greater the angle lesser the diffuse. 
		mult_color(&diffuse, &effective_color, intersection->shape->diffuse * light_dot_normal * scene->lights[light_idx].intensity);
		light_v.x = -light_v.x;
		light_v.y = -light_v.y;
		light_v.z = -light_v.z;
		light_v.w = -light_v.w;
		reflect(&reflect_v, &light_v, &intersection->normal);
		reflect_dot_eye = dot_product(&reflect_v, &intersection->eye);
		if (reflect_dot_eye <= 0)
			ft_bzero(&specular, sizeof(t_color));
		else
			mult_color(&specular, &scene->lights[light_idx].color, intersection->shape->specular * powf(reflect_dot_eye, intersection->shape->shininess) * scene->lights[light_idx].intensity);
	}
	// add_colors(&result, &ambient, &diffuse);
	// add_colors(&result, &result, &specular);
	result.r = ambient.r + diffuse.r + specular.r;
	result.g = ambient.g + diffuse.g + specular.g;
	result.b = ambient.b + diffuse.b + specular.b;
	return(result);
}