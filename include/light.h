/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:43:04 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/17 15:06:26 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_H
# define LIGHT_H

# include <vector.h>
# include <color.h>

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
	float			intensity;
	t_color			color;
};

#endif