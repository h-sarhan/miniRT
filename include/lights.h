/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 23:20:15 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/10 14:45:16 by mkhan            ###   ########.fr       */
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
	double			intensity;
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
	double	intensity;
	t_color	color;
};

#endif
