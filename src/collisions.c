/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 11:17:32 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/24 12:01:27 by hsarhan          ###   ########.fr       */
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
	normal.w = 0;
	float d = - (dot_product(&normal, &plane->origin));
	float distance = (dot_product(&normal, &sphere->origin) + d);
	if (fabs(distance) < sphere->radius)
		return (true);
	return (false);
}

void	sphere_sphere_collision_resolution(t_shape *sphere1, t_shape *sphere2, t_scene *scene)
{
	t_vector	dir;

	sub_vec(&dir, &sphere1->origin, &sphere2->origin);
	float dist = vec_magnitude(&dir);
	normalize_vec(&dir);
	scale_vec(&dir, &dir,  dist - (sphere1->radius + sphere2->radius) + 0.001);
	add_vec(&sphere2->origin, &sphere2->origin, &dir);
	sphere1->is_colliding = true;
	collide(sphere2, scene);
	sphere1->is_colliding = false;
}

void	sphere_plane_collision_resolution(t_shape *sphere, t_shape *plane)
{
	t_vector	resolution;
	t_vector	origin_to_plane;
	double		distance;

	sub_vec(&origin_to_plane, &sphere->origin, &plane->origin);
	distance = sphere->radius - fabs(dot_product(&origin_to_plane, &plane->orientation));
	resolution = plane->orientation;
	scale_vec(&resolution, &resolution, distance + 0.001);
	add_vec(&sphere->origin, &sphere->origin, &resolution);
}

bool	cylinder_plane_collision(t_shape *cylinder, t_shape *plane)
{
	
	t_vector	cylinder_normal;
	mat_vec_multiply(&cylinder_normal, &cylinder->transf, &cylinder->orientation);
	t_vector	cylinder_to_plane;
	sub_vec(&cylinder_to_plane, &cylinder->origin, &plane->origin);
	double	cylinder_to_plane_proj;
	cylinder_to_plane_proj = fabs(dot_product(&plane->orientation, &cylinder_to_plane));
	double	normal_dot_product;
	normal_dot_product = fabs(dot_product(&plane->orientation, &cylinder_normal));
	if (cylinder_to_plane_proj <= cylinder->radius * sqrt(1 - normal_dot_product * normal_dot_product) + (cylinder->height / 2) * normal_dot_product)
	{
		return (true);
	}
	return (false);
}

bool	cylinder_sphere_collision(t_shape *cylinder, t_shape *sphere, bool cylinder_sphere)
{
	t_vector	resolution;
	t_vector	cylinder_to_sphere;
	t_vector	cylinder_normal;
	mat_vec_multiply(&cylinder_normal, &cylinder->transf, &cylinder->orientation);
	sub_vec(&cylinder_to_sphere, &cylinder->origin, &sphere->origin);
	normalize_vec(&cylinder_normal);
	float		v_dist = dot_product(&cylinder_to_sphere, &cylinder_normal);
	t_vector	top_cap_center;
	t_vector	bottom_cap_center;

	scale_vec(&top_cap_center, &cylinder_normal, cylinder->height / 2);
	add_vec(&top_cap_center, &top_cap_center, &cylinder->origin);
	scale_vec(&bottom_cap_center, &cylinder_normal, -cylinder->height / 2);
	add_vec(&bottom_cap_center, &bottom_cap_center, &cylinder->origin);

	t_vector	cap_center;
	if (v_dist < - cylinder->height / 2)
	{
		cap_center = top_cap_center;
	}
	else if (v_dist > cylinder->height / 2)
	{
		cap_center = bottom_cap_center;
	}
	if (v_dist < - cylinder->height / 2 || v_dist > cylinder->height / 2)
	{
		t_vector	cap_to_sphere;

		sub_vec(&cap_to_sphere, &cap_center, &sphere->origin);
		float	dist = vec_magnitude(&cap_to_sphere);
		float	v_cap_distance = fabs(fabs(v_dist) - (cylinder->height / 2));
		float	h_cap_distance = sqrt(dist * dist - v_cap_distance * v_cap_distance);
		if (h_cap_distance < cylinder->radius && v_cap_distance < sphere->radius)
		{
			resolution = cylinder_normal;
			if (v_dist > cylinder->height / 2)
				negate_vec(&resolution, &resolution);
			scale_vec(&resolution, &resolution, sphere->radius - v_cap_distance + 0.001);
			if (cylinder_sphere == true)
				add_vec(&sphere->origin, &sphere->origin, &resolution);
			else
				sub_vec(&cylinder->origin, &cylinder->origin, &resolution);
			return true;
		}
		t_vector	dir;
		scale_vec(&dir, &cylinder_normal, v_cap_distance);
		if (v_dist < - cylinder->height / 2)
		{
			negate_vec(&dir, &dir);
		}
		add_vec(&dir, &cap_center, &dir);
		sub_vec(&dir, &sphere->origin, &dir);
		normalize_vec(&dir);
		scale_vec(&dir, &dir, cylinder->radius);
		t_vector	edge;
		add_vec(&edge, &cap_center, &dir);
		float	edge_distance = vec_distance(&edge, &sphere->origin);
		if (edge_distance < sphere->radius && v_cap_distance < sphere->radius)
		{
			sub_vec(&resolution, &edge, &sphere->origin);
			negate_vec(&resolution, &resolution);
			normalize_vec(&resolution);
			scale_vec(&resolution, &resolution, sphere->radius - edge_distance);
			if (cylinder_sphere == true)
				add_vec(&sphere->origin, &sphere->origin, &resolution);
			else
				sub_vec(&cylinder->origin, &cylinder->origin, &resolution);
			return true;
		}
	}
	else
	{
		t_vector	center_delta;
		scale_vec(&center_delta, &cylinder_normal, -(v_dist));
		t_vector	center_adjusted;
		add_vec(&center_adjusted, &cylinder->origin, &center_delta);
		float	dist = vec_distance(&center_adjusted, &sphere->origin);
		if (dist < (cylinder->radius + sphere->radius + 0.001))
		{
			sub_vec(&resolution, &center_adjusted, &sphere->origin);
			negate_vec(&resolution, &resolution);
			normalize_vec(&resolution);
			scale_vec(&resolution, &resolution, sphere->radius + cylinder->radius - dist);
			if (cylinder_sphere == true)
				add_vec(&sphere->origin, &sphere->origin, &resolution);
			else
				sub_vec(&cylinder->origin, &cylinder->origin, &resolution);
			return true;
		}
	}
	// cylinder->is_colliding = false;
	return (false);
}

void	collide(t_shape *shape, t_scene *scene)
{
	t_shape			*other;
	unsigned int	shape_idx;
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
					sphere_plane_collision_resolution(shape, other);
				}
			}
			else if (shape->type == CYLINDER && other->type == PLANE)
			{
				if (cylinder_plane_collision(shape, other) == true && shape->is_colliding == false)
				{
					t_vector	cylinder_normal;
					t_vector	top_cap_center;
					t_vector	bottom_cap_center;

					mat_vec_multiply(&cylinder_normal, &shape->added_rots, &shape->orientation);
					normalize_vec(&cylinder_normal);
					scale_vec(&top_cap_center, &cylinder_normal, shape->height / 2);
					add_vec(&top_cap_center, &top_cap_center, &shape->origin);
					scale_vec(&bottom_cap_center, &cylinder_normal, -shape->height / 2);
					add_vec(&bottom_cap_center, &bottom_cap_center, &shape->origin);
					t_vector	cap_to_plane;
					sub_vec(&cap_to_plane, &top_cap_center, &other->origin);
					double d1 = fabs(dot_product(&cap_to_plane, &other->orientation));
					sub_vec(&cap_to_plane, &bottom_cap_center, &other->origin);
					double d2 = fabs(dot_product(&cap_to_plane, &other->orientation));
					t_vector	cap_center;
					if (d1 < d2)
						cap_center = top_cap_center;
					else
						cap_center = bottom_cap_center;
					// Form a secondary plane at the cylinder cap
					// This plane will have the equation
					// Ax + By + Cz + D = 0
					// A, B, C are the xyz values of the normal to the plane.
					// D can be found by plugging a point in. The point will just be the cylinder cap
					float		d = -(dot_product(&cylinder_normal, &cap_center));
					// Normal of the secondary plane is the normal of the cylinder
					// We intersect a ray starting from the cylinder center in the direction of motion
					t_ray ray;
					ray.origin = shape->origin;
					// This should be the vector from the cylinder center to the plane
					negate_vec(&ray.direction, &other->orientation);
					normalize_vec(&ray.direction);
					if (fabs(dot_product(&cylinder_normal, &ray.direction)) > 0.001)
					{
						double	t = -(dot_product(&cylinder_normal, &ray.origin) + d) / dot_product(&cylinder_normal, &ray.direction);
						t_vector	point_on_secondary_plane;
						ray_position(&point_on_secondary_plane, &ray, t);
						t_vector	dir;
						sub_vec(&dir, &point_on_secondary_plane, &cap_center);
						if (vec_magnitude(&dir) > 0.001)
							normalize_vec(&dir);
						scale_vec(&dir, &dir, shape->radius + 0.01);
						t_vector	end_point;
						add_vec(&end_point, &cap_center, &dir);
						t_vector	plane_to_end_point;
						sub_vec(&plane_to_end_point, &end_point, &other->origin);
						double	dist = fabs(dot_product(&plane_to_end_point, &other->orientation));
						t_vector	resolution;
						// resolution = *offset;
						// negate_vec(&resolution, &other->orientation);
						resolution = other->orientation;
						scale_vec(&resolution, &resolution, dist);
						add_vec(&shape->origin, &shape->origin, &resolution);
					}
					else
					{
						// This is the case when the direction of motion is perpendicular to the cylinder normal
						t_vector	center_to_point;
						sub_vec(&center_to_point, &other->origin, &shape->origin);
						
						double	dist = fabs(dot_product(&center_to_point, &other->orientation));
						t_vector	resolution;
						resolution = other->orientation;
						// negate_vec(&resolution, &other->orientation);
						// resolution = *offset;
						// normalize_vec(&resolution);
						scale_vec(&resolution, &resolution, shape->radius - dist + 0.001);
						add_vec(&shape->origin, &shape->origin, &resolution);
					}
				}
			}
			else if (shape->type == CYLINDER && other->type == SPHERE && shape->is_colliding == false)
			{
				shape->is_colliding = true;
				if (cylinder_sphere_collision(shape, other, true) == true)
					collide(other, scene);
				shape->is_colliding = false;
			}
			else if (shape->type == SPHERE && other->type == CYLINDER && shape->is_colliding == false)
			{
				// shape->is_colliding = true;
				// cylinder_sphere_collision(other, shape, false);
				// collide(shape, scene);
				// shape->is_colliding = false;
			}
			// t_vector	local_sphere_origin;
			// mat_vec_multiply(&local_sphere_origin, &shape->inv_transf, &other->origin);
			// print_vector(&local_sphere_origin);
			// float	dist = sqrt(local_sphere_origin.x * local_sphere_origin.x + local_sphere_origin.z * local_sphere_origin.z);
			// printf("Distance from center line to sphere origin == %f\n", dist);
		}
		shape_idx++;
	}
}
