/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 22:29:30 by mkhan             #+#    #+#             */
/*   Updated: 2023/02/20 00:45:36 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vector	get_texture_coords(const t_shape *shape, double u, double v,
				t_vector *normal)
{
	t_color		normal_coords;

	if (shape->tex_tile != 0)
	{
		u = (int)floor(u * (shape->tex_height - 1) * shape->tex_tile) \
			% shape->tex_height;
		v = (int)floor(v * (shape->tex_width - 1) * shape->tex_tile) \
			% shape->tex_width;
	}
	else
	{
		u = (int)floor(u * (shape->tex_height - 1));
		v = (int)floor(v * (shape->tex_width - 1));
	}
	if (u >= shape->tex_height || v >= shape->tex_width)
		return (*normal);
	normal_coords = shape->normal_tex[(int)u][(int)v];
	normal->w = 0;
	normal->x = 2 * normal_coords.r - 1;
	normal->y = 2 * normal_coords.g - 1;
	normal->z = 2 * normal_coords.b - 1;
	normalize_vec(normal);
	return (*normal);
}

t_vector	normal_at(const t_shape *shape, const t_vector *itx_point)
{
	t_vector	normal;
	t_vector	world_normal;

	if (shape->type == SPHERE)
		return (sphere_normal(&normal, shape, itx_point));
	else if (shape->type == PLANE)
		return (plane_normal(shape));
	else if (shape->type == CYLINDER)
		return (cylinder_normal(shape, itx_point));
	else if (shape->type == CONE)
		return (cone_normal(shape, itx_point));
	normal = cube_normal(shape, itx_point);
	mat_vec_multiply(&world_normal, &shape->norm_transf, &normal);
	world_normal.w = 0;
	normalize_vec(&world_normal);
	return (world_normal);
}
