/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:49:01 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/21 22:05:02 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAPE_H
# define SHAPE_H

# include <vector.h>
# include <color.h>
# include <matrix.h>

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
 * @param radius Radius of the shape. Only relevant for sphere, cylinder, and
 *  cone
 * @param width Width of the shape. Relevant for ???
 * @param height Height of the shape. Relevant for ???
 * @param origin Origin of the shape. Relevant for ???
 * @param orientation Orientation of the shape. Relevant for plane and ???
 * @param color Color of the shape
 */
typedef struct s_shape		t_shape;
struct s_shape
{
	t_shape_type	type;
	float			radius;
	float			width;
	float			height;
	t_vector		origin;
	t_vector		orientation;
	t_color			color;
	t_mat4			trans;
	t_mat4			inv_trans;
};

#endif
