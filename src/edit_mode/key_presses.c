/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_presses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 19:35:57 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/20 07:44:51 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	handle_color_change(int key, t_scene *scene, t_color *color)
{
	if ((key != KEY_1 && key != KEY_2 && key != KEY_3 && key != KEY_4
			&& key != KEY_5 && key != KEY_6)
		|| scene->settings.edit_mode == false)
		return ;
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
}

int	close_window(t_scene *scene)
{
	printf("QUITTING PROGRAM!\n");
	free_scene(scene);
	exit(EXIT_SUCCESS);
	return (0);
}

void	rest_of_key_presses(int key, t_scene *scene)
{
	toggle_edit_mode(key, scene);
	select_shape(key, scene);
	if (key == KEY_R)
		toggle_reflections(scene);
	if (key == KEY_L)
		scene->settings.light_mode = !scene->settings.light_mode;
	if (key == KEY_TAB && scene->settings.edit_mode
		&& scene->settings.light_mode)
		scene->light_idx = (scene->light_idx + 1) % scene->count.lights;
	if (key == KEY_H)
		scene->help = !scene->help;
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
}

int	key_press(int key, t_scene *scene)
{
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
		handle_color_change(key, scene,
			&scene->shapes[scene->shape_idx].props.color);
	rest_of_key_presses(key, scene);
	if (key == KEY_ESC)
		return (close_window(scene));
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
