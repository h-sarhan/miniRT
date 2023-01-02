/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_presses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 19:35:57 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/02 21:13:34 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	handle_color_change(int key, t_scene *scene)
{
	t_color	color;

	if ((key != KEY_1 && key != KEY_2 && key != KEY_3 && key != KEY_4
			&& key != KEY_5 && key != KEY_6)
		|| scene->settings.edit_mode == false)
		return ;
	color = scene->shapes[scene->shape_idx].props.color;
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
	scene->shapes[scene->shape_idx].props.color = color;
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
		final_pos.x = cam_to_object.x * -8 * shape->props.radius + \
			shape->origin.x;
		final_pos.z = cam_to_object.z * -8 * shape->props.radius + \
			shape->origin.z;
		final_pos.y = shape->origin.y;
		final_pos.w = 1;
	}
	if (shape->type == CUBE)
	{
		final_pos.x = cam_to_object.x * -8 * shape->props.scale.x + \
			shape->origin.x;
		final_pos.z = cam_to_object.z * -8 * shape->props.scale.z + \
			shape->origin.z;
		final_pos.y = shape->origin.y;
		final_pos.w = 1;
	}
	if (shape->type == CYLINDER)
	{
		final_pos.x = cam_to_object.x * -7 * shape->props.radius + \
			shape->origin.x;
		final_pos.z = cam_to_object.z * -7 * shape->props.radius + \
			shape->origin.z;
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
	scene->shapes[scene->shape_idx].props.highlighted = false;
	scene->shapes[scene->count.shapes].props.color.r = 0x00 / 255.0;
	scene->shapes[scene->count.shapes].props.color.g = 0x00 / 255.0;
	scene->shapes[scene->count.shapes].props.color.b = 0x00 / 255.0;
	scene->shapes[scene->count.shapes].props.color.a = 0;
	scene->shapes[scene->count.shapes].props.diffuse = 0.9;
	scene->shapes[scene->count.shapes].props.highlighted = true;
	scene->shapes[scene->count.shapes].origin.x = scene->camera.dir.x * 5 \
		+ scene->camera.position.x;
	scene->shapes[scene->count.shapes].origin.y = scene->camera.dir.y + \
		scene->camera.position.y;
	scene->shapes[scene->count.shapes].origin.z = scene->camera.dir.z * 5 + \
		scene->camera.position.z;
	scene->shapes[scene->count.shapes].origin.w = 1;
	scene->shapes[scene->count.shapes].props.radius = 0.7;
	scene->shapes[scene->count.shapes].props.radius_squared = 0.7 * 0.7;
	scene->shapes[scene->count.shapes].props.scale.x = 0.7;
	scene->shapes[scene->count.shapes].props.scale.y = 0.7;
	scene->shapes[scene->count.shapes].props.scale.z = 0.7;
	scene->shapes[scene->count.shapes].props.reflectiveness = 0.2;
	scene->shapes[scene->count.shapes].type = SPHERE;
	scene->shapes[scene->count.shapes].props.shininess = 50;
	scene->shapes[scene->count.shapes].props.specular = 0.8;
	scene->shapes[scene->count.shapes].id = scene->count.shapes;
	ft_bzero(&scene->shapes[scene->count.shapes].orientation, sizeof(t_vector));
	scene->shapes[scene->count.shapes].orientation.y = 1;
	identity_matrix(&scene->shapes[scene->count.shapes].added_rots);
	scene->count.shapes++;
	scene->shape_idx = scene->count.shapes - 1;
}

void	deploy_flashbang(t_scene *scene)
{
	int	y;
	int	x;

	y = 0;
	while (y < scene->settings.disp_h)
	{
		x = 0;
		while (x < scene->settings.disp_w)
		{
			*(unsigned int *)(scene->disp->disp_addr + (y * \
			scene->settings.disp_w + x) * scene->disp->bpp) = 0xffffff;
			x++;
		}
		y++;
	}
	(void)! system("afplay assets/sound.mp3");
	mlx_put_image_to_window(scene->disp->mlx, scene->disp->win,
		scene->disp->display_img, 0, 0);
}

void	toggle_reflections(t_scene *scene)
{
	if (scene->settings.reflection_depth == 0)
	{
		scene->settings.reflection_depth = REFLECTION_DEPTH;
	}
	else if (scene->settings.reflection_depth != 0)
	{
		scene->settings.reflection_depth = 0;
	}
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
	if (key == KEY_TAB && scene->settings.edit_mode == true)
	{
		scene->shapes[scene->shape_idx].props.highlighted = false;
		scene->shape_idx = (scene->shape_idx + 1) % scene->count.shapes;
		while (scene->shapes[scene->shape_idx].type == PLANE)
			scene->shape_idx = (scene->shape_idx + 1) % scene->count.shapes;
		scene->shapes[scene->shape_idx].props.highlighted = true;
	}
}

bool	is_toggle_key(int key, t_scene *scene)
{
	return (key == KEY_SPACE || key == KEY_R
		|| (scene->settings.edit_mode == true
			&& (key == KEY_RETURN || key == KEY_1 || key == KEY_2
				|| key == KEY_3 || key == KEY_4 || key == KEY_5 || key == KEY_6
				|| key == KEY_TAB || key == KEY_C || key == KEY_T)));
}

void	toggle_edit_mode(int key, t_scene *scene)
{
	if (key == KEY_SPACE)
	{
		scene->settings.edit_mode = !scene->settings.edit_mode;
	}
}

void	toggle_shape(t_scene *scene)
{
	t_shape	*shape;

	shape = &scene->shapes[scene->shape_idx];
	if (shape->props.radius < 0.2)
		shape->props.radius = 1;
	if (shape->props.height < 0.2)
		shape->props.height = 1;
	if (shape->props.scale.x < 0.2)
		shape->props.scale.x = 1;
	if (shape->props.scale.y < 0.2)
		shape->props.scale.y = 1;
	if (shape->props.scale.z < 0.2)
		shape->props.scale.z = 1;
	if (shape->type == SPHERE)
	{
		shape->props.scale.x = shape->props.radius;
		shape->props.scale.y = shape->props.radius;
		shape->props.scale.z = shape->props.radius;
		shape->type = CUBE;
	}
	else if (shape->type == CUBE)
	{
		shape->props.height = shape->props.scale.y * 2;
		shape->props.scale.y = 1;
		shape->props.radius = shape->props.scale.x;
		shape->props.scale.x = shape->props.radius;
		shape->props.scale.z = shape->props.radius;
		shape->type = CYLINDER;
	}
	else if (shape->type == CYLINDER)
	{
		shape->props.scale.x = shape->props.radius;
		shape->props.scale.y = shape->props.radius;
		shape->props.scale.z = shape->props.radius;
		shape->type = SPHERE;
	}
	shape->props.radius_squared = shape->props.radius * shape->props.radius;
}

int	close_window(t_scene *scene)
{
	printf("QUITTING PROGRAM!\n");
	free_scene(scene);
	mlx_destroy_image(scene->disp->mlx, scene->disp->display_img);
	mlx_destroy_image(scene->disp->mlx, scene->disp->render_img);
	mlx_destroy_image(scene->disp->mlx, scene->disp->edit_img);
	mlx_destroy_window(scene->disp->mlx, scene->disp->win);
	exit(EXIT_SUCCESS);
	return (0);
}

int	key_press(int key, t_scene *scene)
{
	printf("key = %d\n", key);
	if (key == KEY_M && scene->settings.edit_mode == true)
	{
	}
	if (key == KEY_RETURN && scene->settings.edit_mode == true)
		spawn_shape(scene);
	if (key == KEY_T && scene->settings.edit_mode == true)
		toggle_shape(scene);
	handle_color_change(key, scene);
	if (key == KEY_O && scene->settings.edit_mode == true \
		&& !scene->look_at.trigger)
		look_at(scene);
	toggle_edit_mode(key, scene);
	select_shape(key, scene);
	if (key == KEY_R)
		toggle_reflections(scene);
	if (key == KEY_ESC)
	{
		close_window(scene);
		return (0);
	}
	if (key == KEY_C && scene->settings.edit_mode == true)
		scene->settings.camera_mode = !scene->settings.camera_mode;
	toggle_keys_held(key, scene, true);
	if (key == KEY_SPACE || key == KEY_R)
		camera_init(&scene->camera, scene);
	if (!is_toggle_key(key, scene))
		return (0);
	calculate_transforms(scene);
	draw_scene(scene);
	return (0);
}

int	key_release(int key, t_scene *scene)
{
	toggle_keys_held(key, scene, false);
	return (0);
}
