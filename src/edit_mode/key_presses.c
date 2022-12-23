/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_presses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 19:35:57 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/23 21:29:52 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	handle_color_change(int key, t_scene *scene)
{
	t_color	color;

	if ((key != KEY_1 && key != KEY_2 && key != KEY_3 && key != KEY_4
			&& key != KEY_5 && key != KEY_6) || scene->edit_mode == false)
		return ;
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
	t_shape		*shape;
	t_vector	cam_to_object;
	t_vector	final_pos;

	shape = &scene->shapes[scene->shape_idx];
	sub_vec(&cam_to_object, &shape->origin, &scene->camera.position);
	normalize_vec(&cam_to_object);
	if (shape->type == SPHERE)
	{
		final_pos.x = cam_to_object.x * -8 * shape->radius + shape->origin.x;
		final_pos.z = cam_to_object.z * -8 * shape->radius + shape->origin.z;
		final_pos.y = shape->origin.y;
		final_pos.w = 1;
	}
	if (shape->type == CUBE)
	{
		final_pos.x = cam_to_object.x * -8 * shape->scale_x + shape->origin.x;
		final_pos.z = cam_to_object.z * -8 * shape->scale_z + shape->origin.z;
		final_pos.y = shape->origin.y;
		final_pos.w = 1;
	}
	if (shape->type == CYLINDER)
	{
		final_pos.x = cam_to_object.x * -7 * shape->radius + shape->origin.x;
		final_pos.z = cam_to_object.z * -7 * shape->radius + shape->origin.z;
		final_pos.y = shape->origin.y;
		final_pos.w = 1;
	}
	scene->look_at.final_pos = final_pos;
	sub_vec(&scene->look_at.final_dir, &shape->origin, &final_pos);
	normalize_vec(&scene->look_at.final_dir);
	scene->look_at.current_dir = scene->camera.dir;
	scene->look_at.initial_orientation = scene->camera.dir;
	scene->look_at.step_num = 0;
	sub_vec(&scene->look_at.pos_diff, &scene->look_at.final_pos,
		&scene->camera.position);
	sub_vec(&scene->look_at.dir_diff, &scene->look_at.final_dir,
		&scene->camera.dir);
	scene->look_at.step_amount = (vec_magnitude(&scene->look_at.pos_diff) + \
		vec_magnitude(&scene->look_at.dir_diff)) + 2;
	scene->look_at.trigger = true;
}

void	spawn_shape(t_scene *scene)
{
	scene->shapes[scene->shape_idx].highlighted = false;
	scene->shapes[scene->count.shapes].color.r = 0x00 / 255.0;
	scene->shapes[scene->count.shapes].color.g = 0x00 / 255.0;
	scene->shapes[scene->count.shapes].color.b = 0x00 / 255.0;
	scene->shapes[scene->count.shapes].color.a = 0;
	scene->shapes[scene->count.shapes].diffuse = 0.9;
	scene->shapes[scene->count.shapes].highlighted = true;
	scene->shapes[scene->count.shapes].origin.x = scene->camera.dir.x * 5 + scene->camera.position.x;
	scene->shapes[scene->count.shapes].origin.y = scene->camera.dir.y + scene->camera.position.y;
	scene->shapes[scene->count.shapes].origin.z = scene->camera.dir.z * 5 + scene->camera.position.z;
	scene->shapes[scene->count.shapes].origin.w = 1;
	scene->shapes[scene->count.shapes].radius = 0.7;
	scene->shapes[scene->count.shapes].scale_x = 0.7;
	scene->shapes[scene->count.shapes].scale_y = 0.7;
	scene->shapes[scene->count.shapes].scale_z = 0.7;
	scene->shapes[scene->count.shapes].reflectiveness = 0.2;
	scene->shapes[scene->count.shapes].type = SPHERE;
	scene->shapes[scene->count.shapes].shininess = 50;
	scene->shapes[scene->count.shapes].specular = 0.8;
	scene->shapes[scene->count.shapes].id = scene->count.shapes;
	scene->shapes[scene->count.shapes].ior = 1;
	identity_matrix(&scene->shapes[scene->count.shapes].added_rots);
	scene->count.shapes++;
	scene->shape_idx = scene->count.shapes - 1;
}

void	deploy_flashbang(t_scene *scene)
{
	int	y;
	int	x;

	y = 0;
	while (y < scene->display_h)
	{
		x = 0;
		while (x < scene->display_w)
		{
			*(unsigned int *)(scene->mlx->display_addr + (y * \
			scene->display_w + x) * scene->mlx->bytes_per_pixel) = 0xffffff;
			x++;
		}
		y++;
	}
	(void)! system("afplay sound.mp3");
	mlx_put_image_to_window(scene->mlx->mlx, scene->mlx->mlx_win,
		scene->mlx->display_img, 0, 0);
}

void	toggle_reflections(t_scene *scene)
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
}

void	toggle_keys_held(int key, t_scene *scene, bool on_off)
{
	if (key == KEY_W)
		scene->keys_held.w = on_off;
	if (key == KEY_A)
		scene->keys_held.a = on_off;
	if (key == KEY_S)
		scene->keys_held.s = on_off;
	if (key == KEY_D)
		scene->keys_held.d = on_off;
	if (key == KEY_UP)
		scene->keys_held.up = on_off;
	if (key == KEY_DOWN)
		scene->keys_held.down = on_off;
	if (key == KEY_LEFT)
		scene->keys_held.left = on_off;
	if (key == KEY_RIGHT)
		scene->keys_held.right = on_off;
	if (key == KEY_Q)
		scene->keys_held.q = on_off;
	if (key == KEY_E)
		scene->keys_held.e = on_off;
	if (key == KEY_PLUS)
		scene->keys_held.plus = on_off;
	if (key == KEY_MINUS)
		scene->keys_held.minus = on_off;
	if (key == KEY_SHIFT)
		scene->keys_held.shift = on_off;
	if (key == KEY_X)
		scene->keys_held.x = on_off;
	if (key == KEY_Y)
		scene->keys_held.y = on_off;
	if (key == KEY_Z)
		scene->keys_held.z = on_off;
}

void	select_shape(int key, t_scene *scene)
{
	if (key == KEY_TAB && scene->edit_mode == true)
	{
		scene->shapes[scene->shape_idx].highlighted = false;
		scene->shape_idx = (scene->shape_idx + 1) % scene->count.shapes;
		while (scene->shapes[scene->shape_idx].type == PLANE)
			scene->shape_idx = (scene->shape_idx + 1) % scene->count.shapes;
		scene->shapes[scene->shape_idx].highlighted = true;
	}
}

bool	is_toggle_key(int key, t_scene *scene)
{
	return (key == KEY_SPACE || key == KEY_R
		|| (scene->edit_mode == true
			&& (key == KEY_RETURN || key == KEY_1 || key == KEY_2
				|| key == KEY_3 || key == KEY_4 || key == KEY_5 || key == KEY_6
				|| key == KEY_TAB || key == KEY_C || key == KEY_T)));
}

void	toggle_edit_mode(int key, t_scene *scene)
{
	if (key == KEY_SPACE)
	{
		scene->edit_mode = !scene->edit_mode;
		// scene->reflection_depth = REFLECTION_DEPTH;
		// if (scene->edit_mode == false)
		// 	scene->reflection_depth = REFLECTION_DEPTH + 1;
	}
}

void	toggle_shape(t_scene *scene)
{
	t_shape *shape;

	shape = &scene->shapes[scene->shape_idx];
	if (shape->radius < 0.2)
		shape->radius = 1;
	if (shape->height < 0.2)
		shape->height = 1;
	if (shape->scale_x < 0.2)
		shape->scale_x = 1;
	if (shape->scale_y < 0.2)
		shape->scale_y = 1;
	if (shape->scale_z < 0.2)
		shape->scale_z = 1;
	if (shape->type == SPHERE)
	{
		shape->scale_x = shape->radius;
		shape->scale_y = shape->radius;
		shape->scale_z = shape->radius;
		shape->type = CUBE;
	}
	else if (shape->type == CUBE)
	{
		shape->height = shape->scale_y * 2;
		shape->scale_y = 1;
		shape->radius = shape->scale_x;
		shape->scale_x = shape->radius;
		shape->scale_z = shape->radius;
		shape->type = CYLINDER;
	}
	else if (shape->type == CYLINDER)
	{
		shape->scale_x = shape->radius;
		shape->scale_y = shape->radius;
		shape->scale_z = shape->radius;
		shape->type = SPHERE;
	}
}

int	set_key_down(int key, t_scene *scene)
{
	printf("%d\n", key);
	if (key == KEY_M && scene->edit_mode == true)
	{
	}
	if (key == KEY_RETURN && scene->edit_mode == true)
		spawn_shape(scene);
	if (key == KEY_T && scene->edit_mode == true)
		toggle_shape(scene);
	handle_color_change(key, scene);
	if (key == KEY_O && scene->edit_mode == true && !scene->look_at.trigger)
		look_at(scene);
	toggle_edit_mode(key, scene);
	select_shape(key, scene);
	if (key == KEY_R)
		toggle_reflections(scene);
	if (key == KEY_ESC)
		deploy_flashbang(scene);
	if (key == KEY_C && scene->edit_mode == true)
		scene->camera_mode = !scene->camera_mode;
	toggle_keys_held(key, scene, true);
	if (key == KEY_SPACE || key == KEY_R)
		camera_init(&scene->camera, scene);
	if (!is_toggle_key(key, scene))
		return (0);
	calculate_transforms(scene);
	draw_scene(scene);
	return (0);
}

int	set_key_up(int key, t_scene *scene)
{
	toggle_keys_held(key, scene, false);
	return (0);
}
