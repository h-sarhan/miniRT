/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 20:19:41 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/14 11:03:44 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	prepare_computations(t_intersect *intersection, t_ray *ray)
{
	ray_position(&intersection->point, ray, intersection->time);
	intersection->normal = normal_at(intersection->shape, &intersection->point);
	negate_vec(&intersection->eye, &ray->direction);
	intersection->eye.w = 0;
	if (dot_product(&intersection->normal, &intersection->eye) < 0)
	{
		intersection->inside = true;
		negate_vec(&intersection->normal, &intersection->normal);
	}
	else
		intersection->inside = false;
	scale_vec(&intersection->over_point, &intersection->normal, 0.001);
	add_vec(&intersection->over_point, &intersection->point,
		&intersection->over_point);
	reflect(&intersection->reflect_vec, &ray->direction, &intersection->normal);
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

void	init_workers(t_worker *workers, t_scene *scene)
{
	unsigned int	i;

	i = 0;
	while (i < NUM_THREADS)
	{
		if (scene->edit_mode == true)
		{
			workers[i].height = scene->edit_h;
			workers[i].width = scene->edit_w;
			workers[i].addr = scene->mlx->edit_addr;
		}
		else
		{
			workers[i].height = scene->render_h;
			workers[i].width = scene->render_w;
			workers[i].addr = scene->mlx->render_addr;
		}
		workers[i].max_workers = NUM_THREADS;
		workers[i].worker_id = i;
		workers[i].scene = scene;
		workers[i].y_start = (workers[i].height / (float)NUM_THREADS) * i;
		workers[i].y_end = (workers[i].height / (float)NUM_THREADS) * (i + 1);
		workers[i].y_scale_start = (scene->display_h / (float)NUM_THREADS) * i;
		workers[i].y_scale_end = (scene->display_h / (float)NUM_THREADS) * (i + 1);
		i++;
	}
}

void	calculate_lighting(t_intersections *arr, t_worker *worker, t_ray *ray,
	int pixel)
{
	t_intersect		*itx;
	unsigned int	light_idx;
	t_color			final_color;
	t_color			light_color;

	itx = hit(arr);
	if (itx != NULL)
	{
		prepare_computations(itx, ray);
		ft_bzero(&final_color, sizeof(t_color));
		light_idx = 0;
		while (light_idx < worker->scene->count.light_count)
		{
			light_color = lighting(itx, worker->scene, light_idx);
			t_color	reflected  = reflected_color(worker->scene, itx, worker->scene->reflection_depth, light_idx);
			add_colors(&final_color, &final_color, &light_color);
			add_colors(&final_color, &final_color, &reflected);
			light_idx++;
		}
		itx->shape->mlx_color = create_mlx_color(&final_color);
			*(int *)(worker->addr + pixel) = itx->shape->mlx_color;
		
	}
}

void	*render_scene(t_worker *worker)
{
	t_intersections	arr;
	int				x;
	int				y;
	unsigned int	shape_idx;
	t_ray			ray;

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
				intersect(&worker->scene->shapes[shape_idx], &ray, &arr);
			calculate_lighting(&arr, worker, &ray, (y * worker->width \
				+ x) * worker->scene->mlx->bytes_per_pixel);
		}
	}
	return (NULL);
}

void	*nearest_neighbours_scaling(t_worker *worker)
{
	int			x;
	int			y;
	int			src_x;
	int			src_y;


	y = worker->y_scale_start - 1;
	while (++y < worker->y_scale_end)
	{
		x = -1;
		while (++x < worker->scene->display_w)
		{
			src_x = round((x / (float)worker->scene->display_w) * \
			worker->width);
			src_y = round((y / (float)worker->scene->display_h) * \
			worker->height);
			src_x = min(src_x, worker->width - 1);
			src_y = min(src_y, worker->height - 1);
			*(unsigned int *)(worker->scene->mlx->display_addr + (y * \
			worker->scene->display_w + x) * worker->scene->mlx->bytes_per_pixel) = \
			*(unsigned int *)(worker->addr + (src_y * \
			worker->width + src_x) * \
			worker->scene->mlx->bytes_per_pixel);
		}
	}
	return (NULL);
}

void	draw_left_arrow(t_scene *scene, int y, int color)
{
	dda(scene, 0, 7, y, y + 7, color);

	dda(scene, 0, 15, y, y, color);

	dda(scene, 0, 7, y, y - 7, color);
}

void	draw_right_arrow(t_scene *scene, int y, int color)
{
	dda(scene, scene->display_w, scene->display_w - 7, y, y + 7, color);

	dda(scene, scene->display_w, scene->display_w - 15, y, y, color);

	dda(scene, scene->display_w, scene->display_w - 7, y, y - 7, color);
}

void	draw_up_arrow(t_scene *scene, int x, int color)
{
	dda(scene, x, x + 7, 0, 7, color);

	dda(scene, x, x, 0, 15, color);

	dda(scene, x, x - 7, 0, 7, color);
}

void	draw_down_arrow(t_scene *scene, int x, int color)
{
	dda(scene, x, x + 7, scene->display_h, scene->display_h - 7, color);

	dda(scene, x, x, scene->display_h, scene->display_h - 15, color);

	dda(scene, x, x - 7, scene->display_h, scene->display_h - 7, color);
}

void	draw_bottom_left_arrow(t_scene *scene, int color)
{
	dda(scene, 0, 10, scene->display_h - 5, scene->display_h - 5, color);
	dda(scene, 0, 10, scene->display_h - 5, scene->display_h - 15, color);
	dda(scene, 0, 0, scene->display_h - 5, scene->display_h - 15, color);
}

void	draw_bottom_right_arrow(t_scene *scene, int color)
{
	dda(scene, scene->display_w - 1, scene->display_w - 10, scene->display_h - 5, scene->display_h - 5, color);
	dda(scene, scene->display_w - 1, scene->display_w - 10, scene->display_h - 5, scene->display_h - 15, color);
	dda(scene, scene->display_w - 1, scene->display_w - 0, scene->display_h - 5, scene->display_h - 15, color);
}

void	draw_top_right_arrow(t_scene *scene, int color)
{
	dda(scene, scene->display_w - 1, scene->display_w - 10,  3, 3, color);
	dda(scene, scene->display_w - 1, scene->display_w - 10, 3, 15, color);
	dda(scene, scene->display_w - 1, scene->display_w - 0, 3, 15, color);
}

void	draw_top_left_arrow(t_scene *scene, int color)
{
	dda(scene, 1, 10,  3, 3, color);
	dda(scene, 1, 10, 3, 15, color);
	dda(scene, 1, 0, 3, 15, color);
}

void	draw_marker(t_scene *scene, int x, int y, int color)
{
	char	*dst;
	
	if (x > 0 && y > 0 && x < scene->display_w && y < scene->display_h)
	{
		dst = scene->mlx->display_addr + (y * scene->display_w + x) * scene->mlx->bytes_per_pixel;
		*(unsigned int*)dst = color;
		dst = scene->mlx->display_addr + ((y + 1) * scene->display_w + x) * scene->mlx->bytes_per_pixel;
		*(unsigned int*)dst = color;
		dst = scene->mlx->display_addr + ((y - 1) * scene->display_w + x) * scene->mlx->bytes_per_pixel;
		*(unsigned int*)dst = color;
		dst = scene->mlx->display_addr + (y * scene->display_w + x + 1) * scene->mlx->bytes_per_pixel;
		*(unsigned int*)dst = color;
		dst = scene->mlx->display_addr + ((y + 1) * scene->display_w + x + 1) * scene->mlx->bytes_per_pixel;
		*(unsigned int*)dst = color;
		dst = scene->mlx->display_addr + ((y - 1) * scene->display_w + x + 1) * scene->mlx->bytes_per_pixel;
		*(unsigned int*)dst = color;
		dst = scene->mlx->display_addr + (y * scene->display_w + x - 1) * scene->mlx->bytes_per_pixel;
		*(unsigned int*)dst = color;
		dst = scene->mlx->display_addr + ((y - 1) * scene->display_w + x - 1) * scene->mlx->bytes_per_pixel;
		*(unsigned int*)dst = color;
		dst = scene->mlx->display_addr + ((y + 1) * scene->display_w + x - 1) * scene->mlx->bytes_per_pixel;
		*(unsigned int*)dst = color;
	}
	if (x <= 0 && y > 0 && y < scene->display_h)
		draw_left_arrow(scene, y, 0x00ffff);
	if (x >= scene->display_w && y > 0 && y < scene->display_h)
		draw_right_arrow(scene, y, 0x00ffff);
	if (y <= 0 && x >= 0 && x < scene->display_w)
		draw_up_arrow(scene, x, 0x00ffff);
	if (y >= scene->display_h && x > 0 && x < scene->display_w)
		draw_down_arrow(scene, x, 0x00ffff);
	if (x <= 0 && y >= scene->display_h)
		draw_bottom_left_arrow(scene, 0x00ffff);
	if (x >= scene->display_w && y >= scene->display_h)
		draw_bottom_right_arrow(scene, 0x00ffff);
	if (x >= scene->display_w && y <= 0)
		draw_top_right_arrow(scene, 0x00ffff);
	if (x <= 0 && y <= 0)
		draw_top_left_arrow(scene, 0x00ffff);
}

void	perspective_projection(t_vector *point, const t_scene *scene)
{
	point->x /= -point->z;
	point->y /= -point->z;
	point->x = (point->x + scene->camera.half_width) / (scene->camera.half_width * 2);
	point->y = (point->y + scene->camera.half_height) / (scene->camera.half_height * 2);
	point->x = 1 - point->x;
	point->y = 1 - point->y;
}

void	draw_shape_info(t_scene *scene)
{
	if (scene->edit_mode == false)
		return ;
	unsigned int	shape_idx = 0;
	t_shape	*shape;

	while (shape_idx < scene->count.shape_count)
	{
		shape = &scene->shapes[shape_idx];
		if (shape->highlighted == false)
		{
			shape_idx++;
			continue;
		}
		if (shape->type == SPHERE)
		{
			char str[1000];
			mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (scene->display_w) * 0.02, (scene->display_h) * (0.05 - 0.01), 0xffffff, "Sphere");
			sprintf(str, "x: % 9.2f", shape->origin.x);
			mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (scene->display_w) * 0.02, (scene->display_h) * (0.07 - 0.01), 0xffffff, str);
			sprintf(str, "y: % 9.2f", shape->origin.y);
			mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (scene->display_w) * 0.02, (scene->display_h) * (0.09 - 0.01), 0xffffff, str);
			sprintf(str, "z: % 9.2f", shape->origin.z);
			mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (scene->display_w) * 0.02, (scene->display_h) * (0.11 - 0.01), 0xffffff, str);
			sprintf(str, "radius: %.2f", shape->radius);
			mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (scene->display_w) * 0.02, (scene->display_h) * (0.13 - 0.01), 0xffffff, str);
		}
		shape_idx++;
	}
}


void	dda(t_scene *scene, double x1, double x2, double y1, double y2, int color)
{
	double	dy;
	double	dx;
	double	c;
	int		i;
	char	*dst;

	dx = (x2 - x1);
	dy = (y2 - y1);
	if (fabs(dx) > fabs(dy))
		c = fabs(dx);
	else
		c = fabs(dy);
	i = 0;
	dx /= c;
	dy /= c;
	while (i <= c)
	{
		if (y1 > scene->display_h || x1 > scene->display_w)
			break ;
		dst = scene->mlx->display_addr + (int)(y1 * scene->display_w + x1) * scene->mlx->bytes_per_pixel;
		*(unsigned int*)dst = color;
		y1 += dy;
		x1 += dx;
		i++;
	}
}


void	draw_shape_marker(t_scene *scene)
{
	if (scene->edit_mode == false)
		return ;
	unsigned int	shape_idx = 0;
	t_shape	*shape;
	t_vector	cam_point;

	while (shape_idx < scene->count.shape_count)
	{
		shape = &scene->shapes[shape_idx];
		if (shape->highlighted == false)
		{
			shape_idx++;
			continue;
		}
		mat_vec_multiply(&cam_point, &scene->camera.transform, &shape->origin);
		perspective_projection(&cam_point, scene);
		if (shape->type == SPHERE)
			draw_marker(scene, (int)(cam_point.x * scene->display_w), (int)(cam_point.y  * scene->display_h) , 0x00ffff);
		shape_idx++;
	}
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}


void	draw_menu(t_scene *scene)
{
	int x = 0;
	int y = 0;
	char *dst;
	while (y < scene->display_h)
	{
		x = 0;
		while (x < scene->display_w * 0.12)
		{
			dst = scene->mlx->info_addr + (unsigned int)(y * scene->display_w * 0.12 + x) * scene->mlx->bytes_per_pixel;
			*(unsigned int*)dst = create_trgb(30, (x * 5) / (scene->display_w * 0.12), (x * 5) / (scene->display_w * 0.12), (x * 5) / (scene->display_w * 0.12));
			x++;
		}
		y++;
	}

}

/**
 * @brief Draws a scene
 * @param scene A struct describing the current scene
 */
void	draw_scene(t_scene *scene)
{
	t_mlx			*mlx;
	t_worker		workers[NUM_THREADS];
	pthread_t		threads[NUM_THREADS];
	struct timespec	start;
	struct timespec	finish;
	double			elapsed;
	int				i;

	mlx = scene->mlx;
	init_workers(workers, scene);
	clock_gettime(CLOCK_MONOTONIC, &start);
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_create(&threads[i], NULL, (void *)render_scene, &workers[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("render time is %f\n", elapsed);
	clock_gettime(CLOCK_MONOTONIC, &start);
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_create(&threads[i], NULL, (void *)nearest_neighbours_scaling,
			&workers[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("scale time is %f\n", elapsed);
	draw_shape_marker(scene);
	mlx_put_image_to_window(mlx->mlx, mlx->mlx_win, mlx->display_img, 0, 0);
	if (scene->menu == true && scene->edit_mode == true)
	{
		draw_menu(scene);
		mlx_put_image_to_window(scene->mlx->mlx, scene->mlx->mlx_win, scene->mlx->info_img, 0, 0);
		draw_shape_info(scene);
	}
}
