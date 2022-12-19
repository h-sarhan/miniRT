/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 11:17:32 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/19 11:58:59 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// HANDLE OBJECTS THAT ARE ALREADY COLLIDING WHEN THE SCENE STARTS

bool	sphere_sphere_collision(const t_shape *sphere1, const t_shape *sphere2)
{
	double		distance;
	t_vector	center_diff;

	sub_vec(&center_diff, &sphere1->origin, &sphere2->origin);
	distance = vec_magnitude(&center_diff);
	if (distance <= (sphere1->radius + sphere2->radius))
		return (true);
	return (false);
}

// From: https://stackoverflow.com/questions/22093749/c-plane-sphere-collision-detection
bool	sphere_plane_collision(const t_shape *sphere, const t_shape *plane)
{
	t_vector	point_on_plane;
	t_vector	center;
	center.x = 0;
	center.y = 0;
	center.z = 0;
	center.w = 1;
	mat_vec_multiply(&point_on_plane, &plane->transf, &center);
	// print_vector(&point_on_plane);
	printf("Sphere origin: \n");
	print_vector(&sphere->origin);
	t_vector	sphere_to_plane;
	sub_vec(&sphere_to_plane, &sphere->origin, &point_on_plane);
	printf("Sphere to plane: \n");
	print_vector(&sphere_to_plane);
	// double distance = (sphere->origin - plane.p[0]) * plane->orientation;
	double distance = dot_product(&sphere_to_plane, &plane->orientation);
	if (fabs(distance) <= sphere->radius)
	{
		return (true);
	}
	return (false);
}

bool	is_colliding(const t_shape *shape, const t_scene *scene)
{
	t_shape	*other;
	int		shape_idx;

	shape_idx = 0;
	while (shape_idx < scene->count.shape_count)
	{
		other = &scene->shapes[shape_idx];
		if (other != shape)
		{
			if (shape->type == SPHERE && other->type == SPHERE)
			{
				if (sphere_sphere_collision(shape, other) == true)
					return (true);
			}
			else if (shape->type == SPHERE && other->type == PLANE)
			{
				if (sphere_plane_collision(shape, other) == true)
					return (true);
			}
		}
		shape_idx++;
	}
	return (false);
}