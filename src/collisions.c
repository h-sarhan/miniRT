/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 11:17:32 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/20 22:27:20 by hsarhan          ###   ########.fr       */
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
bool	sphere_plane_collision(t_shape *sphere, const t_shape *plane, t_vector *offset)
{
	t_vector	point_on_plane;
	t_vector	center;
	center.x = 0;
	center.y = 0;
	center.z = 0;
	center.w = 1;
	mat_vec_multiply(&point_on_plane, &plane->transf, &center);
	t_vector	sphere_to_plane;
	sub_vec(&sphere_to_plane, &sphere->origin, &point_on_plane);
	double distance = dot_product(&sphere_to_plane, &plane->orientation);

	if (fabs(distance) < sphere->radius)
	{
		return (true);
	}
	return (false);
}

// Sphere collision response
bool	collide(t_shape *shape, const t_scene *scene, t_vector *offset)
{
	t_shape			*other;
	unsigned int	shape_idx;
	(void)offset;
	shape_idx = 0;
	while (shape_idx < scene->count.shapes)
	{
		other = &scene->shapes[shape_idx];
		if (other != shape)
		{
			if (shape->type == SPHERE && other->type == SPHERE)
			{
				if (sphere_sphere_collision(shape, other) == true && other->is_colliding == false)
				{
					// find direction from s1 to s2
					t_vector	dir;
					sub_vec(&dir, &shape->origin, &other->origin);
					double dist = vec_magnitude(&dir);
					normalize_vec(&dir);
					scale_vec(&dir, &dir,  dist - (shape->radius + other->radius));
					add_vec(&other->origin, &other->origin, &dir);
					shape->is_colliding = true;
					collide(other, scene, &dir);
					shape->is_colliding = false;
				}
			}
			else if (shape->type == SPHERE && other->type == PLANE)
			{
				if (sphere_plane_collision(shape, other, offset) == true && shape->is_colliding == false)
				{
					printf("Colliding\n");
					bool	collide_x;
					bool	collide_y;
					bool	collide_z;
					sub_vec(&shape->origin, &shape->origin, offset);
					t_vector	offset_copy = *offset;
					offset_copy.y = 0;
					offset_copy.z = 0;
					add_vec(&shape->origin, &shape->origin, &offset_copy);
					collide_x = sphere_plane_collision(shape, other, &offset_copy);
					sub_vec(&shape->origin, &shape->origin, &offset_copy);
					offset_copy = *offset;
					offset_copy.x = 0;
					offset_copy.z = 0;
					add_vec(&shape->origin, &shape->origin, &offset_copy);
					collide_y = sphere_plane_collision(shape, other, &offset_copy);
					sub_vec(&shape->origin, &shape->origin, &offset_copy);
					offset_copy = *offset;
					offset_copy.x = 0;
					offset_copy.y = 0;
					add_vec(&shape->origin, &shape->origin, &offset_copy);
					collide_z = sphere_plane_collision(shape, other, &offset_copy);
					sub_vec(&shape->origin, &shape->origin, &offset_copy);
					ft_bzero(&offset_copy, sizeof(t_vector));
					if (collide_x == false)
						offset_copy.x = offset->x;
					if (collide_y == false)
						offset_copy.y = offset->y;
					if (collide_z == false)
						offset_copy.z = offset->z;
					print_vector(&offset_copy);
					add_vec(&shape->origin, &shape->origin, &offset_copy);
					scale_vec(&offset_copy, offset, 0.01);
					int push_back_counter = 0;
					while (!sphere_plane_collision(shape, other, &offset_copy) && push_back_counter < 100)
					{
						print_vector(&offset_copy);
						add_vec(&shape->origin, &shape->origin, &offset_copy);
						push_back_counter++;
					}
					sub_vec(&shape->origin, &shape->origin, &offset_copy);
					int i = 0;
					while (i < scene->count.shapes)
					{
						scene->shapes[i].is_colliding = false;
						i++;
					}
					scale_vec(&offset_copy, offset, 0.01 * push_back_counter);
					shape->is_colliding = true;
					collide(shape, scene, &offset_copy);
					shape->is_colliding = false;
					// return (true);
				}
			}
		}
		shape_idx++;
	}
	return (false);
}
bool	is_colliding(t_shape *shape, const t_scene *scene, t_vector *offset, bool offset_dir)
{
	t_shape	*other;
	unsigned int		shape_idx;

	shape_idx = 0;
	while (shape_idx < scene->count.shapes)
	{
		other = &scene->shapes[shape_idx];
		if (other != shape)
		{
			if (shape->type == SPHERE && other->type == SPHERE)
			{
				if (sphere_sphere_collision(shape, other) == true)
				{
					// We can call is colliding recursively here
					if (offset_dir == true)
					{
						t_vector	dir;
						sub_vec(&dir, &other->origin, &shape->origin);
						printf("direction vector\n");
						print_vector(&dir);
						normalize_vec(&dir);
						scale_vec(&dir, &dir, offset->x);
						printf("offset vector\n");
						print_vector(&dir);
						add_vec(&other->origin, &other->origin, &dir);
					}
					else
					{
						sub_vec(&other->origin, &other->origin, offset);
					}
					return (true);
				}
			}
			else if (shape->type == SPHERE && other->type == PLANE)
			{
				if (sphere_plane_collision(shape, other, offset) == true)
					return (true);
			}
		}
		shape_idx++;
	}
	return (false);
}