/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:46:47 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/19 20:59:22 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
