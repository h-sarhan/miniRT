/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 11:26:56 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/20 14:53:45 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int t_cmp( const void *s1, const void *s2)
{
   t_intersect *no1 = (t_intersect *) s1;
   t_intersect *no2 = (t_intersect *) s2;
    
   if ( no1->time < no2->time )
      return -1;
   else if ( no1->time > no2->time )
      return 1;
   else
      return 0;
}

bool is_sorted(t_intersections *arr)
{
	int n = arr->count;
	while ( --n >= 1) {
		if ( arr->arr[n].time < arr->arr[n-1].time ) return false;
	}
	return true;
}
 
#include <string.h>

void sort_intersections(t_intersections *arr)
{
	(void)arr;
	qsort(arr, arr->count, sizeof(t_intersect), &t_cmp);
}
void	swap_intersections(t_intersect *itx1, t_intersect *itx2)
{
	t_intersect tmp;

	tmp = *itx1;
	*itx1 = *itx2;
	*itx2 = tmp;
}
 

void	calculate_lighting(t_intersections *arr, t_worker *worker, t_ray *ray,
	int pixel)
{
	t_intersect		*itx;
	unsigned int	light_idx;
	t_color			final_color;
	t_color			light_color;
	double			reflectance;

	sort_intersections(arr);
	itx = hit(arr);
	if (itx != NULL)
	{
		prepare_computations(itx, ray, arr);
		ft_bzero(&final_color, sizeof(t_color));
		light_idx = 0;
		while (light_idx < worker->scene->count.light_count)
		{
			light_color = lighting(itx, worker->scene, light_idx);
			t_color	reflected  = reflected_color(worker->scene, itx, 4, light_idx);
			t_color	refracted  = refracted_color(worker->scene, itx, 4, light_idx);
			if (itx->shape->reflectiveness > 0 && itx->shape->transparency > 0)
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
		*(int *)(worker->addr + pixel) = create_mlx_color(&final_color);
	}
}

void	*render_scene(t_worker *worker)
{
	t_intersections	arr;
	int				x;
	int				y;
	unsigned int	shape_idx;
	t_ray			ray;

	int	line_counter = 0;
	y = worker->y_start - 1;
	while (++y < worker->y_end)
	{
		x = -1;
		while (++x < worker->width)
		{
			*(unsigned int *)(worker->addr + \
				(y * worker->width + x) * \
				worker->scene->mlx->bytes_per_pixel) = 0;
			ray_for_pixel(&ray, &worker->scene->camera, x, y);
			shape_idx = -1;
			arr.count = 0;
			while (++shape_idx < worker->scene->count.shape_count)
			{
				intersect(&worker->scene->shapes[shape_idx], &ray, &arr);
			}
			calculate_lighting(&arr, worker, &ray, (y * worker->width \
				+ x) * worker->scene->mlx->bytes_per_pixel);
		}
		line_counter++;
		if (worker->scene->edit_mode == false && (line_counter == (worker->y_end - worker->y_start) / 5))
		{
			sem_post(worker->scene->sem_loading);
			line_counter = 0;
		}
	}
	return (NULL);
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
	return (create_trgb(0, (get_r(c1) + get_r(c2)) / 2, (get_g(c1) + get_g(c2)) / 2, (get_b(c1) + get_b(c2)) / 2));
}

int	get_color(t_worker *worker, int x, int y)
{
	int	bpp;

	bpp = worker->scene->mlx->bytes_per_pixel;
	return (*(int *)(worker->addr + ((y * worker->width) + x) * bpp));
}

void	set_color(t_worker *worker, int x, int y, int color)
{
	int	bpp;

	bpp = worker->scene->mlx->bytes_per_pixel;
	*(int *)(worker->addr + ((y * worker->width) + x) * bpp) = color;
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


void	fill_in_horizontal(t_worker *worker)
{
	int				x;
	int				y;
	int				avg_color;
	t_intersections	arr;
	unsigned int	shape_idx;
	t_ray			ray;


	y = worker->y_start - 1;
	while (++y < worker->y_end)
	{
		x = 1;
		while (x < worker->width)
		{
			if (x + 1 < worker->width)
			{
				int c1 = get_color(worker, x - 1, y);
				int c2 = get_color(worker, x + 1, y);
				if (color_difference(c1, c2) > 5)
				{
					ray_for_pixel(&ray, &worker->scene->camera, x, y);
					shape_idx = -1;
					arr.count = 0;
					while (++shape_idx < worker->scene->count.shape_count)
						intersect(&worker->scene->shapes[shape_idx], &ray, &arr);
					calculate_lighting(&arr, worker, &ray, (y * worker->width \
						+ x) * worker->scene->mlx->bytes_per_pixel);
						
				}
				else
				{
					avg_color = color_avg(c1, c2);
					set_color(worker, x, y, avg_color);
				}
			}
			else
			{
				set_color(worker, x, y, get_color(worker, x - 1, y));
			}
			x += 2;
		}
	}
}

void	fill_in_vertical(t_worker *worker)
{
	int				x;
	int				y;
	int				avg_color;
	t_intersections	arr;
	unsigned int	shape_idx;
	t_ray			ray;


	y = worker->y_start + 1;
	while (y < worker->y_end)
	{
		x = 0;
		while (x < worker->width)
		{
			if (y + 1 < worker->height)
			{
				int c1 = get_color(worker, x, y - 1);
				int c2 = get_color(worker, x, y + 1);
				if (color_difference(c1, c2) > 5)
				{
					ray_for_pixel(&ray, &worker->scene->camera, x, y);
					shape_idx = -1;
					arr.count = 0;
					while (++shape_idx < worker->scene->count.shape_count)
						intersect(&worker->scene->shapes[shape_idx], &ray, &arr);
					calculate_lighting(&arr, worker, &ray, (y * worker->width \
						+ x) * worker->scene->mlx->bytes_per_pixel);
				}
				else
				{
					avg_color = color_avg(c1, c2);
					set_color(worker, x, y, avg_color);
				}
			}
			else
			{
				set_color(worker, x, y, get_color(worker, x, y - 1));
			}
			x++;
		}
		y += 2;
	}
}

void	*render_scene_dirty(t_worker *worker)
{
	t_intersections	arr;
	int				x;
	int				y;
	unsigned int	shape_idx;
	t_ray			ray;

	int	line_counter = 0;
	y = worker->y_start;
	while (y < worker->y_end)
	{
		x = 0;
		while (x < worker->width)
		{
			*(unsigned int *)(worker->addr + \
				(y * worker->width + x) * \
				worker->scene->mlx->bytes_per_pixel) = 0;
			ray_for_pixel(&ray, &worker->scene->camera, x, y);
			shape_idx = -1;
			arr.count = 0;
			while (++shape_idx < worker->scene->count.shape_count)
				intersect(&worker->scene->shapes[shape_idx], &ray, &arr);
			calculate_lighting(&arr, worker, &ray, (y * worker->width \
				+ x) * worker->scene->mlx->bytes_per_pixel);
			x += 2;
		}
		line_counter++;
		if (worker->scene->edit_mode == false && (line_counter == (worker->y_end - worker->y_start) / 10))
		{
			sem_post(worker->scene->sem_loading);
			line_counter = 0;
		}
		y += 2;
	}
	fill_in_horizontal(worker);
	fill_in_vertical(worker);
	return (NULL);
}
