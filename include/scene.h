/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:46:46 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/17 14:00:40 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

typedef struct s_scene		t_scene;
typedef struct s_ambient	t_ambient;
typedef struct s_color		t_color;
typedef struct s_camera		t_camera;
typedef struct s_light		t_light;
typedef struct s_shape		t_shape;

struct s_ambient
{
	float	intensity;
	// t_color	color;
};

struct s_scene
{
	t_ambient	ambient;
	// t_camera	camera;
	t_light		*lights;
	t_shape		*shapes;
};





#endif