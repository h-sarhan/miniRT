/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 11:17:32 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/24 13:25:58 by hsarhan          ###   ########.fr       */
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

void	sphere_sphere_collision_resolution(t_shape *sphere1, t_shape *sphere2)
{
	t_vector	dir;

	sub_vec(&dir, &sphere1->origin, &sphere2->origin);
	float dist = vec_magnitude(&dir);
	normalize_vec(&dir);
	scale_vec(&dir, &dir,  dist - (sphere1->radius + sphere2->radius) - 0.001);
	add_vec(&sphere2->origin, &sphere2->origin, &dir);
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
	t_vector	up;
	ft_bzero(&up, sizeof(t_vector));
	up.y = 1;
	mat_vec_multiply(&cylinder_normal, &cylinder->transf, &up);
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

bool	cylinder_sphere_collision(t_shape *cylinder, t_shape *sphere, bool cylinder_sphere, bool resolve)
{
	t_vector	resolution;
	t_vector	cylinder_to_sphere;
	t_vector	cylinder_normal;
	t_vector	up;
	ft_bzero(&up, sizeof(t_vector));
	up.y = 1;
	mat_vec_multiply(&cylinder_normal, &cylinder->transf, &up);
	if (fabs(vec_magnitude(&cylinder_normal)) < 0.001)
	{
		printf("THIS SHHOULDNT HAPPEN1\n");
		exit(1);
	}
	normalize_vec(&cylinder_normal);
	sub_vec(&cylinder_to_sphere, &cylinder->origin, &sphere->origin);
	float		v_dist = dot_product(&cylinder_to_sphere, &cylinder_normal);
	t_vector	top_cap_center;
	t_vector	bottom_cap_center;

	scale_vec(&top_cap_center, &cylinder_normal, cylinder->height / 2);
	add_vec(&top_cap_center, &top_cap_center, &cylinder->origin);
	scale_vec(&bottom_cap_center, &cylinder_normal, -cylinder->height / 2);
	add_vec(&bottom_cap_center, &bottom_cap_center, &cylinder->origin);

	if (v_dist < - cylinder->height / 2 || v_dist > cylinder->height / 2)
	{
		t_vector	cap_center;
		if (v_dist < - cylinder->height / 2)
		{
			cap_center = top_cap_center;
		}
		else if (v_dist > cylinder->height / 2)
		{
			cap_center = bottom_cap_center;
		}
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
			if (cylinder_sphere == true && resolve)
			{
				scale_vec(&resolution, &resolution, sphere->radius - v_cap_distance - 0.001);
				add_vec(&sphere->origin, &sphere->origin, &resolution);
			}
			else if (resolve)
			{
				scale_vec(&resolution, &resolution, sphere->radius - v_cap_distance + 0.001);
				sub_vec(&cylinder->origin, &cylinder->origin, &resolution);
			}
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
		if (fabs(vec_magnitude(&dir)) < 0.001)
		{
			printf("THIS SHHOULDNT HAPPEN2\\n");
			exit(1);
		}
		normalize_vec(&dir);
		scale_vec(&dir, &dir, cylinder->radius);
		t_vector	edge;
		add_vec(&edge, &cap_center, &dir);
		float	edge_distance = vec_distance(&edge, &sphere->origin);
		if (edge_distance < sphere->radius && v_cap_distance < sphere->radius)
		{
			sub_vec(&resolution, &edge, &sphere->origin);
			negate_vec(&resolution, &resolution);
			if (fabs(vec_magnitude(&resolution)) < 0.001)
			{
				printf("THIS SHHOULDNT HAPPEN3\n");
				exit(1);
			}
			normalize_vec(&resolution);
			if (cylinder_sphere == true && resolve)
			{

				scale_vec(&resolution, &resolution, sphere->radius - edge_distance - 0.001);
				add_vec(&sphere->origin, &sphere->origin, &resolution);
			}
			else if (resolve)
			{

				scale_vec(&resolution, &resolution, sphere->radius - edge_distance + 0.001);
				sub_vec(&cylinder->origin, &cylinder->origin, &resolution);
			}
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
		if (dist < (cylinder->radius + sphere->radius))
		{
			sub_vec(&resolution, &center_adjusted, &sphere->origin);
			negate_vec(&resolution, &resolution);
			if (fabs(vec_magnitude(&resolution)) < 0.001)
			{
				printf("THIS SHHOULDNT HAPPEN4\n");
				exit(1);
			}
			normalize_vec(&resolution);
			if (cylinder_sphere == true && resolve)
			{
				scale_vec(&resolution, &resolution, sphere->radius + cylinder->radius - dist - 0.001);
				add_vec(&sphere->origin, &sphere->origin, &resolution);
			}
			else if (resolve)
			{
				scale_vec(&resolution, &resolution, sphere->radius + cylinder->radius - dist + 0.001);
				sub_vec(&cylinder->origin, &cylinder->origin, &resolution);
			}
			return true;
		}
	}
	return (false);
}

void	cylinder_plane_collision_resolution(t_shape *cylinder, t_shape *plane)
{
	t_vector	cylinder_normal;
	t_vector	top_cap_center;
	t_vector	bottom_cap_center;

	mat_vec_multiply(&cylinder_normal, &cylinder->added_rots, &cylinder->orientation);
	normalize_vec(&cylinder_normal);
	scale_vec(&top_cap_center, &cylinder_normal, cylinder->height / 2);
	add_vec(&top_cap_center, &top_cap_center, &cylinder->origin);
	scale_vec(&bottom_cap_center, &cylinder_normal, -cylinder->height / 2);
	add_vec(&bottom_cap_center, &bottom_cap_center, &cylinder->origin);
	t_vector	cap_to_plane;
	sub_vec(&cap_to_plane, &top_cap_center, &plane->origin);
	double d1 = fabs(dot_product(&cap_to_plane, &plane->orientation));
	sub_vec(&cap_to_plane, &bottom_cap_center, &plane->origin);
	double d2 = fabs(dot_product(&cap_to_plane, &plane->orientation));
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
	ray.origin = cylinder->origin;
	// This should be the vector from the cylinder center to the plane
	negate_vec(&ray.direction, &plane->orientation);
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
		scale_vec(&dir, &dir, cylinder->radius + 0.01);
		t_vector	end_point;
		add_vec(&end_point, &cap_center, &dir);
		t_vector	plane_to_end_point;
		sub_vec(&plane_to_end_point, &end_point, &plane->origin);
		double	dist = fabs(dot_product(&plane_to_end_point, &plane->orientation));
		t_vector	resolution;
		// resolution = *offset;
		// negate_vec(&resolution, &plane->orientation);
		resolution = plane->orientation;
		scale_vec(&resolution, &resolution, dist + 0.001);
		add_vec(&cylinder->origin, &cylinder->origin, &resolution);
	}
	else
	{
		// This is the case when the direction of motion is perpendicular to the cylinder normal
		t_vector	center_to_point;
		sub_vec(&center_to_point, &plane->origin, &cylinder->origin);
		
		double	dist = fabs(dot_product(&center_to_point, &plane->orientation));
		t_vector	resolution;
		resolution = plane->orientation;
		// negate_vec(&resolution, &plane->orientation);
		// resolution = *offset;
		// normalize_vec(&resolution);
		scale_vec(&resolution, &resolution, cylinder->radius - dist + 0.001);
		add_vec(&cylinder->origin, &cylinder->origin, &resolution);
	}
}

bool	collide(t_scene *scene, bool resolve, int depth)
{
	t_shape			*shape1;
	t_shape			*shape2;
	unsigned int	idx1;
	unsigned int	idx2;
	bool			collided;
	idx1 = 0;
	collided = false;
	(void)collided;
	while (idx1 < scene->count.shapes)
	{
		shape1 = &scene->shapes[idx1];
		idx2 = 0;
		while (idx2 < scene->count.shapes)
		{
			if (idx1 == idx2)
			{
				idx2++;
				continue;
			}
			shape2 = &scene->shapes[idx2];
			if (shape1->type == SPHERE && shape2->type == SPHERE)
			{
				if (sphere_sphere_collision(shape1, shape2) == true)
				{
					collided = true;
					sphere_sphere_collision_resolution(shape1, shape2);
				}
			}
			else if (shape1->type == SPHERE && shape2->type == PLANE)
			{
				if (sphere_plane_collision(shape1, shape2) == true)
				{
					collided = true;
					sphere_plane_collision_resolution(shape1, shape2);
				}
			}
			else if (shape1->type == CYLINDER && shape2->type == PLANE)
			{
				if (cylinder_plane_collision(shape1, shape2) == true)
				{
					collided = true;
					cylinder_plane_collision_resolution(shape1, shape2);
				}
			}
			else if (shape1->type == CYLINDER && shape2->type == SPHERE)
			{
				if (cylinder_sphere_collision(shape1, shape2, true, resolve))
				{
					collided = true;
					printf("cylinder sphere collision\n");
				}
			}
			else if (shape1->type == SPHERE && shape2->type == CYLINDER)
			{
				if (cylinder_sphere_collision(shape2, shape1, false, resolve))
				{
					collided = true;
					printf("sphere cylinder collision\n");
				}
			}
			idx2++;
		}
		idx1++;
	}
	if (!resolve)
		return (collided);
	if (collide(scene, false, 0) == true && depth > 0)
		collide(scene, true, depth - 1);
	return (collided);
}
