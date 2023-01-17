/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:19:12 by mkhan             #+#    #+#             */
/*   Updated: 2023/01/17 19:49:02 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERSECTIONS_H
# define INTERSECTIONS_H

/**
 * @brief A ray being cast from the screen
 * @param origin The ray starting point
 * @param direction The direction where the ray is being projected
 */
typedef struct s_ray			t_ray;
struct s_ray
{
	t_vector	origin;
	t_vector	dir;
};

/**
 * @brief Contains the time value of an intersection of a ray with an object,
 * along with the object that it intersected with
 * @param time The t value of the intersection
 * @param shape A pointer to the object that the ray intersected with
 */
typedef struct s_intersect		t_intersection;
struct s_intersect
{	
	float		time;
	t_shape		*shape;
	t_vector	point;
	t_vector	normal;
	t_vector	eye;
	t_vector	over_point;
	t_vector	reflect_vec;
	bool		inside;
};

/**
 * @brief A wrapper around an array of intersections
 * @param arr Array of intersections
 * @param count Number of intersections in the array
 */
typedef struct s_intersections	t_intersections;
struct s_intersections
{
	t_intersection	arr[200];
	int				count;
};

bool			intersect(t_shape *shape, const t_ray *ray,
					t_intersections *xs);

t_intersection	*hit(t_intersections *xs);

void			ray_position(t_vector *pos, const t_ray *ray, float time);

t_vector		normal_at(const t_shape *shape, const t_vector *itx_point);

void			ray_from_cam(t_ray *ray, const t_camera *cam, float x, float y);
bool	intersect_cylinder(const t_ray *ray, t_shape *shape,
	t_intersections *xs);
bool	intersect_cone(const t_ray *ray, t_shape *shape, t_intersections *xs);

#endif
