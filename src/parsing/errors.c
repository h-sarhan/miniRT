/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 17:41:51 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/06 13:04:33 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

bool	print_shape_error(t_shape *shape, t_shape_errors *err, const char *line,
			int line_num)
{
	if (shape->type == SPHERE)
		return (print_sphere_error(err, line, line_num));
	if (shape->type == CUBE)
		return (print_cube_error(err, line, line_num));
	if (shape->type == PLANE)
		return (print_plane_error(err, line, line_num));
	if (shape->type == CYLINDER)
		return (print_cylinder_error(err, line, line_num));
	return (print_cone_error(err, line, line_num));
}

void	print_error(t_scene *scene, const char *line, int line_num,
			const char *identifer)
{
	t_error_flags	*flags;

	flags = &scene->error_flags;
	if (flags->unknown_identifier == true)
	{
		printf(UNKNOWN_IDENTIFIER, identifer, line_num, line);
		return ;
	}
	if (print_ambient_light_error(&flags->ambient, line, line_num))
		return ;
	if (print_cam_error(&flags->cam, line, line_num))
		return ;
	if (print_light_error(&flags->light, line, line_num))
		return ;
	if (flags->shape.max_shapes)
	{
		printf(MAX_ELEMENT_ERROR, SHAPE_MAX, "shapes");
		return ;
	}
	print_shape_error(&scene->shapes[scene->count.shapes],
		&scene->error_flags.shape, line, line_num);
}

bool	find_error(t_error_flags *err)
{
	return (ft_memchr(err, 1, sizeof(t_error_flags)));
}
