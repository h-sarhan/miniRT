/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 23:07:55 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/18 17:57:41 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAPE_H
# define SHAPE_H

/**
 * @brief Type of shape
 */
typedef enum e_shape_type		t_shape_type;
enum e_shape_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	CONE,
	CUBE,
	TRIANGLE,
};

/**
 * @brief Type of pattern
 */
typedef enum e_pattern_type		t_pattern_type;
enum e_pattern_type
{
	NONE,
	STRIPE,
	CHECKER_BOARD,
	GRADIENT,
	RING,
};

typedef struct s_shape_props	t_props;

struct s_shape_props
{
	bool			highlighted;
	float			radius;
	float			radius_squared;
	float			height;
	float			diffuse;
	float			specular;
	float			shininess;
	float			reflectiveness;
	float			distance_from_origin;
	t_color			color;
	t_vector		rot;
	t_vector		scale;
	t_pattern_type	pattern_type;
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
typedef struct s_shape			t_shape;
struct s_shape
{
	t_shape_type	type;
	int				id;
	t_props			props;
	t_vector		origin;
	t_vector		orientation;
	t_mat4			transf;
	t_mat4			inv_transf;
	t_mat4			norm_transf;
	t_mat4			added_rots;
	t_color			**diffuse_tex;
	t_color			**normal_tex;
	int				tex_width;
	int				tex_height;
	unsigned int	mlx_color;
};

#endif
