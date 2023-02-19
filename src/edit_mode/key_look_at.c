/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_look_at.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 22:15:33 by mkhan             #+#    #+#             */
/*   Updated: 2023/02/19 22:20:51 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
