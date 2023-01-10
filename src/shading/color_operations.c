/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:46:47 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/10 14:45:16 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
 * @brief Multiplies a color by a scalar
 * @param res The result of the operation will be stored in res
 * @param color The color to be multiplied
 * @param val The value to multiply the color with
 */
void	mult_color(t_color *res, const t_color *color, double val)
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

	bpp = worker->scene->disp->bpp;
	return (*(int *)(worker->addr + ((y * worker->width) + x) * bpp));
}

void	set_color(t_worker *worker, int x, int y, int color)
{
	int	bpp;

	if (x >= worker->width || y > worker->height || x < 0 || y < 0)
		return ;
	bpp = worker->scene->disp->bpp;
	*(int *)(worker->addr + ((y * worker->width) + x) * bpp) = color;
}

// We never use this function so might as well delete it
// /**
//  * @brief Subtracts two colors from each other
//  * @param res The result of the operation will be stored in res
//  * @param c1 First color
//  * @param c2 Second color
//  */
// void	sub_colors(t_color *res, const t_color *c1, const t_color *c2)
// {
// 	res->r = c1->r - c2->r;
// 	res->g = c1->g - c2->g;
// 	res->b = c1->b - c2->b;
// }
