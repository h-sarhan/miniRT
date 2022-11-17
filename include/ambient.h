/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambient.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:06:53 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/17 15:05:25 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMBIENT_H
# define AMBIENT_H

# include <color.h>

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