/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 23:20:15 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/26 20:09:30 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHTS_H
# define LIGHTS_H

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
	t_vector		direction;
	t_vector		init_direction;
	t_mat4			added_rots;
	t_color			color;
	float			intensity;
	float			theta;
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

#endif
