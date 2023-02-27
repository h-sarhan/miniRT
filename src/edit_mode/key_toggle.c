/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_toggle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 22:16:35 by mkhan             #+#    #+#             */
/*   Updated: 2023/02/19 22:17:58 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

bool	is_toggle_key(int key, t_scene *scene)
{
	return (key == KEY_SPACE || key == KEY_R
		|| (scene->settings.edit_mode == true
			&& (key == KEY_RETURN || key == KEY_1 || key == KEY_2
				|| key == KEY_3 || key == KEY_4 || key == KEY_5 || key == KEY_6
				|| key == KEY_TAB || key == KEY_C || key == KEY_T
				|| key == KEY_P || key == KEY_L || key == KEY_H)));
}

void	toggle_edit_mode(int key, t_scene *scene)
{
	if (key == KEY_SPACE)
		scene->settings.edit_mode = !scene->settings.edit_mode;
}

void	toggle_shape_properties(t_shape *shape)
{
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
	toggle_shape_properties(shape);
}

void	toggle_reflections(t_scene *scene)
{
	if (scene->settings.reflection_depth == 0)
		scene->settings.reflection_depth = REFLECTION_DEPTH;
	else if (scene->settings.reflection_depth != 0)
		scene->settings.reflection_depth = 0;
}
