/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 11:26:56 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/26 02:15:51 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	itx_cmp(const void *s1, const void *s2)
{
	t_intersect	*no1;
	t_intersect	*no2;

	no1 = (t_intersect *) s1;
	no2 = (t_intersect *) s2;
	if (no1->time < no2->time)
		return (-1);
	else if (no1->time > no2->time)
		return (1);
	return (0);
}

bool	is_sorted(t_intersections *arr)
{
	int	n;

	n = arr->count;
	while (--n >= 1)
	{
		if (arr->arr[n].time < arr->arr[n - 1].time)
			return (false);
	}
	return (true);
}

void	sort_intersections(t_intersections *arr)
{
	qsort(arr, arr->count, sizeof(t_intersect), &itx_cmp);
}

void	swap_intersections(t_intersect *itx1, t_intersect *itx2)
{
	t_intersect	tmp;

	tmp = *itx1;
	*itx1 = *itx2;
	*itx2 = tmp;
}

t_color	calculate_lighting(t_intersections *arr, t_worker *worker, t_ray *ray)
{
	t_intersect		*itx;
	unsigned int	light_idx;
	t_color			final_color;
	t_color			light_color;
	float			reflectance;
	t_color			reflected;
	t_color			refracted;

	if (worker->scene->settings.refraction_depth != 0)
	{
		sort_intersections(arr);
		itx = hit_sorted(arr);
	}
	else
		itx = hit(arr);
	ft_bzero(&final_color, sizeof(t_color));
	if (itx != NULL)
	{
		prepare_computations(worker->scene, itx, ray, arr);
		light_idx = 0;
		while (light_idx < worker->scene->count.lights)
		{
			light_color = lighting(itx, worker->scene, light_idx);
			reflected = reflected_color(worker->scene, itx, worker->scene->settings.reflection_depth, light_idx);
			refracted = refracted_color(worker->scene, itx, worker->scene->settings.refraction_depth, light_idx);
			if (itx->shape->props.reflectiveness > 0 && itx->shape->props.transparency > 0)
			{
				reflectance = schlick(itx);
				mult_color(&reflected, &reflected, reflectance);
				mult_color(&refracted, &refracted, (1 - reflectance));
			}
			add_colors(&final_color, &final_color, &light_color);
			add_colors(&final_color, &final_color, &reflected);
			add_colors(&final_color, &final_color, &refracted);
			light_idx++;
		}
	}
	return (final_color);
}

void	*render_scene(t_worker *worker)
{
	t_intersections	arr;
	int				x;
	int				y;
	unsigned int	shape_idx;
	t_ray			ray;
	t_color			color;
	int				line_counter;

	line_counter = 0;
	y = worker->y_start - 1;
	while (++y < worker->y_end)
	{
		x = -1;
		while (++x < worker->width)
		{
			set_color(worker, x, y, 0);
			ray_for_pixel(&ray, &worker->scene->camera, x, y);
			shape_idx = -1;
			arr.count = 0;
			while (++shape_idx < worker->scene->count.shapes)
			{
				intersect(&worker->scene->shapes[shape_idx], &ray, &arr);
			}
			color = calculate_lighting(&arr, worker, &ray);
			set_color(worker, x, y, create_mlx_color(&color));
		}
		line_counter++;
		if (worker->scene->settings.edit_mode == false
			&& (line_counter == (worker->y_end - worker->y_start) / 5))
		{
			sem_post(worker->scene->sem_loading);
			line_counter = 0;
		}
	}
	return (NULL);
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

void	fill_in_horizontal2(t_worker *worker, int threshold,
			bool show_recalculated)
{
	int				x;
	int				y;
	int				avg_color;
	t_intersections	arr;
	unsigned int	shape_idx;
	t_ray			ray;
	t_color			color;
	int				c1;
	int				c2;
	int				c3;
	int				c4;

	y = worker->y_start - 1;
	while (++y < worker->y_end)
	{
		x = 1;
		while (x < worker->width)
		{
			if (x + 2 < worker->width)
			{
				c1 = get_color(worker, x - 1, y);
				c4 = get_color(worker, x + 2, y);
				if (color_difference(c1, c4) > threshold)
				{
					ray_for_pixel(&ray, &worker->scene->camera, x, y);
					shape_idx = -1;
					arr.count = 0;
					while (++shape_idx < worker->scene->count.shapes)
						intersect(&worker->scene->shapes[shape_idx], &ray,
							&arr);
					color = calculate_lighting(&arr, worker, &ray);
					c2 = create_mlx_color(&color);
					set_color(worker, x, y, c2);
					if (show_recalculated == true)
						set_color(worker, x, y, 0xff0000);
					if (color_difference(c2, c4) > threshold)
					{
						ray_for_pixel(&ray, &worker->scene->camera, x + 1, y);
						shape_idx = -1;
						arr.count = 0;
						while (++shape_idx < worker->scene->count.shapes)
							intersect(&worker->scene->shapes[shape_idx], &ray,
								&arr);
						color = calculate_lighting(&arr, worker, &ray);
						set_color(worker, x + 1, y, create_mlx_color(&color));
						if (show_recalculated == true)
							set_color(worker, x + 1, y, 0xff0000);
					}
					else
					{
						avg_color = color_avg(c2, c4);
						set_color(worker, x + 1, y, avg_color);
					}
				}
				else
				{
					// avg_color = color_avg(c1, c2);
					c2 = color_mix(c1, c4, 2.0 / 3.0);
					c3 = color_mix(c1, c4, 1.0 / 3.0);
					set_color(worker, x, y, c2);
					set_color(worker, x + 1, y, c3);
				}
			}
			else
			{
				// Remove this if it looks bad on the edges
				if (x < worker->width)
					set_color(worker, x, y, get_color(worker, x - 1, y));
				if (x + 1 < worker->width)
					set_color(worker, x + 1, y, get_color(worker, x, y));
			}
			x += 3;
		}
	}
}

void	fill_in_vertical2(t_worker *worker, int threshold,
			bool show_recalculated)
{
	int				x;
	int				y;
	int				avg_color;
	t_intersections	arr;
	unsigned int	shape_idx;
	t_ray			ray;
	t_color			color;
	int				c1;
	int				c2;
	int				c3;
	int				c4;

	y = worker->y_start + 1;
	while (y < worker->y_end)
	{
		x = 0;
		while (x < worker->width)
		{
			if (y + 2 < worker->height)
			{
				c1 = get_color(worker, x, y - 1);
				c4 = get_color(worker, x, y + 2);
				if (color_difference(c1, c4) > threshold)
				{
					ray_for_pixel(&ray, &worker->scene->camera, x, y);
					shape_idx = -1;
					arr.count = 0;
					while (++shape_idx < worker->scene->count.shapes)
						intersect(&worker->scene->shapes[shape_idx], &ray,
							&arr);
					color = calculate_lighting(&arr, worker, &ray);
					c2 = create_mlx_color(&color);
					set_color(worker, x, y, c2);
					if (show_recalculated == true)
						set_color(worker, x, y, 0xff0000);
					if (color_difference(c2, c4) > threshold)
					{
						ray_for_pixel(&ray, &worker->scene->camera, x, y + 1);
						shape_idx = -1;
						arr.count = 0;
						while (++shape_idx < worker->scene->count.shapes)
							intersect(&worker->scene->shapes[shape_idx], &ray,
								&arr);
						color = calculate_lighting(&arr, worker, &ray);
						set_color(worker, x, y + 1, create_mlx_color(&color));
						if (show_recalculated == true)
							set_color(worker, x, y + 1, 0xff0000);
					}
					else
					{
						avg_color = color_avg(c2, c4);
						set_color(worker, x, y + 1, avg_color);
					}
				}
				else
				{
					// avg_color = color_avg(c1, c2);
					c2 = color_mix(c1, c4, 2.0 / 3.0);
					c3 = color_mix(c1, c4, 1.0 / 3.0);
					set_color(worker, x, y, c2);
					set_color(worker, x, y + 1, c3);
				}
			}
			else
			{
				// Improve this
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

void	*render_scene_faster(t_worker *worker)
{
	t_intersections	arr;
	int				x;
	int				y;
	unsigned int	shape_idx;
	t_ray			ray;
	t_color			color;
	int				line_counter;
	bool			show_recalculated;
	int				thresh;

	line_counter = 0;
	y = worker->y_start;
	while (y < worker->y_end)
	{
		x = 0;
		while (x < worker->width)
		{
			set_color(worker, x, y, 0);
			set_color(worker, x + 1, y, 0);
			set_color(worker, x + 2, y, 0);
			ray_for_pixel(&ray, &worker->scene->camera, x, y);
			shape_idx = -1;
			arr.count = 0;
			while (++shape_idx < worker->scene->count.shapes)
				intersect(&worker->scene->shapes[shape_idx], &ray, &arr);
			color = calculate_lighting(&arr, worker, &ray);
			set_color(worker, x, y, create_mlx_color(&color));
			x += 3;
		}
		line_counter++;
		if (worker->scene->settings.edit_mode == false
			&& (line_counter == (worker->y_end - worker->y_start) / 15))
		{
			sem_post(worker->scene->sem_loading);
			line_counter = 0;
		}
		set_color(worker, x, y + 1, 0);
		set_color(worker, x, y + 2, 0);
		y += 3;
	}
	show_recalculated = false;
	thresh = 10;
	fill_in_horizontal2(worker, thresh, show_recalculated);
	fill_in_vertical2(worker, thresh, show_recalculated);
	return (NULL);
}
