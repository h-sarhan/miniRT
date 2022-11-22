/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 17:06:05 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/22 18:02:25 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Creates a color that can be used by mlx
 * @param color Color struct
 * @return Color that can be used in mlx
 */
unsigned int	create_mlx_color(t_color *color)
{
	return (color->a << 24 | color->r << 16 | color->g << 8 | color->b);
}
