/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 23:07:55 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/26 00:09:58 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAPE_H
# define SHAPE_H

/**
 * @brief Type of shape
 */
typedef enum e_shape_type	t_shape_type;
enum e_shape_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	CONE,
	CUBE,
	HYPERBOLOID,
	PARABOLOID,
	TRIANGLE,
};
/**
 * @brief Generic shape struct
 * @param type Type of shape. Sphere, plane, cylinder, etc.
 * @param radius Radius of the shape. Relevant for sphere, cylinder, and
 *  cone
 * @param height Height of the shape. Relevant for cylinder
 * @param origin Origin of the shape. Relevant for sphere, cylinder, and plane
 * @param orientation Orientation of the shape. Relevant for plane and cylinder
 * @param color Color of the shape
 */
typedef struct s_shape		t_shape;
struct s_shape
{
	t_shape_type	type;
	int				id;
	float			radius;
	float			radius_squared;
	float			height;
	t_vector		origin;
	t_vector		orientation;
	t_color			color;
	t_mat4			transf;
	t_mat4			inv_transf;
	t_mat4			norm_transf;
	unsigned int	mlx_color;
	float			diffuse;
	float			specular;
	float			shininess;
	float			reflectiveness;
	float			transparency;
	float			ior;
	float			rot_x;
	float			rot_y;
	float			rot_z;
	float			scale_x;
	float			scale_y;
	float			scale_z;
	bool			highlighted;
	bool			is_colliding;
	float			distance_from_origin;
	t_mat4			added_rots;
};

#endif