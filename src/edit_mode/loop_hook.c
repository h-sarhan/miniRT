/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 18:50:31 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/31 15:53:27 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	handle_color_change(int key, t_scene *scene, t_color *color);

t_vector	*sphere_to_xyz(t_vector *vec, float phi, float theta, float r)
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
		sphere_to_xyz(&vec, scene->cam.phi, scene->cam.theta, CAM_SPEED);
	if (scene->keys_held.a == true)
		sphere_to_xyz(&vec, M_PI_2, scene->cam.theta - M_PI_2, -CAM_SPEED);
	if (scene->keys_held.s == true)
		sphere_to_xyz(&vec, scene->cam.phi, scene->cam.theta, -CAM_SPEED);
	if (scene->keys_held.d == true)
		sphere_to_xyz(&vec, M_PI_2, scene->cam.theta - M_PI_2, CAM_SPEED);
	if (scene->keys_held.q == true)
		vec.y = 0.35;
	if (scene->keys_held.e == true)
		vec.y = -0.35;
	if (scene->keys_held.w == true || scene->keys_held.a == true
		|| scene->keys_held.s == true || scene->keys_held.d == true
		|| scene->keys_held.q == true || scene->keys_held.e == true)
		add_vec(&scene->cam.position, &scene->cam.position, &vec);
}

void	camera_controls(t_scene *scene)
{
	if (scene->keys_held.w || scene->keys_held.a || scene->keys_held.s
		|| scene->keys_held.d || scene->keys_held.q || scene->keys_held.e)
		move_cam(scene);
	if (scene->keys_held.up == true && scene->cam.phi > 0.2)
		scene->cam.phi -= 0.05;
	if (scene->keys_held.down == true && scene->cam.phi < M_PI - 0.2)
		scene->cam.phi += 0.05;
	if (scene->keys_held.left == true)
		scene->cam.theta += 0.10;
	if (scene->keys_held.right == true)
		scene->cam.theta -= 0.10;
	if (scene->keys_held.up || scene->keys_held.left || scene->keys_held.right
		|| scene->keys_held.down)
		sphere_to_xyz(&scene->cam.dir, scene->cam.phi,
			scene->cam.theta, 1);
}

void	move_object_fwd(t_scene *scene, t_shape *shape)
{
	t_vector	offset;

	ft_bzero(&offset, sizeof(t_vector));
	if (scene->keys_held.w)
	{
		sphere_to_xyz(&offset, M_PI / 2, scene->cam.theta, 0.2);
	}
	if (scene->keys_held.s)
	{
		sphere_to_xyz(&offset, M_PI / 2, scene->cam.theta, -0.2);
	}
	add_vec(&shape->origin, &shape->origin, &offset);
}

void	move_object_h(t_scene *scene, t_shape *shape)
{
	t_vector	offset;

	ft_bzero(&offset, sizeof(t_vector));
	if (scene->keys_held.a)
	{
		sphere_to_xyz(&offset, M_PI_2, scene->cam.theta + M_PI_2, 0.2);
	}
	if (scene->keys_held.d)
	{
		sphere_to_xyz(&offset, M_PI_2, scene->cam.theta - M_PI_2, 0.2);
	}
	add_vec(&shape->origin, &shape->origin, &offset);
}

void	move_object_v(t_scene *scene, t_shape *shape)
{
	t_vector	offset;

	ft_bzero(&offset, sizeof(t_vector));
	if (scene->keys_held.q)
	{
		offset.y = 0.2;
	}
	if (scene->keys_held.e)
	{
		offset.y = -0.2;
	}
	add_vec(&shape->origin, &shape->origin, &offset);
}

void	scale_object(t_scene *scene, t_shape *shape)
{
	float	amount;

	amount = 0.04;
	if (scene->keys_held.minus == true)
		amount *= -1;
	if (shape->type == CUBE)
	{
		if (shape->props.scale.x > 0.3 || !scene->keys_held.minus)
			shape->props.scale.x += amount;
		if (shape->props.scale.y > 0.3 || !scene->keys_held.minus)
			shape->props.scale.y += amount;
		if (shape->props.scale.z > 0.3 || !scene->keys_held.minus)
			shape->props.scale.z += amount;
	}
	else
	{
		if (shape->props.radius > 0.3 || !scene->keys_held.minus)
			shape->props.radius += amount;
		shape->props.scale.x = shape->props.radius;
		shape->props.scale.y = shape->props.radius;
		if (shape->type == CYLINDER || shape->type == CONE)
			shape->props.scale.y = 1;
		shape->props.scale.z = shape->props.radius;
	}
	shape->props.radius_squared = shape->props.radius * shape->props.radius;
}

void	change_height(t_scene *scene, t_shape *shape)
{
	if (scene->keys_held.plus == true)
	{
		shape->props.height += 0.04;
	}
	if (scene->keys_held.minus == true)
	{
		if (shape->props.height > 0.2)
			shape->props.height -= 0.04;
	}
}

void	rotate_x(t_scene *scene, t_mat4 *rot_mat, float deg)
{
	t_mat4		rot;
	t_vector	ax;
	t_vector	up;
	t_mat4		mat_copy;

	up.x = 0;
	up.y = 1;
	up.z = 0;
	up.w = 0;
	cross_product(&ax, &up, &scene->cam.dir);
	normalize_vec(&ax);
	if (scene->keys_held.down == true)
		axis_angle(&rot, &ax, -deg);
	else
		axis_angle(&rot, &ax, deg);
	ft_memcpy(&mat_copy, rot_mat, sizeof(t_mat4));
	mat_multiply(rot_mat, &rot, &mat_copy);
}

void	rotate_y(t_scene *scene, t_mat4 *rot_mat, float deg)
{
	t_mat4	rot;
	t_mat4	mat_copy;

	if (scene->keys_held.left == true)
		rotation_matrix_y(&rot, deg);
	else
		rotation_matrix_y(&rot, -deg);
	ft_memcpy(&mat_copy, rot_mat, sizeof(t_mat4));
	mat_multiply(rot_mat, &rot, &mat_copy);
}

void	rotate_z(t_scene *scene, t_mat4 *rot_mat, float deg)
{
	t_mat4	rot;
	t_mat4	mat_copy;

	if (scene->keys_held.left == true)
		axis_angle(&rot, &scene->cam.dir, deg);
	else
		axis_angle(&rot, &scene->cam.dir, -deg);
	ft_memcpy(&mat_copy, rot_mat, sizeof(t_mat4));
	mat_multiply(rot_mat, &rot, &mat_copy);
}

void	collide_after_transform(t_scene *scene)
{
	if (scene->settings.collisions && (scene->keys_held.w
			|| scene->keys_held.a || scene->keys_held.s || scene->keys_held.d
			|| scene->keys_held.up || scene->keys_held.right
			|| scene->keys_held.q || scene->keys_held.e || scene->keys_held.down
			|| scene->keys_held.left || scene->keys_held.plus
			|| scene->keys_held.minus || scene->keys_held.x || scene->keys_held.y || scene->keys_held.z))
		collide(scene, true, 100, &scene->shapes[scene->shape_idx]);
	// if (scene->keys_held.plus == true)
	// {
	// 	if (collide(scene, false, 1, NULL) == true)
	// 	{
	// 		scene->keys_held.plus = false;
	// 		scene->keys_held.minus = true;
	// 		if (scene->keys_held.shift == false)
	// 			scale_object(scene, &scene->shapes[scene->shape_idx]);
	// 		else
	// 			change_height(scene, &scene->shapes[scene->shape_idx]);
	// 		scene->keys_held.minus = false;
	// 		calculate_transforms(scene);
	// 		draw_scene(scene);
	// 	}
	// }
}

void	scale_cube_sides(t_scene *scene, t_shape *shape)
{
	if (shape->type != CUBE)
		return ;
	if (scene->keys_held.shift == false)
	{
		if (scene->keys_held.x)
			shape->props.scale.x += 0.05;
		if (scene->keys_held.y)
			shape->props.scale.y += 0.05;
		if (scene->keys_held.z)
			shape->props.scale.z += 0.05;
	}
	if (scene->keys_held.shift == true)
	{
		if (scene->keys_held.x && shape->props.scale.x > 0.3)
			shape->props.scale.x -= 0.05;
		if (scene->keys_held.y && shape->props.scale.y > 0.3)
			shape->props.scale.y -= 0.05;
		if (scene->keys_held.z && shape->props.scale.z > 0.3)
			shape->props.scale.z -= 0.05;
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
	if (scene->keys_held.shift == false
		&& (scene->keys_held.plus || scene->keys_held.minus))
		scale_object(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.shift == true
		&& (scene->keys_held.plus == true || scene->keys_held.minus == true))
		change_height(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.shift == false
		&& (scene->keys_held.x || scene->keys_held.y || scene->keys_held.z))
		scale_cube_sides(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.shift == true
		&& (scene->keys_held.x || scene->keys_held.y || scene->keys_held.z))
		scale_cube_sides(scene, &scene->shapes[scene->shape_idx]);
	if (scene->keys_held.shift == false
		&& (scene->keys_held.left == true || scene->keys_held.right == true))
		rotate_y(scene, &scene->shapes[scene->shape_idx].added_rots,
			DEG_TO_RAD * 5);
	if (scene->keys_held.shift == true
		&& (scene->keys_held.left == true || scene->keys_held.right == true))
		rotate_z(scene, &scene->shapes[scene->shape_idx].added_rots,
			DEG_TO_RAD * 5);
	if (scene->keys_held.up == true || scene->keys_held.down == true)
		rotate_x(scene, &scene->shapes[scene->shape_idx].added_rots,
			DEG_TO_RAD * 5);
	collide_after_transform(scene);
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
	if (scene->keys_held.shift == false && scene->keys_held.plus == true)
		light->intensity = min(light->intensity + 0.05, 2);
	if (scene->keys_held.shift == false && scene->keys_held.minus == true)
		light->intensity = max(light->intensity - 0.05, 0);
	if (scene->keys_held.up == true || scene->keys_held.down == true)
		rotate_x(scene, &scene->lights[scene->light_idx].added_rots, -DEG_TO_RAD * 2);
	if (scene->keys_held.left == true || scene->keys_held.right == true)
		rotate_y(scene, &scene->lights[scene->light_idx].added_rots, -DEG_TO_RAD * 2);
	if (scene->keys_held.shift == true && scene->keys_held.plus == true)
		scene->lights[scene->light_idx].theta += 0.02;
	if (scene->keys_held.shift == true && scene->keys_held.minus == true)
	{
		if (scene->lights[scene->light_idx].theta > 0.1)
			scene->lights[scene->light_idx].theta -= 0.02;
	}
	normalize_vec(&scene->lights[scene->light_idx].direction);
}

void	reset_look_at(t_scene *scene)
{
	if (scene->cam.dir.x > 0)
		scene->cam.theta = atan(scene->cam.dir.z / scene->cam.dir.x);
	else if (scene->cam.dir.x < 0 && scene->cam.dir.z >= 0)
		scene->cam.theta = atan(scene->cam.dir.z / scene->cam.dir.x) \
		+ M_PI;
	else if (scene->cam.dir.x < 0 && scene->cam.dir.z < 0)
		scene->cam.theta = atan(scene->cam.dir.z / scene->cam.dir.x) \
		- M_PI;
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

int	render_loop(t_scene *scene)
{
	if (scene->settings.camera_mode == true
		&& scene->settings.edit_mode == true)
		camera_controls(scene);
	else if (scene->settings.edit_mode == true && scene->settings.light_mode == false)
	{
		transform_object(scene);
		mouse_rotate(scene);
		mouse_move(scene);
	}
	else if (scene->settings.edit_mode == true && scene->settings.light_mode == true)
	{
		light_controls(scene);
	}
	if (scene->keys_held.o == true && scene->settings.edit_mode == true \
		&& !scene->look_at.trigger)
		look_at(scene, &scene->shapes[scene->shape_idx]);
	if (scene->look_at.trigger == true && scene->settings.edit_mode == true)
		look_at_animation(scene);
	if (scene->settings.edit_mode == true
		&& (scene->keys_held.w || scene->keys_held.a || scene->keys_held.s
			|| scene->keys_held.d || scene->keys_held.up
			|| scene->keys_held.right || scene->keys_held.q
			|| scene->keys_held.e || scene->keys_held.down
			|| scene->keys_held.left || scene->keys_held.plus
			|| scene->keys_held.minus || scene->keys_held.x
			|| scene->keys_held.y || scene->keys_held.z))
	{
		calculate_transforms(scene);
		draw_scene(scene);
	}
	return (0);
}
