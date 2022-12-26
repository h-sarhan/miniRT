/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_controls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 10:20:14 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/26 20:10:19 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	mouse_down(int key_code, int x, int y, t_scene *scene)
{
	(void)x;
	(void)y;
	scene->mouse.key = key_code;
	if (scene->settings.edit_mode == false || scene->mouse.key != LEFT_MOUSE_DOWN)
		return (0);
	if (x < 0 || y < 0 || x >= scene->settings.display_w || y >= scene->settings.display_h)
		return (0);
	scene->mouse.x = x;
	scene->mouse.y = y;
	t_ray	mouse_selection;
	ray_for_pixel(&mouse_selection, &scene->camera, 
		scene->mouse.x * scene->settings.edit_w / scene->settings.display_w,
		 scene->mouse.y * scene->settings.edit_h / scene->settings.display_h);
	int	shape_idx;
	t_intersections	arr;
	shape_idx = -1;
	arr.count = 0;
	while (++shape_idx < scene->count.shapes)
		intersect(&scene->shapes[shape_idx], &mouse_selection, &arr);
	t_intersection	*itx = hit(&arr);
	if (itx->shape->type != PLANE)
	{
		scene->shapes[scene->shape_idx].props.highlighted = false;
		scene->shape_idx = itx->shape->id;
		itx->shape->props.highlighted = true;
	}
	draw_scene(scene);
	return (0);
}

int	mouse_up(int key_code, int x, int y, t_scene *scene)
{
	(void)x;
	(void)y;
	(void)key_code;
	scene->mouse.key = 0;
	return (0);
}

int	mouse_rotate(t_scene *scene)
{
	if (scene->mouse.key == LEFT_MOUSE_DOWN && scene->settings.edit_mode == true)
	{
		scene->mouse.prev_x = scene->mouse.x;
		scene->mouse.prev_y = scene->mouse.y;
		#ifdef __linux__
			mlx_mouse_get_pos(scene->disp->mlx, scene->disp->win, &scene->mouse.x, &scene->mouse.y);
		#else
			mlx_mouse_get_pos(scene->disp->win, &scene->mouse.x, &scene->mouse.y);
		#endif
		if (abs(scene->mouse.x - scene->mouse.prev_x) > 1 || abs(scene->mouse.y - scene->mouse.prev_y) > 1)
		{
			float	x_diff = abs(scene->mouse.x - scene->mouse.prev_x);
			float	y_diff = abs(scene->mouse.y - scene->mouse.prev_y);
			float	degrees = max3((x_diff + y_diff) * 0.3, 3, 0);
			t_vector	rot_dir;
			rot_dir.x = scene->mouse.x - scene->mouse.prev_x;
			rot_dir.y = scene->mouse.y - scene->mouse.prev_y;
			rot_dir.z = 0;
			rot_dir.w = 0;
			rot_dir.x *= -1;
			normalize_vec(&rot_dir);
			t_vector	into;
			ft_bzero(&into, sizeof(t_vector));
			into.z = 1;
			t_vector	rot_axis;
			cross_product(&rot_axis, &into, &rot_dir);
			normalize_vec(&rot_axis);
			t_mat4	rot_mat;
			axis_angle(&rot_mat, &rot_axis, deg_to_rad(degrees));
			t_mat4	mat_copy;
			ft_memcpy(&mat_copy, &scene->shapes[scene->shape_idx].added_rots, sizeof(t_mat4));
			mat_multiply(&scene->shapes[scene->shape_idx].added_rots, &rot_mat, &mat_copy);
			collide(scene, true, 100, &scene->shapes[scene->shape_idx]);
			calculate_transforms(scene);
			draw_scene(scene);
		}
	}
	return (0);
}