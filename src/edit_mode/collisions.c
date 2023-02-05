/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 11:17:32 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/05 21:04:53 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mathRT.h"
#include "miniRT.h"

extern t_vector *point_to_draw_1;
extern t_vector *point_to_draw_2;

bool	sphere_sphere_collision(const t_shape *sphere1, const t_shape *sphere2)
{
	double		distance;
	t_vector	center_diff;

	sub_vec(&center_diff, &sphere1->origin, &sphere2->origin);
	distance = vec_magnitude(&center_diff);
	if (distance <= (sphere1->props.radius + sphere2->props.radius))
		return (true);
	return (false);
}

t_vector	closest_point_on_box(const t_vector *point, const t_shape *box)
{
	t_vector	point_to_center;
	t_vector	closest;

	ft_bzero(&closest, sizeof(t_vector));
	sub_vec(&point_to_center, point, &box->origin);
	closest = box->origin;
	t_vector	box_u;
	box_u.x = box->added_rots[0][0];
	box_u.y = box->added_rots[1][0];
	box_u.z = box->added_rots[2][0];
	box_u.w = 0;
	normalize_vec(&box_u);
	t_vector	box_v;
	box_v.x = box->added_rots[0][1];
	box_v.y = box->added_rots[1][1];
	box_v.z = box->added_rots[2][1];
	box_v.w = 0;
	normalize_vec(&box_v);
	t_vector	box_w;
	box_w.x = box->added_rots[0][2];
	box_w.y = box->added_rots[1][2];
	box_w.z = box->added_rots[2][2];
	box_w.w = 0;
	normalize_vec(&box_w);
	
	t_vector	step;
	double dist = dot_product(&point_to_center, &box_u);
	if (dist > box->props.scale.x)
		dist = box->props.scale.x;
	if (dist < -box->props.scale.x)
		dist = -box->props.scale.x;
	scale_vec(&step, &box_u, dist);
	add_vec(&closest, &closest, &step);
	dist = dot_product(&point_to_center, &box_v);
	if (dist > box->props.scale.y)
		dist = box->props.scale.y;
	if (dist < -box->props.scale.y)
		dist = -box->props.scale.y;
	scale_vec(&step, &box_v, dist);
	add_vec(&closest, &closest, &step);
	dist = dot_product(&point_to_center, &box_w);
	if (dist > box->props.scale.z)
		dist = box->props.scale.z;
	if (dist < -box->props.scale.z)
		dist = -box->props.scale.z;
	scale_vec(&step, &box_w, dist);
	add_vec(&closest, &closest, &step);
	return (closest);
}

bool	sphere_box_collision(t_shape *box, t_shape *sphere, bool box_sphere, bool resolve)
{
	t_vector	point_on_box  = closest_point_on_box(&sphere->origin, box);
	t_vector box_to_sphere;
	sub_vec(&box_to_sphere, &point_on_box, &sphere->origin);
	if (dot_product(&box_to_sphere, &box_to_sphere) <= sphere->props.radius_squared)
	{
		if (resolve)
		{
			t_vector	resolution;

			sub_vec(&resolution, &sphere->origin, &point_on_box);
			if (vec_magnitude(&resolution) < 0.0001)
				return (true);
			normalize_vec(&resolution);
			double	distance = vec_distance(&point_on_box, &sphere->origin);
			scale_vec(&resolution, &resolution, sphere->props.radius - distance);
			// if (vec_magnitude(&resolution) > 1)
			// 	return (true);
			if (box_sphere)
				add_vec(&sphere->origin, &sphere->origin, &resolution);
			else
				sub_vec(&box->origin, &box->origin, &resolution);
		}
		return (true);
	}
	else
	{
		return (false);
	}
}


bool	box_plane_collsion(t_shape *box, const t_shape *plane, bool resolve)
{
	t_vector	box_max;
	t_vector	box_min;
	t_vector	sides = box->props.scale;
	t_vector	box_half;
	double		extent;

	add_vec(&box_max, &box->origin, &sides);
	sub_vec(&box_min, &box->origin, &sides);
	sub_vec(&box_half, &box_max, &box_min);
	scale_vec(&box_half, &box_half, 0.5);
	t_vector	box_u;
	box_u.x = box->added_rots[0][0];
	box_u.y = box->added_rots[1][0];
	box_u.z = box->added_rots[2][0];
	box_u.w = 0;
	normalize_vec(&box_u);
	t_vector	box_v;
	box_v.x = box->added_rots[0][1];
	box_v.y = box->added_rots[1][1];
	box_v.z = box->added_rots[2][1];
	box_v.w = 0;
	normalize_vec(&box_v);
	t_vector	box_w;
	box_w.x = box->added_rots[0][2];
	box_w.y = box->added_rots[1][2];
	box_w.z = box->added_rots[2][2];
	box_w.w = 0;
	normalize_vec(&box_w);
	extent = box->props.scale.x * fabs(dot_product(&plane->orientation, &box_u)) + box->props.scale.y * fabs(dot_product(&plane->orientation, &box_v)) + box->props.scale.z * fabs(dot_product(&plane->orientation, &box_w));
	double center_to_plane = dot_product(&box->origin, &plane->orientation) - (plane->props.distance_from_origin);
	if (center_to_plane > extent || center_to_plane < -extent)
		return (false);
	if (resolve)
	{
		t_vector	resolution;
		resolution = plane->orientation;
		if (center_to_plane < 0)
		{
			scale_vec(&resolution, &resolution, extent - fabs(center_to_plane));
			sub_vec(&box->origin, &box->origin, &resolution);
		}
		else
		{
			scale_vec(&resolution, &resolution, extent - center_to_plane);
			add_vec(&box->origin, &box->origin, &resolution);
		}

	}
	return (true);
}


bool	sphere_plane_collision(t_shape *sphere, const t_shape *plane)
{
	t_vector	normal;
	normal = plane->orientation;
	normal.w = 0;
	// double d = - (dot_product(&normal, &plane->origin));
	double distance = (dot_product(&normal, &sphere->origin) - plane->props.distance_from_origin);
	if (fabs(distance) < sphere->props.radius)
		return (true);
	return (false);
}

void	sphere_sphere_collision_resolution(t_shape *sphere1, t_shape *sphere2)
{
	t_vector	dir;

	sub_vec(&dir, &sphere1->origin, &sphere2->origin);
	double dist = vec_magnitude(&dir);
	if (dist < 0.001)
		return ;
	normalize_vec(&dir);
	scale_vec(&dir, &dir,  dist - (sphere1->props.radius + sphere2->props.radius) - 0.001);
	add_vec(&sphere2->origin, &sphere2->origin, &dir);
}

void	sphere_plane_collision_resolution(t_shape *sphere, t_shape *plane)
{
	t_vector	resolution;
	t_vector	origin_to_plane;
	double		distance;

	sub_vec(&origin_to_plane, &sphere->origin, &plane->origin);
	distance = sphere->props.radius - fabs(dot_product(&origin_to_plane, &plane->orientation));
	resolution = plane->orientation;
	scale_vec(&resolution, &resolution, distance + 0.001);
	add_vec(&sphere->origin, &sphere->origin, &resolution);
}

bool	cylinder_plane_collision(t_shape *cylinder, t_shape *plane)
{
	t_vector	cylinder_normal;
	t_vector	up_vector;
	ft_bzero(&up_vector, sizeof(t_vector));
	up_vector.y = 1;
	mat_vec_multiply(&cylinder_normal, &cylinder->transf, &up_vector);
	if (vec_magnitude(&cylinder_normal) < 0.001)
	{
		return (false);
	}
	normalize_vec(&cylinder_normal);
	t_vector	cylinder_to_plane;
	sub_vec(&cylinder_to_plane, &cylinder->origin, &plane->origin);
	double	cylinder_to_plane_proj;
	cylinder_to_plane_proj = fabs(dot_product(&plane->orientation, &cylinder_to_plane));
	double	normal_dot_product;
	normal_dot_product = fabs(dot_product(&plane->orientation, &cylinder_normal));
	if (fabs(normal_dot_product - 1) < 0.001)
	{
		if (cylinder_to_plane_proj <= cylinder->props.height / 2)
		{
			return (true);
		}
		return (false);
	}
	if (cylinder_to_plane_proj < cylinder->props.radius * sqrt(1 - normal_dot_product * normal_dot_product) + (cylinder->props.height / 2) * normal_dot_product)
	{
		return (true);
	}
	return (false);
}

t_vector	closest_point_on_cylinder(t_shape *cylinder, t_vector *point)
{
	t_vector	point_to_cylinder;
	sub_vec(&point_to_cylinder, point, &cylinder->origin);
	t_vector	cylinder_normal;
	t_vector	up;
	ft_bzero(&up, sizeof(t_vector));
	up.y = 1;
	mat_vec_multiply(&cylinder_normal, &cylinder->transf, &up);
	normalize_vec(&cylinder_normal);
	
	double d = dot_product(&cylinder_normal, &point_to_cylinder);
	if (d > 0 && d > cylinder->props.height / 2 + cylinder->props.radius)
		d = cylinder->props.height / 2 + cylinder->props.radius;
	if (d < 0 && d < - cylinder->props.height / 2 - cylinder->props.radius)
		d = -cylinder->props.height / 2 - cylinder->props.radius;
	scale_vec(&cylinder_normal, &cylinder_normal, d);
	t_vector closest_point;
	add_vec(&closest_point, &cylinder->origin, &cylinder_normal);
	normalize_vec(&cylinder_normal);
	return closest_point;
}

bool	box_cylinder_collision(t_shape *box, t_shape *cylinder, bool box_cylinder, bool resolve)
{
	(void)box;
	(void)cylinder;
	(void)resolve;
	(void)box_cylinder;
	t_vector	dir;
	ft_bzero(&dir, sizeof(t_vector));
	dir.y = 1;
	if (gjk(box, cylinder) == true)
	{
		ft_bzero(&box->props.color, sizeof(t_color));
		box->props.color.g = 1;
		ft_bzero(&cylinder->props.color, sizeof(t_color));
		cylinder->props.color.g = 1;
		return (true);
	}
	ft_bzero(&box->props.color, sizeof(t_color));
	box->props.color.r = 1;
	ft_bzero(&cylinder->props.color, sizeof(t_color));
	cylinder->props.color.r = 1;
	return (false);
}

bool	cylinder_cylinder_collision(t_shape *cyl1, t_shape *cyl2, bool resolve)
{
	(void)cyl1;
	(void)cyl2;
	(void)resolve;
	t_vector	dir;
	ft_bzero(&dir, sizeof(t_vector));
	dir.y = 1;
	if (gjk(cyl1, cyl2) == true)
	{
		ft_bzero(&cyl1->props.color, sizeof(t_color));
		cyl1->props.color.g = 1;
		return (true);
	}
	ft_bzero(&cyl1->props.color, sizeof(t_color));
	cyl1->props.color.r = 1;
	return (false);
}

bool	cone_cone_collision(t_shape *co1, t_shape *co2, bool resolve)
{
	(void)co1;
	(void)co2;
	(void)resolve;
	t_vector	dir;
	ft_bzero(&dir, sizeof(t_vector));
	dir.y = 1;
	if (gjk(co1, co2) == true)
	{
		ft_bzero(&co1->props.color, sizeof(t_color));
		co1->props.color.g = 1;
		return (true);
	}
	ft_bzero(&co1->props.color, sizeof(t_color));
	co1->props.color.r = 1;
	return (false);
}

bool	cylinder_sphere_collision(t_shape *cylinder, t_shape *sphere, bool cylinder_sphere, bool resolve)
{
	t_vector	resolution;
	t_vector	cylinder_to_sphere;
	t_vector	cylinder_normal;
	t_vector	up_vector;
	ft_bzero(&up_vector, sizeof(t_vector));
	up_vector.y = 1;
	mat_vec_multiply(&cylinder_normal, &cylinder->transf, &up_vector);
	if (fabs(vec_magnitude(&cylinder_normal)) < 0.001)
	{
		return (false);
	}
	normalize_vec(&cylinder_normal);
	sub_vec(&cylinder_to_sphere, &cylinder->origin, &sphere->origin);
	double		v_dist = dot_product(&cylinder_to_sphere, &cylinder_normal);
	t_vector	top_cap_center;
	t_vector	bottom_cap_center;

	scale_vec(&top_cap_center, &cylinder_normal, cylinder->props.height / 2);
	add_vec(&top_cap_center, &top_cap_center, &cylinder->origin);
	scale_vec(&bottom_cap_center, &cylinder_normal, -cylinder->props.height / 2);
	add_vec(&bottom_cap_center, &bottom_cap_center, &cylinder->origin);

	if (v_dist < - cylinder->props.height / 2 || v_dist > cylinder->props.height / 2)
	{
		t_vector	cap_center;
		if (v_dist < - cylinder->props.height / 2)
		{
			cap_center = top_cap_center;
		}
		else if (v_dist > cylinder->props.height / 2)
		{
			cap_center = bottom_cap_center;
		}
		t_vector	cap_to_sphere;

		sub_vec(&cap_to_sphere, &cap_center, &sphere->origin);
		double	dist = vec_magnitude(&cap_to_sphere);
		double	v_cap_distance = fabs(fabs(v_dist) - (cylinder->props.height / 2));
		if ((dist * dist - v_cap_distance * v_cap_distance) < 0)
			return (false);
		double	h_cap_distance = sqrt(dist * dist - v_cap_distance * v_cap_distance);
		if (h_cap_distance < cylinder->props.radius && v_cap_distance < sphere->props.radius)
		{
			resolution = cylinder_normal;
			if (v_dist > cylinder->props.height / 2)
				negate_vec(&resolution, &resolution);
			if (cylinder_sphere == true && resolve)
			{
				scale_vec(&resolution, &resolution, sphere->props.radius - v_cap_distance - 0.001);
				add_vec(&sphere->origin, &sphere->origin, &resolution);
			}
			else if (resolve)
			{
				scale_vec(&resolution, &resolution, sphere->props.radius - v_cap_distance + 0.001);
				sub_vec(&cylinder->origin, &cylinder->origin, &resolution);
			}
			return true;
		}
		t_vector	dir;
		scale_vec(&dir, &cylinder_normal, v_cap_distance);
		if (v_dist < - cylinder->props.height / 2)
		{
			negate_vec(&dir, &dir);
		}
		add_vec(&dir, &cap_center, &dir);
		sub_vec(&dir, &sphere->origin, &dir);
		if (fabs(vec_magnitude(&dir)) < 0.001)
		{
			// printf("THIS SHHOULDNT HAPPEN2\\n");
			// exit(1);
		return (false);
			
		}
		normalize_vec(&dir);
		scale_vec(&dir, &dir, cylinder->props.radius);
		t_vector	edge;
		add_vec(&edge, &cap_center, &dir);
		double	edge_distance = vec_distance(&edge, &sphere->origin);
		if (edge_distance < sphere->props.radius && v_cap_distance < sphere->props.radius)
		{
			sub_vec(&resolution, &edge, &sphere->origin);
			negate_vec(&resolution, &resolution);
			if (fabs(vec_magnitude(&resolution)) < 0.001)
			{
				// printf("THIS SHHOULDNT HAPPEN3\n");
				// exit(1);
		return (false);
				
			}
			normalize_vec(&resolution);
			if (cylinder_sphere == true && resolve)
			{

				scale_vec(&resolution, &resolution, sphere->props.radius - edge_distance - 0.001);
				add_vec(&sphere->origin, &sphere->origin, &resolution);
			}
			else if (resolve)
			{

				scale_vec(&resolution, &resolution, sphere->props.radius - edge_distance + 0.001);
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
		double	dist = vec_distance(&center_adjusted, &sphere->origin);
		if (dist < (cylinder->props.radius + sphere->props.radius))
		{
			sub_vec(&resolution, &center_adjusted, &sphere->origin);
			negate_vec(&resolution, &resolution);
			if (vec_magnitude(&resolution) < 0.001)
			{
				// printf("THIS SHHOULDNT HAPPEN4\n");

				// exit(1);
				return (false);
			}
			normalize_vec(&resolution);
			if (cylinder_sphere == true && resolve)
			{
				scale_vec(&resolution, &resolution, sphere->props.radius + cylinder->props.radius - dist - 0.001);
				add_vec(&sphere->origin, &sphere->origin, &resolution);
			}
			else if (resolve)
			{
				scale_vec(&resolution, &resolution, sphere->props.radius + cylinder->props.radius - dist + 0.001);
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
	t_vector	up_vector;
	
	ft_bzero(&up_vector, sizeof(t_vector));
	up_vector.y = 1;
	mat_vec_multiply(&cylinder_normal, &cylinder->transf, &up_vector);
	if (vec_magnitude(&cylinder_normal) < 0.001)
	{
		return ;
	}
	normalize_vec(&cylinder_normal);
	scale_vec(&top_cap_center, &cylinder_normal, cylinder->props.height / 2);
	add_vec(&top_cap_center, &top_cap_center, &cylinder->origin);
	scale_vec(&bottom_cap_center, &cylinder_normal, -cylinder->props.height / 2);
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
	t_vector	resolution;
	double	normal_dot_product;
	normal_dot_product = fabs(dot_product(&plane->orientation, &cylinder_normal));
	if (fabs(normal_dot_product - 1) < 0.00001)
	{
		// printf();
		resolution = plane->orientation;
		// t_
		scale_vec(&resolution, &resolution, min(d1, d2) + 0.001);
		add_vec(&cylinder->origin, &cylinder->origin, &resolution);
		return ;
	}
	// Form a secondary plane at the cylinder cap
	// This plane will have the equation
	// Ax + By + Cz + D = 0
	// A, B, C are the xyz values of the normal to the plane.
	// D can be found by plugging a point in. The point will just be the cylinder cap
	double		d = -(dot_product(&cylinder_normal, &cap_center));
	// Normal of the secondary plane is the normal of the cylinder
	// We intersect a ray starting from the cylinder center in the direction of motion
	t_ray ray;
	ray.origin = cylinder->origin;
	// This should be the vector from the cylinder center to the plane
	negate_vec(&ray.dir, &plane->orientation);
	normalize_vec(&ray.dir);
	if (fabs(dot_product(&cylinder_normal, &ray.dir)) > 0.001)
	{

		double	t = -(dot_product(&cylinder_normal, &ray.origin) + d) / dot_product(&cylinder_normal, &ray.dir);
		t_vector	point_on_secondary_plane;
		ray_position(&point_on_secondary_plane, &ray, t);
		t_vector	dir;
		sub_vec(&dir, &point_on_secondary_plane, &cap_center);
		if (vec_magnitude(&dir) < 0.001)
			return ;
		normalize_vec(&dir);
		scale_vec(&dir, &dir, cylinder->props.radius + 0.01);
		t_vector	end_point;
		add_vec(&end_point, &cap_center, &dir);
		t_vector	plane_to_end_point;
		sub_vec(&plane_to_end_point, &end_point, &plane->origin);
		double	dist = fabs(dot_product(&plane_to_end_point, &plane->orientation));
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
		scale_vec(&resolution, &resolution, cylinder->props.radius - dist + 0.001);
		add_vec(&cylinder->origin, &cylinder->origin, &resolution);
	}
}


bool	box_box_collision(t_shape *box_1, t_shape *box_2, bool resolve)
{
	(void)resolve;
	t_vector	resolution;
	if (resolve == true)
	{
		if (test_box_axes(box_1, box_2, &resolution) == true)
		{
			// ft_bzero(&box_1->props.color, sizeof(t_color));
			// box_1->props.color.g = 1;
			// print_vector(&resolution);
			add_vec(&box_2->origin, &box_2->origin, &resolution);
			return (true);
		}
		// ft_bzero(&box_1->props.color, sizeof(t_color));
		// box_1->props.color.r = 1;
		return (false);
	}
	else if (test_box_axes(box_1, box_2, NULL) == true)
	{
		// ft_bzero(&box_1->props.color, sizeof(t_color));
		// box_1->props.color.g = 1;
		return (true);
	}
	// ft_bzero(&box_1->props.color, sizeof(t_color));
	// box_1->props.color.r = 1;
	return (false);
}
t_vector	cone_furthest_point(const t_vector *dir, const t_shape *cone);

bool	collide(t_scene *scene, bool resolve, int depth, t_shape *transformed_shape)
{
	t_shape			*shape1;
	t_shape			*shape2;
	int				idx1;
	int				idx2;
	bool			collided;
	idx1 = 0;
	collided = false;
	(void)collided;
	calculate_transforms(scene);
	
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
					if (resolve == true)
					{
						if (shape2 == transformed_shape)
							sphere_sphere_collision_resolution(shape2, shape1);
						else
							sphere_sphere_collision_resolution(shape1, shape2);
					}
				}
			}
			else if (shape1->type == SPHERE && shape2->type == PLANE)
			{
				if (sphere_plane_collision(shape1, shape2) == true)
				{
					collided = true;
					if (resolve == true)
						sphere_plane_collision_resolution(shape1, shape2);
				}
			}
			else if (shape1->type == CYLINDER && shape2->type == PLANE)
			{
				if (cylinder_plane_collision(shape1, shape2) == true)
				{
					collided = true;
					if (resolve == true)
						cylinder_plane_collision_resolution(shape1, shape2);
				}
			}
			else if (shape1->type == CUBE && shape2->type == PLANE)
			{
				if (box_plane_collsion(shape1, shape2, resolve) == true)
				{
					collided = true;
				}
			}
			else if (shape1->type == CYLINDER && shape2->type == SPHERE && transformed_shape != shape2)
			{
				if (cylinder_sphere_collision(shape1, shape2, true, resolve))
				{
					collided = true;
					// printf("cylinder sphere collision\n");
				}
			}
			else if (shape1->type == SPHERE && shape2->type == CYLINDER && transformed_shape != shape2)
			{
				if (cylinder_sphere_collision(shape2, shape1, false, resolve))
				{
					collided = true;
					// printf("sphere cylinder collision\n");
				}
			}
			else if (shape1->type == SPHERE && shape2->type == CUBE && transformed_shape != shape2)
			{
				if (sphere_box_collision(shape2, shape1, false, resolve) == true)
				{
					collided = true;
					// printf("sphere cylinder collision\n");
				}
			}
			else if (shape1->type == CUBE && shape2->type == SPHERE && transformed_shape != shape2)
			{
				if (sphere_box_collision(shape1, shape2, true, resolve) == true)
				{
					collided = true;
					// printf("sphere cylinder collision\n");
				}
			}
			else if (shape1->type == CUBE && shape2->type == CYLINDER && transformed_shape != shape2)
			{
				if (box_cylinder_collision(shape1, shape2, false, resolve) == true)
				{
					collided = true;
					// printf("sphere cylinder collision\n");
				}
			}
			else if (shape1->type == CYLINDER && shape2->type == CUBE && transformed_shape != shape2)
			{
				if (box_cylinder_collision(shape2, shape1, true, resolve) == true)
				{
					collided = true;
					// printf("sphere cylinder collision\n");
				}
			}
			else if (shape1->type == CYLINDER && shape2->type == CYLINDER)
			{
				if (cylinder_cylinder_collision(shape1, shape2, false) == true)
				{
					collided = true;
					// if (resolve == true)
					// {
					// 	if (shape2 == transformed_shape)
					// 		cylinder_cylinder_collision(shape2, shape1, true);
					// 	else
					// 		cylinder_cylinder_collision(shape1, shape2, true);
					// }
				}
			}
			else if (shape1->type == CONE && shape2->type == CONE)
			{
				if (cone_cone_collision(shape1, shape2, false) == true)
				{
					collided = true;
				}
			}
			else if (shape1->type == CUBE && shape2->type == CUBE)
			{
				if (box_box_collision(shape1, shape2, false) == true)
				{
					collided = true;
					if (resolve == true)
					{
						if (shape2 == transformed_shape)
							box_box_collision(shape2, shape1, true);
						else
							box_box_collision(shape1, shape2, true);
					}
				}
			}
			else if (shape1->type == CONE)
			{
				t_vector	dir;
				
				ft_bzero(&dir, sizeof(t_vector));
				dir.x = 1;
				// dir.y = 1;
				normalize_vec(&dir);
				t_vector furthest_point = cone_furthest_point(&dir, shape1);
				*point_to_draw_1 = furthest_point;
				
			}
			idx2++;
		}
		idx1++;
	}
	if (!resolve)
		return (collided);
	if (collide(scene, false, 0, NULL) == true && depth > 0)
		collide(scene, true, depth - 1, NULL);
	return (collided);
}
