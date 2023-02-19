/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:46:47 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/20 00:51:13 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	get_color(t_worker *worker, int x, int y)
{
	int	bpp;

	bpp = worker->scene->disp->bpp;
	return (*(int *)(worker->addr + ((y * worker->width) + x) * bpp));
}

void	set_color(t_scene *scene, int x, int y, int color)
{
	int	bpp;

	if (x >= scene->settings.disp_w || y > scene->settings.disp_h || x < 0
		|| y < 0)
		return ;
	bpp = scene->disp->bpp;
	*(int *)(scene->disp->render_addr + ((y * scene->settings.disp_w) \
		+ x) * bpp) = color;
}
