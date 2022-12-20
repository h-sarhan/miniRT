/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 19:35:57 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/20 18:04:13 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <unistd.h>


void	handle_color_change(int key, t_scene *scene)
{
	t_color	color;

	color = scene->shapes[scene->shape_idx].color;
	if (key == KEY_1 && (color.r + 10 / 255.0) < 1)
		color.r += 10 / 255.0;
	if (key == KEY_2 && (color.r - 10 / 255.0) > 0)
		color.r -= 10 / 255.0;
	if (key == KEY_3 && (color.g + 10 / 255.0) < 1)
		color.g += 10 / 255.0;
	if (key == KEY_4 && (color.g - 10 / 255.0) > 0)
		color.g -= 10 / 255.0;
	if (key == KEY_5 && (color.b + 10 / 255.0) < 1)
		color.b += 10 / 255.0;
	if (key == KEY_6 && (color.b - 10 / 255.0) > 0)
		color.b -= 10 / 255.0;
	scene->shapes[scene->shape_idx].color = color;
}

void	look_at(t_scene *scene)
{
	t_shape *shape = &scene->shapes[scene->shape_idx];
	t_vector	cam_to_object;
	t_vector	final_pos;
	sub_vec(&cam_to_object, &shape->origin, &scene->camera.position);
	normalize_vec(&cam_to_object);
	final_pos.x = cam_to_object.x *  -8 * shape->radius + shape->origin.x;
	final_pos.z = cam_to_object.z *  -8 * shape->radius + shape->origin.z;
	final_pos.y = shape->origin.y;
	final_pos.w = 1;
	scene->look_at.final_pos = final_pos;
	sub_vec(&scene->look_at.final_dir, &shape->origin, &final_pos);
	normalize_vec(&scene->look_at.final_dir);
	scene->look_at.current_dir = scene->camera.orientation;
	scene->look_at.initial_orientation = scene->camera.orientation;
	scene->look_at.trigger = true;
	scene->look_at.step_num = 0;
	sub_vec(&scene->look_at.pos_diff, &scene->look_at.final_pos, &scene->camera.position);
	scene->look_at.step_amount = vec_magnitude(&scene->look_at.pos_diff) * 1.5;
	printf("step amount = %d\n", scene->look_at.step_amount);
	sub_vec(&scene->look_at.dir_diff, &scene->look_at.final_dir, &scene->camera.orientation);
		
}

int	set_key_down(int key, t_scene *scene)
{
	printf("%d\n", key);
	// scene->shapes[scene->shape_idx].highlighted = false;
	if (key == KEY_M && scene->edit_mode == true)
	{
		// if (scene->mouse.toggle == true)
		// {
		// 	scene->mouse.toggle = false;
		// 	mlx_mouse_show();
		// }
		// else if (scene->mouse.toggle == false)
		// {
		// 	scene->mouse.toggle = true;
		// 	mlx_mouse_hide();
		// }
	}
	if (key == KEY_RETURN && scene->edit_mode == true)
	{
		scene->shapes[scene->shape_idx].highlighted = false;
		scene->shapes[scene->count.shape_count].color.r = 0xFF / 255.0;
		scene->shapes[scene->count.shape_count].color.g = 0x10 / 255.0;
		scene->shapes[scene->count.shape_count].color.b = 0xf0 / 255.0;
		scene->shapes[scene->count.shape_count].color.a = 0;
		scene->shapes[scene->count.shape_count].diffuse = 0.9;
		scene->shapes[scene->count.shape_count].highlighted = true;
		scene->shapes[scene->count.shape_count].origin.x = scene->camera.orientation.x * 5 + scene->camera.position.x;
		scene->shapes[scene->count.shape_count].origin.y = scene->camera.orientation.y + scene->camera.position.y;
		scene->shapes[scene->count.shape_count].origin.z = scene->camera.orientation.z * 5 + scene->camera.position.z;
		scene->shapes[scene->count.shape_count].origin.w = 1;
		scene->shapes[scene->count.shape_count].radius = 0.7;
		scene->shapes[scene->count.shape_count].reflectiveness = 0.2;
		scene->shapes[scene->count.shape_count].type = SPHERE;
		scene->shapes[scene->count.shape_count].shininess = 50;
		scene->shapes[scene->count.shape_count].specular = 0.8;
		scene->count.shape_count++;
		scene->shape_idx = scene->count.shape_count - 1;
		calculate_transforms(scene);
		draw_scene(scene);
	}
	if ((key == KEY_1
	|| key == KEY_2
	|| key == KEY_3
	|| key == KEY_4
	|| key == KEY_5
	|| key == KEY_6
	|| key == KEY_7
	|| key == KEY_8
	|| key == KEY_9) && scene->edit_mode == true
	)
	{
		handle_color_change(key, scene);
		calculate_transforms(scene);
		draw_scene(scene);
	}
	if (key == KEY_O && scene->edit_mode == true)
	{
		look_at(scene);
		// camera_init(&scene->camera, scene);
		// calculate_transforms(scene);
		// draw_scene(scene);
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
	if (key == KEY_TAB && scene->edit_mode == true)
	{
		scene->shapes[scene->shape_idx].highlighted = false;
		scene->shape_idx = (scene->shape_idx + 1) % scene->count.shape_count;
		while (scene->shapes[scene->shape_idx].type == PLANE)
		{
			scene->shape_idx++;
		}
		scene->shapes[scene->shape_idx].highlighted = true;
		camera_init(&scene->camera, scene);
		calculate_transforms(scene);
		draw_scene(scene);
	}
	if (key == KEY_R)
	{
		if (scene->reflection_depth == 0 && scene->refraction_depth == 0)
		{
			scene->edit_scale = 0.55;
			scene->reflection_depth = REFLECTION_DEPTH;
		}
		else if (scene->reflection_depth != 0 && scene->refraction_depth == 0)
		{
			scene->edit_scale = 0.35;
			scene->refraction_depth = 5;
		}
		else if (scene->reflection_depth != 0 && scene->refraction_depth != 0)
		{
			scene->edit_scale = 0.75;
			scene->reflection_depth = 0;
			scene->refraction_depth = 0;
		}
		scene->edit_w = 1920 * scene->edit_scale;
		scene->edit_h = 1080 * scene->edit_scale;
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
	if (key == KEY_C && scene->edit_mode == true)
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
			scene->shapes[scene->shape_idx].origin.x += 0.2 *sin(scene->camera.phi)*cos(scene->camera.theta);
			scene->shapes[scene->shape_idx].origin.z += 0.2 *sin(scene->camera.phi)*sin(scene->camera.theta);
			t_vector	offset;
			offset.x = -0.0001 *sin(scene->camera.phi)*cos(scene->camera.theta);
			offset.y = 0;
			offset.z = -0.0001 *sin(scene->camera.phi)*sin(scene->camera.theta);
			offset.w = 0;
			while (scene->collisions &&is_colliding(&scene->shapes[scene->shape_idx], scene, &offset, false) == true)
			{
				scene->shapes[scene->shape_idx].origin.x -= 0.0001 *sin(scene->camera.phi)*cos(scene->camera.theta);
				scene->shapes[scene->shape_idx].origin.z -= 0.0001 *sin(scene->camera.phi)*sin(scene->camera.theta);
			}
		}
		if (scene->keys_held.a == true)
		{
			scene->shapes[scene->shape_idx].origin.x += 0.2 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
			scene->shapes[scene->shape_idx].origin.z += 0.2 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
			t_vector	offset;
			offset.x = -0.0001 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
			offset.y = 0;
			offset.z = -0.0001 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
			offset.w = 0;
			while (scene->collisions &&is_colliding(&scene->shapes[scene->shape_idx], scene, &offset, false) == true)
			{
				scene->shapes[scene->shape_idx].origin.x -= 0.0001 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
				scene->shapes[scene->shape_idx].origin.z -= 0.0001 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
			}
		}
		if (scene->keys_held.s == true)
		{
			scene->shapes[scene->shape_idx].origin.x -= 0.2 *sin(scene->camera.phi)*cos(scene->camera.theta);
			scene->shapes[scene->shape_idx].origin.z -= 0.2 *sin(scene->camera.phi)*sin(scene->camera.theta);
			t_vector	offset;
			offset.x = 0.0001 *sin(scene->camera.phi)*cos(scene->camera.theta);
			offset.y = 0;
			offset.z = 0.0001 *sin(scene->camera.phi)*sin(scene->camera.theta);
			offset.w = 0;
			while (scene->collisions &&is_colliding(&scene->shapes[scene->shape_idx], scene, &offset, false) == true)
			{
				scene->shapes[scene->shape_idx].origin.x += 0.0001 *sin(scene->camera.phi)*cos(scene->camera.theta);
				scene->shapes[scene->shape_idx].origin.z += 0.0001 *sin(scene->camera.phi)*sin(scene->camera.theta);
			}
		}
		if (scene->keys_held.d == true)
		{
			scene->shapes[scene->shape_idx].origin.x -= 0.2 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
			scene->shapes[scene->shape_idx].origin.z -= 0.2 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
			t_vector	offset;
			
			offset.x = 0.0001 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
			offset.y = 0;
			offset.z = 0.0001 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
			offset.w = 0;
			while (scene->collisions && is_colliding(&scene->shapes[scene->shape_idx], scene, &offset, false) == true)
			{
				scene->shapes[scene->shape_idx].origin.x += 0.0001 *sin(M_PI / 2)*cos(scene->camera.theta + M_PI / 2);
				scene->shapes[scene->shape_idx].origin.z += 0.0001 *sin(M_PI / 2)*sin(scene->camera.theta + M_PI / 2);
			}
		}
		if (scene->keys_held.plus == true)
		{
			scene->shapes[scene->shape_idx].radius += 0.04;
			t_vector	offset;
			ft_bzero(&offset, sizeof(t_vector));
			offset.x = 0.04;
			while (scene->collisions &&is_colliding(&scene->shapes[scene->shape_idx], scene, &offset, true) == true)
			{
				scene->shapes[scene->shape_idx].radius -= 0.002;
			}

		}
		if (scene->keys_held.minus == true)
		{
			if (scene->shapes[scene->shape_idx].radius > 0.3)
			{
				scene->shapes[scene->shape_idx].radius -= 0.04;
				// t_vector	offset;
				// ft_bzero(&offset, sizeof(t_vector));
				// while (is_colliding(&scene->shapes[scene->shape_idx], scene, &offset) == true)
				// {
				// 	scene->shapes[scene->shape_idx].radius += 0.002;
				// }
			}
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
		{
			scene->shapes[scene->shape_idx].origin.y += 0.1;
			t_vector	offset;
			ft_bzero(&offset, sizeof(t_vector));
			offset.y = -0.01;
			while (scene->collisions &&is_colliding(&scene->shapes[scene->shape_idx], scene, &offset, false) == true)
			{
				scene->shapes[scene->shape_idx].origin.y -= 0.01;
			}
		}
		if (scene->keys_held.e == true)
		{
			scene->shapes[scene->shape_idx].origin.y -= 0.1;
			t_vector	offset;
			ft_bzero(&offset, sizeof(t_vector));
			offset.y = 0.01;
			while (scene->collisions &&is_colliding(&scene->shapes[scene->shape_idx], scene, &offset, false) == true)
			{
				scene->shapes[scene->shape_idx].origin.y += 0.01;
			}

		}
	}
	if (scene->look_at.trigger == true && scene->edit_mode == true)
	{
		if (scene->look_at.step_num != scene->look_at.step_amount)
		{
			scene->camera.position.x += scene->look_at.pos_diff.x / scene->look_at.step_amount;
			scene->camera.position.y += scene->look_at.pos_diff.y / scene->look_at.step_amount;
			scene->camera.position.z += scene->look_at.pos_diff.z / scene->look_at.step_amount;
		}
		double x_diff = scene->look_at.current_dir.x - scene->look_at.final_dir.x;
		double y_diff = scene->look_at.current_dir.y - scene->look_at.final_dir.y;
		double z_diff = scene->look_at.current_dir.z - scene->look_at.final_dir.z;
		if (fabs(x_diff) > 0.01)
		{
			if (x_diff < 0)
			{
				scene->look_at.current_dir.x += 0.04;
				if (fabs(x_diff) < 0.1)
					scene->look_at.current_dir.x = scene->look_at.final_dir.x;
			}
			if (x_diff > 0)
			{
				scene->look_at.current_dir.x -= 0.04;
				if (fabs(x_diff) < 0.1)
					scene->look_at.current_dir.x = scene->look_at.final_dir.x;
			}
		}
		if (fabs(y_diff) > 0.01)
		{
			if (y_diff < 0)
			{
				scene->look_at.current_dir.y += 0.04;
				if (fabs(y_diff) < 0.1)
					scene->look_at.current_dir.y = scene->look_at.final_dir.y;
			}
			if (y_diff > 0)
			{
				scene->look_at.current_dir.y -= 0.04;
				if (fabs(y_diff) < 0.1)
					scene->look_at.current_dir.y = scene->look_at.final_dir.y;
			}
		}
		if (fabs(z_diff) > 0.01)
		{
			if (z_diff < 0)
			{
				scene->look_at.current_dir.z += 0.04;
				if (fabs(z_diff) < 0.1)
					scene->look_at.current_dir.z = scene->look_at.final_dir.z;
			}
			if (z_diff > 0)
			{
				scene->look_at.current_dir.z -= 0.04;
				if (fabs(z_diff) < 0.1)
					scene->look_at.current_dir.z = scene->look_at.final_dir.z;
			}
		}
		scene->camera.orientation = scene->look_at.current_dir;
		calculate_transforms(scene);
		draw_scene(scene);
		x_diff = scene->look_at.current_dir.x - scene->look_at.final_dir.x;
		y_diff = scene->look_at.current_dir.y - scene->look_at.final_dir.y;
		z_diff = scene->look_at.current_dir.z - scene->look_at.final_dir.z;
		if (scene->look_at.step_num < scene->look_at.step_amount)
			scene->look_at.step_num++;
		if (scene->look_at.step_num == scene->look_at.step_amount && fabs(x_diff) < 0.01 && fabs(y_diff) < 0.01 && fabs(z_diff) < 0.01)
		{
			if (scene->camera.orientation.x > 0)
			{
				scene->camera.theta = atan(scene->camera.orientation.z / scene->camera.orientation.x);
			}
			else if (scene->camera.orientation.x < 0 && scene->camera.orientation.z >= 0)
			{
				scene->camera.theta = atan(scene->camera.orientation.z / scene->camera.orientation.x) + M_PI;
			}
			else if (scene->camera.orientation.x < 0 && scene->camera.orientation.z < 0)
			{
				scene->camera.theta = atan(scene->camera.orientation.z / scene->camera.orientation.x) - M_PI;
			}
			scene->camera.phi = acos(scene->camera.orientation.y);
			scene->look_at.trigger = false;
			scene->look_at.step_num = 0;
		}
	}
	mouse_rotate(scene);
	if (scene->mouse.active == false && scene->edit_mode == true && (scene->keys_held.w
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
