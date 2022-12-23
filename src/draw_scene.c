/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 20:19:41 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/23 15:10:40 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	remove_element_at(t_shape **containers, int idx, int *count)
{
	int i;

	for(i = idx; i < *count - 1; i++)
	{
		containers[i] = containers[i + 1];
	}
	*count -= 1;
}

bool	includes_shape(t_shape **containers, t_shape *shape, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (containers[i]->id == shape->id)
		{
			return (true);
		}
		i++;
	}
	return (false);
}

void	get_iors(t_intersect *intersection, t_intersections *xs)
{
	t_shape	*containers[SHAPE_MAX];
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (i < xs->count)
	{
		if (fabs(xs->arr[i].time - intersection->time) < 0.01 && xs->arr[i].shape == intersection->shape)
		{
			if (count == 0)
				intersection->n1 = 1.0;
			else
				intersection->n1 = containers[count - 1]->ior;
		}
		if (includes_shape(containers, xs->arr[i].shape, count) == true)
		{
			remove_element_at(containers, i, &count);
			// i++;
		}
		else
		{
			containers[count] = xs->arr[i].shape;
			count++;
		}
		if (fabs(xs->arr[i].time - intersection->time) < 0.01 && xs->arr[i].shape == intersection->shape)
		{
			if (count == 0)
				intersection->n2 = 1.0;
			else
				intersection->n2 = containers[count - 1]->ior;
			break;
		}
		i++;
	}
}

void	prepare_computations(t_scene *scene, t_intersect *intersection, t_ray *ray, t_intersections *xs)
{
	if (intersection->shape->transparency != 0 || scene->refraction_depth == 0)
		get_iors(intersection, xs);

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
	scale_vec(&intersection->over_point, &intersection->normal, EPSILON);
	add_vec(&intersection->over_point, &intersection->point,
		&intersection->over_point);
	scale_vec(&intersection->under_point, &intersection->normal, EPSILON);
	sub_vec(&intersection->under_point, &intersection->point,
		&intersection->under_point);
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
	int w;
	int h;
	(void)color;
	void *img = mlx_xpm_file_to_image(scene->mlx->mlx, "./left_arrow.xpm", &w, &h);
	if (img == NULL)
	{
		printf("Could not open imaege file\n");
	}
	mlx_put_image_to_window(scene->mlx, scene->mlx->mlx_win, img, 0, y - h / 2);
	mlx_destroy_image(scene->mlx, img);
}

void	draw_right_arrow(t_scene *scene, int y, int color)
{
	int w;
	int h;
	(void)color;
	void *img = mlx_xpm_file_to_image(scene->mlx->mlx, "./right_arrow.xpm", &w, &h);
	if (img == NULL)
	{
		printf("Could not open imaege file\n");
	}
	mlx_put_image_to_window(scene->mlx, scene->mlx->mlx_win, img, scene->display_w - 25, y - h / 2);
	mlx_destroy_image(scene->mlx, img);
}

void	draw_up_arrow(t_scene *scene, int x, int color)
{
	int w;
	int h;
	(void)color;
	void *img = mlx_xpm_file_to_image(scene->mlx->mlx, "./up_arrow.xpm", &w, &h);
	if (img == NULL)
	{
		printf("Could not open imaege file\n");
	}
	mlx_put_image_to_window(scene->mlx, scene->mlx->mlx_win, img, x - w / 2, 0);
	mlx_destroy_image(scene->mlx, img);
}

void	draw_down_arrow(t_scene *scene, int x, int color)
{
	int w;
	int h;
	(void)color;
	void *img = mlx_xpm_file_to_image(scene->mlx->mlx, "./down_arrow.xpm", &w, &h);
	if (img == NULL)
	{
		printf("Could not open imaege file\n");
	}
	mlx_put_image_to_window(scene->mlx, scene->mlx->mlx_win, img, x - w / 2, scene->display_h - 25);
	mlx_destroy_image(scene->mlx, img);
}

void	draw_bottom_left_arrow(t_scene *scene, int color)
{
	int w;
	int h;
	(void)color;
	void *img = mlx_xpm_file_to_image(scene->mlx->mlx, "./bottom_left.xpm", &w, &h);
	if (img == NULL)
	{
		printf("Could not open imaege file\n");
	}
	mlx_put_image_to_window(scene->mlx, scene->mlx->mlx_win, img, 0, scene->display_h - 35);
	mlx_destroy_image(scene->mlx, img);
}

void	draw_bottom_right_arrow(t_scene *scene, int color)
{
	int w;
	int h;
	(void)color;
	void *img = mlx_xpm_file_to_image(scene->mlx->mlx, "./bottom_right.xpm", &w, &h);
	if (img == NULL)
	{
		printf("Could not open imaege file\n");
	}
	mlx_put_image_to_window(scene->mlx, scene->mlx->mlx_win, img, scene->display_w - 35, scene->display_h - 35);
	mlx_destroy_image(scene->mlx, img);
}

void	draw_top_right_arrow(t_scene *scene, int color)
{
	int w;
	int h;
	(void)color;
	void *img = mlx_xpm_file_to_image(scene->mlx->mlx, "./top_right.xpm", &w, &h);
	if (img == NULL)
	{
		printf("Could not open imaege file\n");
	}
	mlx_put_image_to_window(scene->mlx, scene->mlx->mlx_win, img, scene->display_w - 35, 0);
	mlx_destroy_image(scene->mlx, img);
}

void	draw_top_left_arrow(t_scene *scene, int color)
{
	int w;
	int h;
	(void)color;
	void *img = mlx_xpm_file_to_image(scene->mlx->mlx, "./top_left.xpm", &w, &h);
	if (img == NULL)
	{
		printf("Could not open imaege file\n");
	}
	mlx_put_image_to_window(scene->mlx, scene->mlx->mlx_win, img, 0, 0);
	mlx_destroy_image(scene->mlx, img);
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
}

void	draw_arrow(t_scene *scene, int x, int y, int color)
{
	if (x <= 0 && y > 0 && y < scene->display_h)
		draw_left_arrow(scene, y, color);
	if (x >= scene->display_w && y > 0 && y < scene->display_h)
		draw_right_arrow(scene, y, color);
	if (y <= 0 && x >= 0 && x < scene->display_w)
		draw_up_arrow(scene, x, color);
	if (y >= scene->display_h && x > 0 && x < scene->display_w)
		draw_down_arrow(scene, x, color);
	if (x <= 0 && y >= scene->display_h)
		draw_bottom_left_arrow(scene, color);
	if (x >= scene->display_w && y >= scene->display_h)
		draw_bottom_right_arrow(scene, color);
	if (x >= scene->display_w && y <= 0)
		draw_top_right_arrow(scene, color);
	if (x <= 0 && y <= 0)
		draw_top_left_arrow(scene, color);
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
	unsigned int	shape_idx = 0;
	t_shape		*shape;
	t_vector	origin_proj;
	t_vector	origin;
	
	if (scene->edit_mode == false)
		return ;
	while (shape_idx < scene->count.shapes)
	{
		shape = &scene->shapes[shape_idx];
		if (shape->highlighted == false)
		{
			shape_idx++;
			continue;
		}
		ft_memcpy(&origin, &shape->origin, sizeof(t_vector));
		if (shape->type == SPHERE)
		{
			origin.x -= 0.2;
			origin.y += shape->radius;
		}
		if (shape->type == CYLINDER)
		{
			origin.x -= 0.2;
			origin.y += shape->height / 2;
		}
		mat_vec_multiply(&origin_proj, &scene->camera.transform, &origin);
		if (origin_proj.z > 0)
			return ;
		perspective_projection(&origin_proj, scene);
		if ((shape->type == SPHERE || shape->type == CYLINDER || shape->type == CUBE))
		{
			char str[1000];
			if (shape->type == SPHERE)
				mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (origin_proj.x * scene->display_w), (origin_proj.y - 0.12) * scene->display_h , 0xffffff, "Sphere");
			if (shape->type == CYLINDER)
				mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (origin_proj.x * scene->display_w), (origin_proj.y - 0.12) * scene->display_h , 0xffffff, "Cylinder");
			sprintf(str, "x: % 9.2f", shape->origin.x);
			mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (origin_proj.x * scene->display_w), (origin_proj.y - 0.10) * scene->display_h , 0xffffff, str);
			sprintf(str, "y: % 9.2f", shape->origin.y);
			mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (origin_proj.x * scene->display_w), (origin_proj.y - 0.08) * scene->display_h , 0xffffff, str);
			sprintf(str, "z: % 9.2f", shape->origin.z);
			mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (origin_proj.x * scene->display_w), (origin_proj.y - 0.06) * scene->display_h , 0xffffff, str);
			sprintf(str, "radius: %.2f", shape->radius);
			mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (origin_proj.x * scene->display_w), (origin_proj.y - 0.04) * scene->display_h , 0xffffff, str);
			// if (shape->type == SPHERE)
			// 	mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (scene->display_w) * 0.02, (scene->display_h) * (0.04), 0xffffff, "Sphere");
			// if (shape->type == CYLINDER)
			// 	mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (scene->display_w) * 0.02, (scene->display_h) * (0.04), 0xffffff, "Cylinder");
			// sprintf(str, "x: % 9.2f", shape->origin.x);
			// mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (scene->display_w) * 0.02, (scene->display_h) * (0.06), 0xffffff, str);
			// sprintf(str, "y: % 9.2f", shape->origin.y);
			// mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (scene->display_w) * 0.02, (scene->display_h) * (0.08), 0xffffff, str);
			// sprintf(str, "z: % 9.2f", shape->origin.z);
			// mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (scene->display_w) * 0.02, (scene->display_h) * (0.10), 0xffffff, str);
			// sprintf(str, "radius: %.2f", shape->radius);
			// mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, (scene->display_w) * 0.02, (scene->display_h) * (0.12), 0xffffff, str);
		}
		shape_idx++;
	}
}


void	dda(t_scene *scene, float x1, float x2, float y1, float y2, int color)
{
	float	dy;
	float	dx;
	float	c;
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
	unsigned int	shape_idx = 0;
	t_shape		*shape;
	t_vector	origin_proj;

	if (scene->edit_mode == false)
		return ;
	while (shape_idx < scene->count.shapes)
	{
		shape = &scene->shapes[shape_idx];
		if (shape->highlighted == false)
		{
			shape_idx++;
			continue;
		}
		mat_vec_multiply(&origin_proj, &scene->camera.transform, &shape->origin);
		perspective_projection(&origin_proj, scene);
		if (shape->type == SPHERE || shape->type == CYLINDER || shape->type == CUBE)
		{
			if (origin_proj.z < 0)
				draw_marker(scene, (int)(origin_proj.x * scene->display_w), (int)(origin_proj.y  * scene->display_h) , 0x00ffff);
			mlx_put_image_to_window(scene->mlx->mlx, scene->mlx->mlx_win, scene->mlx->display_img, 0, 0);
			#ifdef __APPLE__
			draw_arrow(scene,  (int)(origin_proj.x * scene->display_w), (int)(origin_proj.y  * scene->display_h) , 0x00ffff);
			#endif
		}
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
		while (x < scene->display_w * 0.15)
		{
			dst = scene->mlx->info_addr + (unsigned int)(y * scene->display_w * 0.15 + x) * scene->mlx->bytes_per_pixel;
			*(unsigned int*)dst = create_trgb(30, (x * 5) / (scene->display_w * 0.15 ), (x * 5) / (scene->display_w * 0.12), (x * 5) / (scene->display_w * 0.12));
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(scene->mlx->mlx, scene->mlx->mlx_win, scene->mlx->info_img, 0, 0);
	int	starting_x = (scene->display_w) * 0.011;
	int	starting_y = (scene->display_h) * (0.05);
	int	gap = scene->display_h * 0.03;
	int color = 0x99e9f2;
	if (scene->edit_mode == true && scene->camera_mode == false)
	{
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, scene->display_w * 0.035, starting_y, color, "[Edit mode]");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap, color, "WASDQE: Move");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap * 2, color, "C:      Camera mode");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap * 3, color, "TAB:    Switch Shape");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap * 4, color, "+/-:    Scale Shape");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap * 5, color, "R:      Reflections");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap * 6, color, "M:      Menu");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap * 7, color, "Space:  Render");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap * 8, color, "1-9:    Colors");
	}
	if (scene->edit_mode == true && scene->camera_mode == true)
	{
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, scene->display_w * 0.035, starting_y, color, "[Camera mode]");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap, color, "WASDQE:     Move");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap * 2, color, "C:          Edit mode");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap * 3, color, "Arrow Keys: Look");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap * 4, color, "R:          Reflect");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap * 5, color, "M:          Menu");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap * 6, color, "Space:      Render");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap * 7, color, "+/-:        Res scale");
		mlx_string_put(scene->mlx->mlx, scene->mlx->mlx_win, starting_x, starting_y + gap * 8, color, "1-9:        Colors");
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
	float			elapsed;
	int				i;

	mlx = scene->mlx;
	init_workers(workers, scene);
	clock_gettime(CLOCK_MONOTONIC, &start);
	i = 0;
	while (i < NUM_THREADS)
	{
		// pthread_create(&threads[i], NULL, (void *)render_scene, &workers[i]);
		// pthread_create(&threads[i], NULL, (void *)render_scene_fast, &workers[i]);
		pthread_create(&threads[i], NULL, (void *)render_scene_faster, &workers[i]);
		i++;
	}
	if (scene->edit_mode == false)
	{
		int sem_counter = 0;
		ft_putstr_fd("[", 1);
		int load = 0;
		while (load < NUM_THREADS * 5)
		{
			ft_putstr_fd(RED".", 1);
			load++;
		}
		ft_putstr_fd(RESET"]", 1);
		ft_putstr_fd("\r", 1);
		ft_putstr_fd("[", 1);
		while (sem_counter < NUM_THREADS * 5)
		{
			ft_putstr_fd(GREEN"=", 1);
			sem_wait(scene->sem_loading);
			sem_counter++;
		}
		ft_putstr_fd(RESET"]\n", 1);
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
	// printf("render time is %f\n", elapsed);
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
	// printf("scale time is %f\n", elapsed);
	clock_gettime(CLOCK_MONOTONIC, &start);
	if (scene->edit_mode == false)
		mlx_put_image_to_window(scene->mlx->mlx, scene->mlx->mlx_win, scene->mlx->display_img, 0, 0);
	else
	{
		draw_shape_marker(scene);
		if (scene->camera_mode == false)
			draw_shape_info(scene);
	}
		
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	// printf("draw time is %f\n", elapsed);
}
