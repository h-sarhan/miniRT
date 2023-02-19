/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_controls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 22:24:56 by mkhan             #+#    #+#             */
/*   Updated: 2023/02/19 22:25:46 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	rest_of_light_controls(t_scene *scene, t_light *light)
{
	if (scene->keys_held.shift == false && scene->keys_held.plus == true)
		light->intensity = min(light->intensity + 0.05, 5);
	if (scene->keys_held.shift == false && scene->keys_held.minus == true)
		light->intensity = max(light->intensity - 0.05, 0);
	if (scene->keys_held.up == true || scene->keys_held.down == true)
		rotate_x(scene, &scene->lights[scene->light_idx].added_rots,
			-DEG_TO_RAD * 2);
	if (scene->keys_held.left == true || scene->keys_held.right == true)
		rotate_y(scene, &scene->lights[scene->light_idx].added_rots,
			-DEG_TO_RAD * 2);
	if (scene->keys_held.shift == true && scene->keys_held.plus == true)
		scene->lights[scene->light_idx].theta += 0.02;
	if (scene->keys_held.shift == true && scene->keys_held.minus == true)
	{
		if (scene->lights[scene->light_idx].theta > 0.1)
			scene->lights[scene->light_idx].theta -= 0.02;
	}
	normalize_vec(&scene->lights[scene->light_idx].direction);
}

void	light_controls(t_scene *scene)
{
	t_vector	offset;
	t_light		*light;

	light = &scene->lights[scene->light_idx];
	ft_bzero(&offset, sizeof(t_vector));
	if (scene->keys_held.w)
		sphere_to_xyz(&offset, M_PI / 2, scene->cam.theta, 0.2);
	if (scene->keys_held.s)
		sphere_to_xyz(&offset, M_PI / 2, scene->cam.theta, -0.2);
	if (scene->keys_held.w || scene->keys_held.s)
		add_vec(&light->position, &light->position, &offset);
	if (scene->keys_held.q == true)
		light->position.y += 0.3;
	if (scene->keys_held.e == true)
		light->position.y -= 0.3;
	if (scene->keys_held.a)
		sphere_to_xyz(&offset, M_PI_2, scene->cam.theta + M_PI_2, 0.2);
	if (scene->keys_held.d)
		sphere_to_xyz(&offset, M_PI_2, scene->cam.theta - M_PI_2, 0.2);
	if (scene->keys_held.a || scene->keys_held.d)
		add_vec(&light->position, &light->position, &offset);
	rest_of_light_controls(scene, light);
}

void	reset_look_at(t_scene *scene)
{
	if (scene->cam.dir.x > 0)
		scene->cam.theta = atan(scene->cam.dir.z / scene->cam.dir.x);
	else if (scene->cam.dir.x < 0 && scene->cam.dir.z >= 0)
		scene->cam.theta = atan(scene->cam.dir.z / scene->cam.dir.x) + M_PI;
	else if (scene->cam.dir.x < 0 && scene->cam.dir.z < 0)
		scene->cam.theta = atan(scene->cam.dir.z / scene->cam.dir.x) - M_PI;
	scene->cam.phi = acos(scene->cam.dir.y);
	scene->look_at.trigger = false;
	scene->look_at.step_num = 0;
}

void	look_at_animation(t_scene *scene)
{
	t_vector	pos_step;
	t_vector	dir_step;

	scale_vec(&pos_step, &scene->look_at.pos_diff, \
		1.0 / scene->look_at.step_amount);
	scale_vec(&dir_step, &scene->look_at.dir_diff, \
		1.0 / scene->look_at.step_amount);
	add_vec(&scene->cam.position, &scene->cam.position, &pos_step);
	add_vec(&scene->look_at.current_dir, &scene->look_at.current_dir,
		&dir_step);
	scene->cam.dir = scene->look_at.current_dir;
	calculate_transforms(scene);
	draw_scene(scene);
	scene->look_at.step_num++;
	if (scene->look_at.step_num >= scene->look_at.step_amount)
		reset_look_at(scene);
}

bool	is_loop_hook_key(t_scene *scene)
{
	return (scene->keys_held.w || scene->keys_held.a || scene->keys_held.s
		|| scene->keys_held.d || scene->keys_held.up
		|| scene->keys_held.right || scene->keys_held.q
		|| scene->keys_held.e || scene->keys_held.down
		|| scene->keys_held.left || scene->keys_held.plus
		|| scene->keys_held.minus || scene->keys_held.x
		|| scene->keys_held.y || scene->keys_held.z);
}
