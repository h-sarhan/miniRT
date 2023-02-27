/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 18:50:31 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/27 13:12:57 by hsarhan          ###   ########.fr       */
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

void	scale_object(t_scene *scene, t_shape *shape)
{
	double	amount;

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

int	render_loop(t_scene *scene)
{
	if (scene->settings.camera_mode == true
		&& scene->settings.edit_mode == true)
		camera_controls(scene);
	else if (scene->settings.edit_mode && !scene->settings.light_mode)
	{
		transform_object(scene);
		mouse_rotate(scene);
		mouse_move(scene);
	}
	else if (scene->settings.edit_mode && scene->settings.light_mode)
		light_controls(scene);
	if (scene->keys_held.o && scene->settings.edit_mode \
		&& !scene->look_at.trigger)
		look_at(scene, &scene->shapes[scene->shape_idx]);
	if (scene->look_at.trigger == true && scene->settings.edit_mode == true)
		look_at_animation(scene);
	if (scene->settings.edit_mode == true
		&& is_loop_hook_key(scene))
	{
		calculate_transforms(scene);
		draw_scene(scene);
	}
	return (0);
}

void	collide_after_transform(t_scene *scene)
{
	if (scene->settings.collisions && (scene->keys_held.w
			|| scene->keys_held.a || scene->keys_held.s || scene->keys_held.d
			|| scene->keys_held.up || scene->keys_held.right
			|| scene->keys_held.q || scene->keys_held.e || scene->keys_held.down
			|| scene->keys_held.left || scene->keys_held.plus
			|| scene->keys_held.minus || scene->keys_held.x || scene->keys_held.y || scene->keys_held.z))
		collide(scene, true, 10, &scene->shapes[scene->shape_idx]);
}