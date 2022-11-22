/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:46:46 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/22 14:42:03 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include <color.h>
# include <ambient.h>
# include <camera.h>
# include <light.h>
# include <shape.h>

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