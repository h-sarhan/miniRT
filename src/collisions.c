/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 11:17:32 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/23 15:32:21 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// HANDLE OBJECTS THAT ARE ALREADY COLLIDING WHEN THE SCENE STARTS
bool	sphere_sphere_collision(const t_shape *sphere1, const t_shape *sphere2)
{
	float		distance;
	t_vector	center_diff;

	sub_vec(&center_diff, &sphere1->origin, &sphere2->origin);
	distance = vec_magnitude(&center_diff);
	if (distance <= (sphere1->radius + sphere2->radius))
		return (true);
	return (false);
}

bool	sphere_plane_collision(t_shape *sphere, const t_shape *plane)
{
	t_vector	normal;
	normal = plane->orientation;
	float d = -(normal.x * plane->origin.x + normal.y * plane->origin.y + normal.z * plane->origin.z);
	float distance = (normal.x * sphere->origin.x + normal.y *  sphere->origin.y + normal.z *  sphere->origin.z + d);
	if (fabs(distance) < sphere->radius)
		return (true);
	return (false);
}

void	sphere_sphere_collision_resolution(t_shape *shape, t_shape *other, const t_scene *scene)
{
	t_vector	dir;

	sub_vec(&dir, &shape->origin, &other->origin);
	float dist = vec_magnitude(&dir);
	normalize_vec(&dir);
	scale_vec(&dir, &dir,  dist - (shape->radius + other->radius));
	add_vec(&other->origin, &other->origin, &dir);
	shape->is_colliding = true;
	collide_translate(other, scene, &dir);
	shape->is_colliding = false;
}

void	collide_scale(t_shape *shape, const t_scene *scene,
		float radius, float height, float width)
{
	t_shape			*other;
	unsigned int	shape_idx;
	shape_idx = 0;
	(void)height;
	(void)width;
	while (shape_idx < scene->count.shapes)
	{
		other = &scene->shapes[shape_idx];
		if (other != shape)
		{
			if (shape->type == SPHERE && other->type == SPHERE)
			{
				if (sphere_sphere_collision(shape, other) == true && other->is_colliding == false)
				{
					sphere_sphere_collision_resolution(shape, other, scene);
				}
			}
			else if (shape->type == SPHERE && other->type == PLANE)
			{
				if (sphere_plane_collision(shape, other) == true && shape->is_colliding == false)
				{
					shape->radius -= radius;
				}
			}
			// else if (shape->type == CYLINDER && other->type == PLANE)
			// {
			// 	if (cylinder_plane_collision(shape, other) == true && shape->is_colliding == false)
			// 	{
			// 		t_vector	offset_copy;
			// 		scale_vec(&offset_copy, offset, 0.01);
			// 		int	counter = 0;
			// 		while (counter < 200 && cylinder_plane_collision(shape, other) == true)
			// 		{
			// 			sub_vec(&shape->origin, &shape->origin, &offset_copy);
			// 			counter ++;
			// 		}
			// 		// sphere_plane_translate_resolution(shape, other, scene, offset);
			// 		// printf("Collision\n");
			// 	}
			// }
		}
		shape_idx++;
	}
}

void	sphere_plane_translate_resolution(t_shape *shape, t_shape *other, const t_scene *scene, t_vector *offset)
{
	bool	collide_x;
	bool	collide_y;
	bool	collide_z;
	sub_vec(&shape->origin, &shape->origin, offset);
	t_vector	offset_copy = *offset;
	offset_copy.y = 0;
	offset_copy.z = 0;
	add_vec(&shape->origin, &shape->origin, &offset_copy);
	collide_x = sphere_plane_collision(shape, other);
	sub_vec(&shape->origin, &shape->origin, &offset_copy);
	offset_copy = *offset;
	offset_copy.x = 0;
	offset_copy.z = 0;
	add_vec(&shape->origin, &shape->origin, &offset_copy);
	collide_y = sphere_plane_collision(shape, other);
	sub_vec(&shape->origin, &shape->origin, &offset_copy);
	offset_copy = *offset;
	offset_copy.x = 0;
	offset_copy.y = 0;
	add_vec(&shape->origin, &shape->origin, &offset_copy);
	collide_z = sphere_plane_collision(shape, other);
	sub_vec(&shape->origin, &shape->origin, &offset_copy);
	ft_bzero(&offset_copy, sizeof(t_vector));
	if (collide_x == false)
		offset_copy.x = offset->x;
	if (collide_y == false)
		offset_copy.y = offset->y;
	if (collide_z == false)
		offset_copy.z = offset->z;
	if (collide_x && collide_y && collide_z)
		return;
	add_vec(&shape->origin, &shape->origin, &offset_copy);
	scale_vec(&offset_copy, offset, 0.05);
	int push_back_counter = 0;
	while (!sphere_plane_collision(shape, other) && push_back_counter < 200)
	{
		add_vec(&shape->origin, &shape->origin, &offset_copy);
		push_back_counter++;
	}
	sub_vec(&shape->origin, &shape->origin, &offset_copy);
	unsigned int i = 0;
	while (i < scene->count.shapes)
	{
		scene->shapes[i].is_colliding = false;
		i++;
	}
	scale_vec(&offset_copy, offset, 0.01 * push_back_counter);
	if (fabs(offset_copy.x) + fabs(offset_copy.y) + fabs(offset_copy.z) > 0.01)
	{
		shape->is_colliding = true;
		collide_translate(shape, scene, &offset_copy);
		shape->is_colliding = false;
	}
}


bool	cylinder_plane_collision(t_shape *cylinder, t_shape *plane)
{
	// t_vector	top_cap_center;
	// t_vector	bottom_cap_center;
	// t_vector	normal;
	// (void)plane;
	// printf("cUP is \n");
	// mat_vec_multiply(&normal, &cylinder->added_rots, &cylinder->orientation);
	// normalize_vec(&normal);
	// print_vector(&normal);
	// scale_vec(&top_cap_center, &normal, -cylinder->height / 2);
	// add_vec(&top_cap_center, &top_cap_center, &cylinder->origin);
	// scale_vec(&bottom_cap_center, &normal, cylinder->height / 2);
	// add_vec(&bottom_cap_center, &bottom_cap_center, &cylinder->origin);
	// printf("Bottom cap is \n");
	// print_vector(&bottom_cap_center);
	// printf("Top cap is \n");
	// print_vector(&top_cap_center);
	// return (false);
	t_vector	cylinder_normal;
	mat_vec_multiply(&cylinder_normal, &cylinder->transf, &cylinder->orientation);
	// normalize_vec(&cylinder_normal);
	t_vector	cylinder_to_plane;
	sub_vec(&cylinder_to_plane, &cylinder->origin, &plane->origin);
	// normalize_vec(&cylinder_to_plane);
	double	cylinder_to_plane_proj;
	cylinder_to_plane_proj = fabs(dot_product(&plane->orientation, &cylinder_to_plane));
	double	normal_dot_product;
	normal_dot_product = fabs(dot_product(&plane->orientation, &cylinder_normal));
	if (cylinder_to_plane_proj <= cylinder->radius * sqrt(1 - normal_dot_product * normal_dot_product) + (cylinder->height / 2) * normal_dot_product)
	{
		printf("COLLISION\n");
		return (true);
	}
	return (false);
}

void	collide_translate(t_shape *shape, const t_scene *scene, t_vector *offset)
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
					sphere_sphere_collision_resolution(shape, other, scene);
				}
			}
			else if (shape->type == SPHERE && other->type == PLANE)
			{
				if (sphere_plane_collision(shape, other) == true && shape->is_colliding == false)
				{
					sphere_plane_translate_resolution(shape, other, scene, offset);
				}
			}
			else if (shape->type == CYLINDER && other->type == PLANE)
			{
				if (cylinder_plane_collision(shape, other) == true && shape->is_colliding == false)
				{
					t_vector	offset_copy;
					scale_vec(&offset_copy, offset, 0.01);
					int	counter = 0;
					while (counter < 200 && cylinder_plane_collision(shape, other) == true)
					{
						sub_vec(&shape->origin, &shape->origin, &offset_copy);
						counter ++;
					}
					// sphere_plane_translate_resolution(shape, other, scene, offset);
					// printf("Collision\n");
				}
			}
		}
		shape_idx++;
	}
}
