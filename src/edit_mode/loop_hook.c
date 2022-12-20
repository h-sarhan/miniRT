/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 18:50:31 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/20 22:11:24 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vector	*sphere_to_xyz(t_vector *vec, double phi, double theta, double r)
{
	vec->x = r * sin(phi) * cos(theta);
	vec->z = r * sin(phi) * sin(theta);
	vec->y = r * cos(phi);
	vec->w = 0;
	return (vec);
}

void	move_cam(t_scene *scene)
{
	t_vector	vec;

	ft_bzero(&vec, sizeof(t_vector));
	if (scene->keys_held.w == true)
		sphere_to_xyz(&vec, scene->camera.phi, scene->camera.theta, CAM_SPEED);
	if (scene->keys_held.a == true)
		sphere_to_xyz(&vec, M_PI_2, scene->camera.theta - M_PI_2, -CAM_SPEED);
	if (scene->keys_held.s == true)
		sphere_to_xyz(&vec, scene->camera.phi, scene->camera.theta, -CAM_SPEED);
	if (scene->keys_held.d == true)
		sphere_to_xyz(&vec, M_PI_2, scene->camera.theta - M_PI_2, CAM_SPEED);
	if (scene->keys_held.q == true)
		vec.y = 0.35;
	if (scene->keys_held.e == true)
		vec.y = -0.35;
	add_vec(&scene->camera.position, &scene->camera.position, &vec);
}

void	camera_controls(t_scene *scene)
{
	if (scene->keys_held.w || scene->keys_held.a || scene->keys_held.s
		|| scene->keys_held.d || scene->keys_held.q || scene->keys_held.e)
		move_cam(scene);
	if (scene->keys_held.up == true && scene->camera.phi > 0.5)
		scene->camera.phi -= 0.05;
	if (scene->keys_held.down == true && scene->camera.phi < M_PI - 0.5)
		scene->camera.phi += 0.05;
	if (scene->keys_held.left == true)
		scene->camera.theta += 0.10;
	if (scene->keys_held.right == true)
		scene->camera.theta -= 0.10;
	if (scene->keys_held.up || scene->keys_held.left || scene->keys_held.right
		|| scene->keys_held.down)
		sphere_to_xyz(&scene->camera.dir, scene->camera.phi,
			scene->camera.theta, 1);
}

void	move_object_fwd(t_scene *scene, t_shape *shape)
{
	t_vector	offset;
	t_vector	increment;

	ft_bzero(&offset, sizeof(t_vector));
	ft_bzero(&increment, sizeof(t_vector));
	if (scene->keys_held.w)
	{
		sphere_to_xyz(&offset, scene->camera.phi, scene->camera.theta, 0.2);
		sphere_to_xyz(&increment, scene->camera.phi, scene->camera.theta,
			-0.0001);
	}
	if (scene->keys_held.s)
	{
		sphere_to_xyz(&offset, scene->camera.phi, scene->camera.theta, -0.2);
		sphere_to_xyz(&increment, scene->camera.phi, scene->camera.theta,
			0.0001);
	}
	add_vec(&shape->origin, &shape->origin, &offset);
	collide(shape, scene, &offset);
	// while (scene->collisions && is_colliding(shape, scene, &increment, 0))
	// 	add_vec(&shape->origin, &shape->origin, &increment);
}

void	move_object_h(t_scene *scene, t_shape *shape)
{
	t_vector	offset;
	t_vector	increment;

	ft_bzero(&offset, sizeof(t_vector));
	ft_bzero(&increment, sizeof(t_vector));
	if (scene->keys_held.a)
	{
		sphere_to_xyz(&offset, M_PI_2, scene->camera.theta + M_PI_2, 0.2);
		sphere_to_xyz(&increment, M_PI_2, scene->camera.theta + M_PI_2,
			-0.0001);
	}
	if (scene->keys_held.d)
	{
		sphere_to_xyz(&offset, M_PI_2, scene->camera.theta - M_PI_2, 0.2);
		sphere_to_xyz(&increment, M_PI_2, scene->camera.theta - M_PI_2,
			-0.0001);
	}
	add_vec(&shape->origin, &shape->origin, &offset);
	collide(shape, scene, &offset);
	// while (scene->collisions && is_colliding(shape, scene, &increment, 0))
	// 	add_vec(&shape->origin, &shape->origin, &increment);
}

void	move_object_v(t_scene *scene, t_shape *shape)
{
	t_vector	offset;
	t_vector	increment;

	ft_bzero(&offset, sizeof(t_vector));
	ft_bzero(&increment, sizeof(t_vector));
	if (scene->keys_held.q)
	{
		offset.y = 0.2;
		increment.y = -0.0001;
	}
	if (scene->keys_held.e)
	{
		offset.y = -0.2;
		increment.y = 0.0001;
	}
	add_vec(&shape->origin, &shape->origin, &offset);
	collide(shape, scene, &offset);
	// while (scene->collisions && is_colliding(shape, scene, &increment, 0))
	// 	add_vec(&shape->origin, &shape->origin, &increment);
}

void	scale_object(t_scene *scene, t_shape *shape)
{
	t_vector	offset;

	ft_bzero(&offset, sizeof(t_vector));
	if (scene->keys_held.plus == true)
	{
		shape->radius += 0.04;
		offset.x = 0.04;
		collide(shape, scene, &offset);
		// while (scene->collisions && is_colliding(shape, scene, &offset, true))
		// 	shape->radius -= 0.002;
	}
	if (scene->keys_held.minus == true)
	{
		if (shape->radius > 0.3)
			shape->radius -= 0.04;
	}
}

void	transform_object(t_scene *scene)
{
	if (scene->keys_held.w == true || scene->keys_held.s == true)
		move_object_fwd(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.a == true || scene->keys_held.d == true)
		move_object_h(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.q == true || scene->keys_held.e == true)
		move_object_v(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.plus == true || scene->keys_held.minus == true)
		scale_object(scene, &scene->shapes[scene->shape_idx]);
}

void	light_controls(t_scene *scene)
{
	if (scene->keys_held.up == true)
		scene->lights[0].position.y += 0.3;
	if (scene->keys_held.down == true)
		scene->lights[0].position.y -= 0.3;
	if (scene->keys_held.left == true)
		scene->lights[0].position.x -= 0.3;
	if (scene->keys_held.right == true)
		scene->lights[0].position.x += 0.3;
}

void	reset_look_at(t_scene *scene)
{
	if (scene->camera.dir.x > 0)
		scene->camera.theta = atan(scene->camera.dir.z / scene->camera.dir.x);
	else if (scene->camera.dir.x < 0 && scene->camera.dir.z >= 0)
		scene->camera.theta = atan(scene->camera.dir.z / scene->camera.dir.x) \
		+ M_PI;
	else if (scene->camera.dir.x < 0 && scene->camera.dir.z < 0)
		scene->camera.theta = atan(scene->camera.dir.z / scene->camera.dir.x) \
		- M_PI;
	scene->camera.phi = acos(scene->camera.dir.y);
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
	add_vec(&scene->camera.position, &scene->camera.position, &pos_step);
	add_vec(&scene->look_at.current_dir, &scene->look_at.current_dir,
		&dir_step);
	scene->camera.dir = scene->look_at.current_dir;
	calculate_transforms(scene);
	draw_scene(scene);
	scene->look_at.step_num++;
	if (scene->look_at.step_num >= scene->look_at.step_amount)
		reset_look_at(scene);
}

int	key_handler(t_scene *scene)
{
	if (scene->camera_mode == true && scene->edit_mode == true)
		camera_controls(scene);
	else if (scene->edit_mode == true)
	{
		transform_object(scene);
		light_controls(scene);
	}
	if (scene->look_at.trigger == true && scene->edit_mode == true)
		look_at_animation(scene);
	if (scene->look_at.trigger == false && scene->mouse.active == false
		&& scene->edit_mode == true && (scene->keys_held.w || scene->keys_held.a
			|| scene->keys_held.s || scene->keys_held.d || scene->keys_held.up
			|| scene->keys_held.right || scene->keys_held.q
			|| scene->keys_held.e || scene->keys_held.down
			|| scene->keys_held.left || scene->keys_held.plus
			|| scene->keys_held.minus))
	{
		calculate_transforms(scene);
		draw_scene(scene);
	}
	return (0);
}