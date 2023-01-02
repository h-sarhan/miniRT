/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:29:14 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/02 18:05:01 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	get_r(int col)
{
	return ((col >> 16) & 0xFF);
}

int	get_g(int col)
{
	return ((col >> 8) & 0xFF);
}

int	get_b(int col)
{
	return (col & 0xFF);
}

int	color_mix(int c1, int c2, float mix)
{
	int	result;
	int	r;
	int	g;
	int	b;

	r = get_r(c1) * mix + get_r(c2) * (1 - mix);
	g = get_g(c1) * mix + get_g(c2) * (1 - mix);
	b = get_b(c1) * mix + get_b(c2) * (1 - mix);
	result = r << 16 | g << 8 | b;
	return (result);
}

int	color_difference(int c1, int c2)
{
	int	r_diff;
	int	g_diff;
	int	b_diff;

	r_diff = abs(get_r(c1) - get_r(c2));
	g_diff = abs(get_g(c1) - get_g(c2));
	b_diff = abs(get_b(c1) - get_b(c2));
	return (r_diff + g_diff + b_diff);
}

static unsigned int	clamp_color(float color)
{
	color *= 256;
	if (color > 255)
		return (255);
	if (color < 0)
		return (0);
	return (color);
}

/**
 * @brief Creates a color that can be used by mlx
 * @param color Color struct
 * @return Color that can be used in mlx
 */
unsigned int	create_mlx_color(t_color *color)
{
	return (clamp_color(color->a) << 24 | clamp_color(color->r) << 16
		| clamp_color(color->g) << 8 | clamp_color(color->b));
}
