/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_presses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 19:35:57 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/11 20:20:44 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	handle_color_change(int key, t_scene *scene, t_color *color)
{
	// t_color	color;

	if ((key != KEY_1 && key != KEY_2 && key != KEY_3 && key != KEY_4
			&& key != KEY_5 && key != KEY_6)
		|| scene->settings.edit_mode == false)
		return ;
	// color = scene->shapes[scene->shape_idx].props.color;
	if (key == KEY_1 && (color->r + 10 / 255.0) < 1)
		color->r += 10 / 255.0;
	if (key == KEY_2 && (color->r - 10 / 255.0) > 0)
		color->r -= 10 / 255.0;
	if (key == KEY_3 && (color->g + 10 / 255.0) < 1)
		color->g += 10 / 255.0;
	if (key == KEY_4 && (color->g - 10 / 255.0) > 0)
		color->g -= 10 / 255.0;
	if (key == KEY_5 && (color->b + 10 / 255.0) < 1)
		color->b += 10 / 255.0;
	if (key == KEY_6 && (color->b - 10 / 255.0) > 0)
		color->b -= 10 / 255.0;
	// scene->shapes[scene->shape_idx].props.color = color;
}

void	sphere_lookat_pos(t_vector *cam_to_object, t_look_at *look_at,
		t_shape *shape)
{
	t_vector	*final_pos;

	final_pos = &look_at->final_pos;
	if (cam_to_object->x < 0)
		final_pos->x = -5 * shape->props.radius + shape->origin.x;
	else
		final_pos->x = 5 * shape->props.radius + shape->origin.x;
	if (cam_to_object->z < 0)
		final_pos->z = -5 * shape->props.radius + shape->origin.z;
	else
		final_pos->z = 5 * shape->props.radius + shape->origin.z;
	final_pos->y = shape->origin.y;
	final_pos->w = 1;
}

void	cube_lookat_pos(t_vector *cam_to_object, t_look_at *look_at,
		t_shape *shape)
{
	t_vector	*final_pos;

	final_pos = &look_at->final_pos;
	if (cam_to_object->x < 0)
		final_pos->x = -5 * shape->props.scale.x + shape->origin.x;
	else
		final_pos->x = 5 * shape->props.scale.x + shape->origin.x;
	if (cam_to_object->z < 0)
		final_pos->z = -5 * shape->props.scale.z + shape->origin.z;
	else
		final_pos->z = 5 * shape->props.scale.z + shape->origin.z;
	final_pos->y = shape->origin.y;
	final_pos->w = 1;
}

void	cylinder_cone_lookat_pos(t_vector *cam_to_object, t_look_at *look_at,
		t_shape *shape)
{
	t_vector	*final_pos;

	final_pos = &look_at->final_pos;
	if (cam_to_object->x < 0)
		final_pos->x = -2 * max(shape->props.height, shape->props.radius) \
		+ shape->origin.x;
	else
		final_pos->x = 2 * max(shape->props.height, shape->props.radius) \
		+ shape->origin.x;
	if (cam_to_object->z < 0)
		final_pos->z = -2 * max(shape->props.height, shape->props.radius) \
		+ shape->origin.z;
	else
		final_pos->z = 2 * max(shape->props.height, shape->props.radius) \
			+ shape->origin.z;
	final_pos->y = shape->origin.y;
	final_pos->w = 1;
}

void	look_at(t_scene *scene, t_shape *shape)
{
	t_vector	cam_to_object;

	sub_vec(&cam_to_object, &shape->origin, &scene->cam.position);
	negate_vec(&cam_to_object, &cam_to_object);
	if (vec_magnitude(&cam_to_object) < 2)
		return ;
	if (shape->type == SPHERE)
		sphere_lookat_pos(&cam_to_object, &scene->look_at, shape);
	if (shape->type == CUBE)
		cube_lookat_pos(&cam_to_object, &scene->look_at, shape);
	if (shape->type == CYLINDER || shape->type == CONE)
		cylinder_cone_lookat_pos(&cam_to_object, &scene->look_at, shape);
	sub_vec(&scene->look_at.final_dir, &shape->origin,
		&scene->look_at.final_pos);
	normalize_vec(&scene->look_at.final_dir);
	scene->look_at.current_dir = scene->cam.dir;
	scene->look_at.initial_orientation = scene->cam.dir;
	scene->look_at.step_num = 0;
	sub_vec(&scene->look_at.pos_diff, &scene->look_at.final_pos,
		&scene->cam.position);
	sub_vec(&scene->look_at.dir_diff, &scene->look_at.final_dir,
		&scene->cam.dir);
	scene->look_at.step_amount = (vec_magnitude(&scene->look_at.pos_diff) + \
		vec_magnitude(&scene->look_at.dir_diff)) + 2;
	scene->look_at.trigger = true;
}

void	default_shape_props(t_props *props)
{
	ft_bzero(&props->color, sizeof(t_color));
	props->diffuse = 0.9;
	props->highlighted = true;
	props->radius = 0.7;
	props->radius_squared = 0.7 * 0.7;
	props->scale.x = 0.7;
	props->scale.y = 0.7;
	props->scale.z = 0.7;
	props->reflectiveness = 0.2;
	props->shininess = 50;
	props->specular = 0.8;
}

void	spawn_shape(t_scene *scene)
{
	scene->shapes[scene->shape_idx].props.highlighted = false;
	scene->shapes[scene->count.shapes].origin.x = scene->cam.dir.x * 5 \
		+ scene->cam.position.x;
	scene->shapes[scene->count.shapes].origin.y = scene->cam.dir.y + \
		scene->cam.position.y;
	scene->shapes[scene->count.shapes].origin.z = scene->cam.dir.z * 5 + \
		scene->cam.position.z;
	scene->shapes[scene->count.shapes].origin.w = 1;
	default_shape_props(&scene->shapes[scene->count.shapes].props);
	scene->shapes[scene->count.shapes].type = SPHERE;
	scene->shapes[scene->count.shapes].id = scene->count.shapes;
	ft_bzero(&scene->shapes[scene->count.shapes].orientation, sizeof(t_vector));
	scene->shapes[scene->count.shapes].orientation.y = 1;
	identity_matrix(&scene->shapes[scene->count.shapes].added_rots);
	scene->count.shapes++;
	scene->shape_idx = scene->count.shapes - 1;
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
void	toggle_keys_held2(int key, t_scene *scene, bool on_off);

void	toggle_keys_held(int key, t_scene *scene, bool on_off)
{
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
	if (key == KEY_O)
		scene->keys_held.o = on_off;
	toggle_keys_held2(key, scene, on_off);
}

void	toggle_keys_held2(int key, t_scene *scene, bool on_off)
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
}

void	select_shape(int key, t_scene *scene)
{
	if (key == KEY_TAB && scene->settings.edit_mode == true && scene->settings.light_mode == false)
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
				|| key == KEY_TAB || key == KEY_C || key == KEY_T
				|| key == KEY_P)));
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
	shape->props.radius = max(shape->props.radius, 0.5);
	shape->props.radius_squared = shape->props.radius * shape->props.radius;
	shape->props.height = max(shape->props.height, 0.5);
	shape->props.scale.x = shape->props.radius;
	shape->props.scale.y = shape->props.radius;
	shape->props.scale.z = shape->props.radius;
	ft_bzero(&shape->orientation, sizeof(t_vector));
	shape->orientation.y = 1;
	identity_matrix(&shape->added_rots);
	if (shape->type == SPHERE)
		shape->type = CUBE;
	else if (shape->type == CUBE)
	{
		shape->props.height = 1;
		shape->props.scale.y = 1;
		shape->props.radius = shape->props.scale.x;
		shape->type = CYLINDER;
	}
	else if (shape->type == CYLINDER)
	{
		shape->props.height = shape->props.scale.y * 2;
		shape->props.scale.y = 1;
		shape->props.radius = shape->props.scale.x;
		shape->type = CONE;
	}
	else if (shape->type == CONE)
		shape->type = SPHERE;
}

int	close_window(t_scene *scene)
{
	printf("QUITTING PROGRAM!\n");
	mlx_destroy_image(scene->disp->mlx, scene->disp->display_img);
	mlx_destroy_image(scene->disp->mlx, scene->disp->render_img);
	mlx_destroy_image(scene->disp->mlx, scene->disp->edit_img);
	mlx_destroy_window(scene->disp->mlx, scene->disp->win);
	free_scene(scene);
	exit(EXIT_SUCCESS);
	return (0);
}

int	key_press(int key, t_scene *scene)
{
	printf("key = %d\n", key);
	if (key == KEY_J)
	{
		scene->settings.supersampling = !scene->settings.supersampling;
		calculate_transforms(scene);
		draw_scene(scene);
	}
	if (key == KEY_RETURN && scene->settings.edit_mode == true)
		spawn_shape(scene);
	if (key == KEY_T && scene->settings.edit_mode == true)
		toggle_shape(scene);
	if (scene->settings.light_mode == true)
		handle_color_change(key, scene, &scene->lights[scene->light_idx].color);
	else
		handle_color_change(key, scene, &scene->shapes[scene->shape_idx].props.color);
	toggle_edit_mode(key, scene);
	select_shape(key, scene);
	if (key == KEY_R)
		toggle_reflections(scene);
	if (key == KEY_L)
	{
		scene->settings.light_mode = !scene->settings.light_mode;
	}
	if (key == KEY_TAB && scene->settings.edit_mode == true && scene->settings.light_mode == true)
	{
		scene->light_idx = (scene->light_idx + 1) % scene->count.lights;
	}
	if (key == KEY_ESC)
	{
		close_window(scene);
		return (0);
	}
	if (key == KEY_H)
	{
		// toggle_help_menu(scene);
	}
	if (key == KEY_P)
	{
		scene->shapes[scene->shape_idx].props.pattern_type += 1;
		scene->shapes[scene->shape_idx].props.pattern_type %= 5;
	}
	if (key == KEY_C && scene->settings.edit_mode == true)
		scene->settings.camera_mode = !scene->settings.camera_mode;
	toggle_keys_held(key, scene, true);
	if (key == KEY_SPACE || key == KEY_R)
		camera_init(&scene->cam, scene);
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
