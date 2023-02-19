/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_pixels.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:34:51 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/19 18:53:38 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	fill_in_skipped_pixels_h(double x, double y, t_worker *worker,
		int threshold)
{
	int				c1;
	int				c4;
	t_intersections	arr;

	c1 = get_color(worker, x - 1, y);
	c4 = get_color(worker, x + 2, y);
	if (color_difference(c1, c4) > threshold)
	{
		if (worker->scene->settings.supersampling == true)
			super_sample_pixel(x - 1, y, &arr, worker);
		super_sample_pixel(x, y, &arr, worker);
		super_sample_pixel(x + 1, y, &arr, worker);
		if (worker->scene->settings.supersampling == true)
			super_sample_pixel(x + 2, y, &arr, worker);
	}
	else
	{
		set_color(worker, x, y, color_mix(c1, c4, 2.0 / 3.0));
		set_color(worker, x + 1, y, color_mix(c1, c4, 1.0 / 3.0));
	}
}

void	fill_in_skipped_pixels_v(double x, double y, t_worker *worker,
		int threshold)
{
	int				c1;
	int				c4;
	t_intersections	arr;

	c1 = get_color(worker, x, y - 1);
	if (y + 2 >= worker->y_end && y < worker->y_end)
		super_sample_pixel(x, y, &arr, worker);
	if (y + 2 >= worker->y_end && (y + 1) < worker->y_end)
		super_sample_pixel(x, y + 1, &arr, worker);
	if (y + 2 >= worker->y_end)
		return ;
	c4 = get_color(worker, x, y + 2);
	if (color_difference(c1, c4) > threshold)
	{
		super_sample_pixel(x, y - 1, &arr, worker);
		super_sample_pixel(x, y, &arr, worker);
		super_sample_pixel(x, y + 1, &arr, worker);
		super_sample_pixel(x, y + 2, &arr, worker);
	}
	else
	{
		set_color(worker, x, y, color_mix(c1, c4, 2.0 / 3.0));
		set_color(worker, x, y + 1, color_mix(c1, c4, 1.0 / 3.0));
	}
}

void	fill_in_horizontal(t_worker *worker, int threshold)
{
	int				x;
	int				y;

	y = worker->y_start - 1;
	while (++y < worker->y_end)
	{
		x = 1;
		while (x < worker->width)
		{
			if (x + 2 < worker->width)
				fill_in_skipped_pixels_h(x, y, worker, threshold);
			else
			{
				if (x < worker->width)
					set_color(worker, x, y, get_color(worker, x - 1, y));
				if (x + 1 < worker->width)
					set_color(worker, x + 1, y, get_color(worker, x, y));
			}
			x += 3;
		}
	}
}

void	fill_in_vertical(t_worker *worker, int threshold)
{
	int				x;
	int				y;

	y = worker->y_start + 1;
	while (y < worker->y_end)
	{
		x = 1;
		while (x < worker->width)
		{
			if (y + 2 < worker->height)
				fill_in_skipped_pixels_v(x, y, worker, threshold);
			else
			{
				if (y < worker->height)
					set_color(worker, x, y, get_color(worker, x, y - 1));
				if (y + 1 < worker->height)
					set_color(worker, x, y + 1, get_color(worker, x, y));
			}
			x++;
		}
		y += 3;
	}
}
