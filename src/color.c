/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 17:06:05 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/23 11:58:03 by hsarhan          ###   ########.fr       */
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
	t_color	copy;

	ft_memcpy(&copy, color, sizeof(t_color));
	copy.r *= 255;
	copy.g *= 255;
	copy.b *= 255;
	if (copy.r > 255)
		copy.r = 255;
	if (copy.r < 0)
		copy.r = 0;
	if (copy.g > 255)
		copy.g = 255;
	if (copy.g < 0)
		copy.g = 0;
	if (copy.b > 255)
		copy.b = 255;
	if (copy.b < 0)
		copy.b = 0;
	return ((int)copy.a << 24 | (int)copy.r << 16 |
			(int)copy.g << 8 | (int)copy.b);
}

/**
 * @brief Adds two colors together
 * @param res The result of the operation will be stored in res
 * @param c1 First color
 * @param c2 Second color
 */
void	add_colors(t_color *res, const t_color *c1, const t_color *c2)
{
	res->r = c1->r + c2->r;
	res->g = c1->g + c2->g;
	res->b = c1->b + c2->b;
}

/**
 * @brief Subtracts two colors from each other
 * @param res The result of the operation will be stored in res
 * @param c1 First color
 * @param c2 Second color
 */
void	sub_colors(t_color *res, const t_color *c1, const t_color *c2)
{
	res->r = c1->r - c2->r;
	res->g = c1->g - c2->g;
	res->b = c1->b - c2->b;
}

/**
 * @brief Multiplies a color by a scalar
 * @param res The result of the operation will be stored in res
 * @param color The color to be multiplied
 * @param val The value to multiply the color with
 */
void	mult_color(t_color *res, const t_color *color, float val)
{
	res->r = color->r * val;
	res->g = color->g * val;
	res->b = color->b * val;
}

/**
 * @brief Blends two colors together by multiplying their components
 * @param res The result of the operation will be stored in res
 * @param c1 First color
 * @param c2 Second color
 */
void	blend_colors(t_color *res, const t_color *c1, const t_color *c2)
{
	res->r = c1->r * c2->r;
	res->g = c1->g * c2->g;
	res->b = c1->b * c2->b;
}
