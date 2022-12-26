/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 17:06:05 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/26 12:57:17 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	get_t(int trgb)
{
	return ((trgb >> 24) & 0xFF);
}

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

int	color_avg(int c1, int c2)
{
	return (create_trgb(0, (get_r(c1) + get_r(c2)) / 2,
			(get_g(c1) + get_g(c2)) / 2, (get_b(c1) + get_b(c2)) / 2));
}

int	color_mix(int c1, int c2, float mix)
{
	int	result;
	int	t;
	int	r;
	int	g;
	int	b;

	t = get_t(c1) * mix + get_t(c2) * (1 - mix);
	r = get_r(c1) * mix + get_r(c2) * (1 - mix);
	g = get_g(c1) * mix + get_g(c2) * (1 - mix);
	b = get_b(c1) * mix + get_b(c2) * (1 - mix);
	result = create_trgb(t, r, g, b);
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

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
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

int	get_color(t_worker *worker, int x, int y)
{
	int	bpp;

	bpp = worker->scene->disp->bytes_per_pixel;
	return (*(int *)(worker->addr + ((y * worker->width) + x) * bpp));
}

void	set_color(t_worker *worker, int x, int y, int color)
{
	int	bpp;

	if (x >= worker->width || y > worker->height || x < 0 || y < 0)
		return ;
	bpp = worker->scene->disp->bytes_per_pixel;
	*(int *)(worker->addr + ((y * worker->width) + x) * bpp) = color;
}
