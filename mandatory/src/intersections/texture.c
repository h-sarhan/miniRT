/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 22:29:30 by mkhan             #+#    #+#             */
/*   Updated: 2023/02/19 22:30:39 by mkhan            ###   ########.fr       */
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

t_vector	normal_from_texture(const t_shape *shape, const t_vector *itx_point)
{
	t_vector	shape_point;
	double		u;
	double		v;
	t_vector	normal;

	ft_bzero(&normal, sizeof(t_vector));
	normal.y = 1;
	mat_vec_multiply(&shape_point, &shape->inv_transf, itx_point);
	if (shape->type == SPHERE)
		spherical_map(&u, &v, &shape_point);
	else if (shape->type == CYLINDER || shape->type == CONE)
	{
		shape_point.y /= shape->props.height;
		shape_point.y -= 0.5;
		cylindrical_map(&u, &v, &shape_point);
	}
	else
		cubical_map(&u, &v, &shape_point);
	if (u < 0 || v < 0)
		return (normal);
	return (get_texture_coords(shape, u, v, &normal));
}

t_vector	normal_at(const t_shape *shape, const t_vector *itx_point)
{
	t_vector	normal;
	t_vector	world_normal;

	if (shape->type == SPHERE)
		return (sphere_normal(&normal, shape, itx_point));
	else if (shape->type == PLANE)
		return (plane_normal(shape, itx_point));
	else if (shape->type == CYLINDER)
		return (cylinder_normal(shape, itx_point));
	else if (shape->type == CONE)
		return (cone_normal(shape, itx_point));
	normal = cube_normal(shape, itx_point);
	if (shape->normal_tex != NULL)
		return (normal_map(&normal, shape, itx_point));
	mat_vec_multiply(&world_normal, &shape->norm_transf, &normal);
	world_normal.w = 0;
	normalize_vec(&world_normal);
	return (world_normal);
}
