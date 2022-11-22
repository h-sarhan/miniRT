/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:46:46 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/22 18:58:35 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "vector.h"
# include "matrix.h"

/**
 * @brief Type of light
 */
typedef enum e_light_type	t_light_type;
enum e_light_type
{
	POINT,
	SPOT
};

/**
 * @brief Represents a color.
 * @param r Red
 * @param g Green
 * @param b Blue
 * @param a Transparency
 */
typedef struct s_color		t_color;
struct s_color
{
	int	r;
	int	g;
	int	b;
	int	a;
};

// color.c
unsigned int	create_mlx_color(t_color *color);

/**
 * @brief A light source
 * @param type Type of light
 * @param position Origin of the light
 * @param intensity Strength of the light
 * @param color Color of the light
 */
typedef struct s_light		t_light;
struct s_light
{
	t_light_type	type;
	t_vector		position;
	float			intensity;
	t_color			color;
};

/**
 * @brief An approximation of indirect lighting
 * @param intensity How strong the light is
 * @param color The color of the light
 */
typedef struct s_ambient	t_ambient;
struct s_ambient
{
	float	intensity;
	t_color	color;
};

/**
 * @brief The camera defines the viewport position
 * @param position Origin of the camera
 * @param orientation Normalized vector representing the direction
 * where the camera is pointing
 * @param fov Field of view of the camera
 */
typedef struct s_camera		t_camera;
struct s_camera
{
	t_vector	position;
	t_vector	orientation;
	int			fov;
};

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
	float			radius;
	float			height;
	t_vector		origin;
	t_vector		orientation;
	t_color			color;
	t_mat4			transf;
	t_mat4			inv_transf;
	t_mat4			norm_transf;
	unsigned int	mlx_color;
};

/**
 * @brief Holds the number of elements in a scene
 * @param ambient_count Number of ambient lights. Should be 1
 * @param camera_count Number of cameras. Should be 1
 * @param light_count Number of lights
 * @param shape_count Number of shapes
 */
typedef struct s_el_count	t_el_count;
struct	s_el_count
{
	unsigned int	ambient_count;
	unsigned int	camera_count;
	unsigned int	light_count;
	unsigned int	shape_count;
};

/**
 * @brief A description of a 3D scene
 * @param ambient Ambient lighting information
 * @param camera Camera information
 * @param lights Array of lights
 * @param shapes Array of shapes
 */
typedef struct s_scene		t_scene;
struct s_scene
{
	t_ambient	ambient;
	t_camera	camera;
	t_light		*lights;
	t_shape		*shapes;
	t_el_count	count;
	int			win_h;
	int			win_w;
};

#endif