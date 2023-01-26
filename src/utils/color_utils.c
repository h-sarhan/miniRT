/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:29:14 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/26 20:09:30 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	color_mix(int c1, int c2, float mix)
{
	int				r;
	int				g;
	int				b;
	unsigned char	*col1;
	unsigned char	*col2;

	col1 = (unsigned char *)&c1;
	col2 = (unsigned char *)&c2;
	r = col1[2] * mix + col2[2] * (1 - mix);
	g = col1[1] * mix + col2[1] * (1 - mix);
	b = col1[0] * mix + col2[0] * (1 - mix);
	return (r << 16 | g << 8 | b);
}

int	color_difference(int c1, int c2)
{
	int				r_diff;
	int				g_diff;
	int				b_diff;
	unsigned char	*col1;
	unsigned char	*col2;

	col1 = (unsigned char *)&c1;
	col2 = (unsigned char *)&c2;
	r_diff = abs(col1[2] - col2[2]);
	g_diff = abs(col1[1] - col2[1]);
	b_diff = abs(col1[0] - col2[0]);
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

t_color	int_to_color(int hex_color)
{
	t_color	color;

	color.a = 0;
	color.r = ((unsigned char *)&hex_color)[2] / 255.0f;
	color.g = ((unsigned char *)&hex_color)[1] / 255.0f;
	color.b = ((unsigned char *)&hex_color)[0] / 255.0f;
	return (color);
}
