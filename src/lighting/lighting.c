/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 12:39:00 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/23 16:11:52 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// function reflect(in, normal)
// return in - normal * 2 * dot(in, normal)

void	reflect(t_vector *res, t_vector *in_vector, t_vector *normal)
{
	float	dot_res;
	
	dot_res = dot_product(in_vector, normal);
	scale_vec(res, normal, dot_res * 2);
	sub_vec(res, in_vector, res);
	res->w = 0;
}

t_color	lighting(t_shape *shape, t_scene *scene, t_vector *point, t_vector *eye_v, t_vector *normal_v)
{
	t_color		effective_color;
	t_vector	light_v;
	t_color		ambient;
	t_color		diffuse;
	t_color		specular;
	float		light_dot_normal;
	float		reflect_dot_eye;
	t_vector	reflect_v;
	t_color		result;
	
	// effective color calculation
	blend_colors(&effective_color, &shape->color, &scene->lights[0].color);
	// light_v calculation
	sub_vec(&light_v, &scene->lights[0].position, point);
	normalize_vec(&light_v);
	// ambient calculation
	mult_color(&ambient, &effective_color, scene->ambient.intensity);
	light_dot_normal = dot_product(&light_v, normal_v);
	if (light_dot_normal < 0)
	{
		ft_bzero(&diffuse, sizeof(t_color));
		ft_bzero(&specular, sizeof(t_color));
	}
	else
	{
		// Greater the angle lesser the diffuse. 
		mult_color(&diffuse, &effective_color, shape->diffuse * light_dot_normal);
		light_v.x = -light_v.x;
		light_v.y = -light_v.y;
		light_v.z = -light_v.z;
		reflect(&reflect_v, &light_v, normal_v);
		reflect_dot_eye = dot_product(&reflect_v, eye_v);
		if (reflect_dot_eye <= 0)
			ft_bzero(&specular, sizeof(t_color));
		else
			mult_color(&specular, &scene->lights[0].color, shape->specular * powf(reflect_dot_eye, shape->shininess));
	}
	add_colors(&result, &ambient, &diffuse);
	add_colors(&result, &result, &specular);
	return(result);
}