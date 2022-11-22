/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 17:16:48 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/22 17:59:40 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAY_H
# define RAY_H

# include "miniRT.h"

/**
 * @brief A ray being cast from the screen
 * @param origin The ray starting point
 * @param direction The direction where the ray is being projected
 */
typedef struct s_ray	t_ray;
struct s_ray
{
	t_vector	origin;
	t_vector	direction;
};

#endif