/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 19:35:57 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/18 13:20:06 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <unistd.h>


void	handle_color_change(int key, t_scene *scene)
{
	t_color	color;
	color = scene->shapes[scene->shape_idx % scene->count.shape_count].color;
	if (key == KEY_1)
	{
		if (color.r + 5 / 255.0 < 1)
			color.r += 5 / 255.0;
	}
	if (key == KEY_2)
	{
		if (color.r - 5 / 255.0 > 0)
			color.r -= 5 / 255.0;
	}
	if (key == KEY_3)
	{
		if (color.g  + 5 / 255.0 < 1)
			color.g += 5 / 255.0;
	}
	if (key == KEY_4)
	{
		if (color.g > 5 / 255.0)
			color.g -= 5 / 255.0;
	}
	if (key == KEY_5)
	{
		if (color.b + 5 / 255.0 < 1)
			color.b += 5 / 255.0;
	}
	if (key == KEY_6)
	{
		if (color.b > 5 / 255.0)
			color.b -= 5 / 255.0;
	}
	if (key == KEY_7)
	{
		color.r = 0x6d / 255.0;
		color.g = 0x65 / 255.0;
		color.b = 0x6d / 255.0;
	}
	if (key == KEY_8)
	{
		color.r = 0x15 / 255.0;
		color.g = 0xaa / 255.0;
		color.b = 0xbf / 255.0;
	}
	if (key == KEY_9)
	{
		color.r = 0xf7 / 255.0;
		color.g = 0x67 / 255.0;
		color.b = 0x07 / 255.0;
	}
	scene->shapes[scene->shape_idx % scene->count.shape_count].color = color;
}


void	look_at(t_scene *scene)
{
	t_shape *shape = &scene->shapes[scene->shape_idx % scene->count.shape_count];
	t_vector	final_pos = shape->origin;
	final_pos.z = final_pos.z - shape->radius - 2;
	t_vector	final_dir;
	sub_vec(&final_dir, &shape->origin, &final_pos);
	normalize_vec(&final_dir);
	// scene->camera.position.x = shape->origin.x;
	// scene->camera.position.y = shape->origin.y;
	// scene->camera.position.z = shape->origin.z - shape->radius - 2;
	scene->camera.position = final_pos;
	scene->camera.orientation = final_dir;
	scene->camera.theta = atan(scene->camera.orientation.z / scene->camera.orientation.x);
	scene->camera.phi = acos(scene->camera.orientation.y);
}

int	set_key_down(int key, t_scene *scene)
{
	printf("%d\n", key);
	// scene->shapes[scene->shape_idx % scene->count.shape_count].highlighted = false;
	if (key == KEY_1
	|| key == KEY_2
	|| key == KEY_3
	|| key == KEY_4
	|| key == KEY_5
	|| key == KEY_6
	|| key == KEY_7
	|| key == KEY_8
	|| key == KEY_9
	)
	{
		handle_color_change(key, scene);
		calculate_transforms(scene);
		draw_scene(scene);
	}
	if (key == KEY_M)
	{
		scene->menu = !scene->menu;
		calculate_transforms(scene);
		draw_scene(scene);
	}
	if (key == KEY_O && scene->edit_mode == true)
	{
		look_at(scene);
		camera_init(&scene->camera, scene);
		calculate_transforms(scene);
		draw_scene(scene);
	}
	if (key == KEY_SPACE)
	{
		scene->edit_mode = !scene->edit_mode;
		if (scene->edit_mode == true)
		{
			scene->reflection_depth = REFLECTION_DEPTH;
		}
		else
		{
			scene->reflection_depth = REFLECTION_DEPTH;
		}
		camera_init(&scene->camera, scene);
		calculate_transforms(scene);
		draw_scene(scene);
	}
	if (key == KEY_TAB)
	{
		scene->shapes[scene->shape_idx % scene->count.shape_count].highlighted = false;
		scene->shape_idx++;
		while (scene->shapes[scene->shape_idx % scene->count.shape_count].type == PLANE)
		{
			scene->shape_idx++;
		}
		scene->shapes[scene->shape_idx % scene->count.shape_count].highlighted = true;
		camera_init(&scene->camera, scene);
		calculate_transforms(scene);
		draw_scene(scene);
	}
	if (key == KEY_R)
	{
		if (scene->reflection_depth == 0)
			scene->reflection_depth = REFLECTION_DEPTH;
		else
			scene->reflection_depth = 0;
		camera_init(&scene->camera, scene);
		calculate_transforms(scene);
		draw_scene(scene);
	}
	if (key == KEY_ESC)
	{
		int y = 0;
		while (y < scene->display_h)
		{
			int x = 0;
			while (x < scene->display_w)
			{
				*(unsigned int *)(scene->mlx->display_addr + (y * \
				scene->display_w + x) * scene->mlx->bytes_per_pixel) = 0xffffff;
				x++;
			}
			y++;
		}
		system("afplay sound.mp3");
		mlx_put_image_to_window(scene->mlx->mlx, scene->mlx->mlx_win, scene->mlx->display_img, 0, 0);
	}
	if (key == KEY_C)
	{
		scene->camera_mode = !scene->camera_mode;
		calculate_transforms(scene);
		draw_scene(scene);
	}
	if (key == KEY_W)
		scene->keys_held.w = true;
	if (key == KEY_A)
		scene->keys_held.a = true;
	if (key == KEY_S)
		scene->keys_held.s = true;
	if (key == KEY_D)
		scene->keys_held.d = true;
	if (key == KEY_UP)
		scene->keys_held.up = true;
	if (key == KEY_DOWN)
		scene->keys_held.down = true;
	if (key == KEY_LEFT)
		scene->keys_held.left = true;
	if (key == KEY_RIGHT)
		scene->keys_held.right = true;
	if (key == KEY_Q)
		scene->keys_held.q = true;
	if (key == KEY_E)
		scene->keys_held.e = true;
	if (key == KEY_PLUS)
		scene->keys_held.plus = true;
	if (key == KEY_MINUS)
		scene->keys_held.minus = true;
	return (0);
}

int	set_key_up(int key, t_scene *scene)
{
	if (key == KEY_C)
		scene->keys_held.c = false;
	if (key == KEY_TAB)
		scene->keys_held.tab = false;
	if (key == KEY_W)
		scene->keys_held.w = false;
	if (key == KEY_A)
		scene->keys_held.a = false;
	if (key == KEY_S)
		scene->keys_held.s = false;
	if (key == KEY_D)
		scene->keys_held.d = false;
	if (key == KEY_UP)
		scene->keys_held.up = false;
	if (key == KEY_DOWN)
		scene->keys_held.down = false;
	if (key == KEY_LEFT)
		scene->keys_held.left = false;
	if (key == KEY_RIGHT)
		scene->keys_held.right = false;
	if (key == KEY_Q)
		scene->keys_held.q = false;
	if (key == KEY_E)
		scene->keys_held.e = false;
	if (key == KEY_PLUS)
		scene->keys_held.plus = false;
	if (key == KEY_MINUS)
		scene->keys_held.minus = false;
	return (0);
}

int	key_handler(t_scene *scene)
{
	if (scene->camera_mode == true && scene->edit_mode == true)
	{
		if (scene->keys_held.w == true)
		{
			scene->camera.position.x += 0.5 *sin(scene->camera.phi)*cos(scene->camera.theta);
			scene->camera.position.z += 0.5 *sin(scene->camera.phi)*sin(scene->camera.theta);
			scene->camera.position.y += 0.5 *cos(scene->camera.phi);
		}
		if (scene->keys_held.a == true)
		{
			scene->camera.position.x += 0.5 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
			scene->camera.position.z += 0.5 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
			scene->camera.position.y += 0.5 *cos(M_PI / 2);
		}
		if (scene->keys_held.s == true)
		{
			scene->camera.position.x -= 0.5 *sin(scene->camera.phi)*cos(scene->camera.theta);
			scene->camera.position.z -= 0.5 *sin(scene->camera.phi)*sin(scene->camera.theta);
			scene->camera.position.y -= 0.5 *cos(scene->camera.phi);
		}
		if (scene->keys_held.d == true)
		{
			scene->camera.position.x += 0.5 *sin(M_PI / 2)*cos(scene->camera.theta - M_PI / 2);
			scene->camera.position.z += 0.5 *sin(M_PI / 2)*sin(scene->camera.theta - M_PI / 2);
			scene->camera.position.y += 0.5 *cos(M_PI / 2);
		}
		if (scene->keys_held.up == true)
		{
			if (scene->camera.phi > 0.5)
				scene->camera.phi -= 0.05;
			scene->camera.orientation.x = sin(scene->camera.phi) * cos(scene->camera.theta);
			scene->camera.orientation.z = sin(scene->camera.phi) * sin(scene->camera.theta);
			scene->camera.orientation.y = cos(scene->camera.phi);
		}
		if (scene->keys_held.down == true)
		{
			if (scene->camera.phi < M_PI - 0.5)
				scene->camera.phi += 0.05;
			scene->camera.orientation.x = sin(scene->camera.phi) * cos(scene->camera.theta);
			scene->camera.orientation.z = sin(scene->camera.phi) * sin(scene->camera.theta);
			scene->camera.orientation.y = cos(scene->camera.phi);
		}
		if (scene->keys_held.left == true)
		{
			scene->camera.theta += 0.10;
			scene->camera.orientation.x = sin(scene->camera.phi) * cos(scene->camera.theta);
			scene->camera.orientation.z = sin(scene->camera.phi) * sin(scene->camera.theta);
			scene->camera.orientation.y = cos(scene->camera.phi);
		}
		if (scene->keys_held.right == true)
		{
			scene->camera.theta -= 0.10;
			scene->camera.orientation.x = sin(scene->camera.phi) * cos(scene->camera.theta);
			scene->camera.orientation.z = sin(scene->camera.phi) * sin(scene->camera.theta);
			scene->camera.orientation.y = cos(scene->camera.phi);
		}
		if (scene->keys_held.q == true)
			scene->camera.position.y += 0.35;
		if (scene->keys_held.e == true)
			scene->camera.position.y -= 0.35;
		if (scene->keys_held.plus == true)
		{
			// if (scene->edit_scale < 0.7)
			// {
			// 	scene->edit_scale += 0.05;
			// 	scene->edit_w = 1920 * scene->edit_scale;
			// 	scene->edit_h = 1080 * scene->edit_scale;
			// 	camera_init(&scene->camera, scene);
			// 	calculate_transforms(scene);
			// 	draw_scene(scene);
			// }
		}
		if (scene->keys_held.minus == true)
		{
			// if (scene->edit_scale > 0.06)
			// {
			// 	scene->edit_scale -= 0.05;
			// 	scene->edit_w = 1920 * scene->edit_scale;
			// 	scene->edit_h = 1080 * scene->edit_scale;
			// 	camera_init(&scene->camera, scene);
			// 	calculate_transforms(scene);
			// 	draw_scene(scene);
			// }
		}
	}
	else if (scene->edit_mode == true)
	{
		if (scene->keys_held.w == true)
		{
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.x += 0.2 *sin(scene->camera.phi)*cos(scene->camera.theta);
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.z += 0.2 *sin(scene->camera.phi)*sin(scene->camera.theta);
		}
		if (scene->keys_held.a == true)
		{
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.x += 0.2 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.z += 0.2 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
		}
		if (scene->keys_held.s == true)
		{
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.x -= 0.2 *sin(scene->camera.phi)*cos(scene->camera.theta);
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.z -= 0.2 *sin(scene->camera.phi)*sin(scene->camera.theta);
		}
		if (scene->keys_held.d == true)
		{
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.x -= 0.2 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.z -= 0.2 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
		}
		if (scene->keys_held.plus == true)
			scene->shapes[scene->shape_idx % scene->count.shape_count].radius += 0.04;
		if (scene->keys_held.minus == true)
		{
			if (scene->shapes[scene->shape_idx % scene->count.shape_count].radius > 0.3)
				scene->shapes[scene->shape_idx % scene->count.shape_count].radius -= 0.04;
		}
		if (scene->keys_held.up == true)
			scene->lights[0].position.y += 0.3;
		if (scene->keys_held.down == true)
			scene->lights[0].position.y -= 0.3;
		if (scene->keys_held.left == true)
			scene->lights[0].position.x -= 0.3;
		if (scene->keys_held.right == true)
			scene->lights[0].position.x += 0.3;
		if (scene->keys_held.q == true)
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.y += 0.1;
		if (scene->keys_held.e == true)
			scene->shapes[scene->shape_idx % scene->count.shape_count].origin.y -= 0.1;
	}
	if (scene->edit_mode == true && (scene->keys_held.w
		|| scene->keys_held.a
		|| scene->keys_held.s
		|| scene->keys_held.d
		|| scene->keys_held.up
		|| scene->keys_held.down
		|| scene->keys_held.left
		|| scene->keys_held.right
		|| scene->keys_held.q
		|| scene->keys_held.e
		|| scene->keys_held.plus
		|| scene->keys_held.minus)
		)
	{
		calculate_transforms(scene);
		draw_scene(scene);
	}
	return (0);
}
