/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 17:41:51 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/03 23:37:14 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"


bool	print_color_error(t_color_error *err, const char *line, int line_num,
			char* element)
{
	if (err->r)
	{
		printf(GENERIC_ERROR, element, line_num, line);
		printf(RED_OOR);
		return (false);
	}
	if (err->g)
	{
		printf(GENERIC_ERROR, element, line_num, line);
		printf(GREEN_OOR);
		return (false);
	}
	if (err->b)
	{
		printf(GENERIC_ERROR, element, line_num, line);
		printf(BLUE_OOR);
		return (false);
	}
	if (err->other)
	{
		printf(GENERIC_ERROR, element, line_num, line);
		printf(AMBIENT_LIGHT_SYNTAX);
		return (false);
	}
	return (true);
}

bool	print_orient_error(t_orient_error *err, const char *line, int line_num,
			char* element)
{
	if (err->x)
	{
		printf(GENERIC_ERROR, element, line_num, line);
		printf(ORIENT_X_OOR);
		return (false);
	}
	if (err->y)
	{
		printf(GENERIC_ERROR, element, line_num, line);
		printf(ORIENT_Y_OOR);
		return (false);
	}
	if (err->z)
	{
		printf(GENERIC_ERROR, element, line_num, line);
		printf(ORIENT_Z_OOR);
		return (false);
	}
	if (err->other)
	{
		printf(GENERIC_ERROR, element, line_num, line);
		printf(CAMERA_SYNTAX);
		return (false);
	}
	if (err->zero)
	{
		printf(GENERIC_ERROR, element, line_num, line);
		printf(ORIENT_ZERO);
		return (false);
	}
	return (true);
}

void	print_errors(t_scene *scene, const char *line, int line_num,
			const char *identifer)
{
	t_error_flags	*flags;

	flags = &scene->parse_errors.errors;
	if (flags->unknown_identifier == true)
	{
		printf(UNKNOWN_IDENTIFIER, identifer, line_num, line);
		return ;
	}
	if (flags->ambient.intensity_other)
	{
		printf(GENERIC_ERROR, "ambient light intensity", line_num, line);
		printf(AMBIENT_LIGHT_SYNTAX);
		return ;
	}
	if (flags->ambient.intensity_range)
	{
		printf(LIGHT_INTENSITY_OOR, "Ambient light", line_num, line);
		return ;
	}
	if (print_color_error(&flags->ambient.color, line, line_num, "ambient light color") == false)
		return ;
	if (flags->ambient.other)
	{
		printf(GENERIC_ERROR, "ambient light", line_num, line);
		printf(AMBIENT_LIGHT_SYNTAX);
		return ;
	}

	if (flags->cam.coords)
	{
		printf(GENERIC_ERROR, "camera coordinates", line_num, line);
		printf(CAMERA_SYNTAX);
		return ;
	}
	print_orient_error(&flags->cam.orient, line, line_num, "camera orientation");
	if (flags->cam.up_vector)
	{
		printf(GENERIC_ERROR, "camera orientation", line_num, line);
		printf(CAMERA_UP_VECTOR);
		return ;
	}
	if (flags->cam.fov_other)
	{
		printf(GENERIC_ERROR, "camera field of view", line_num, line);
		printf(CAMERA_SYNTAX);
		return ;
	}
	if (flags->cam.fov_range)
	{
		printf(GENERIC_ERROR, "camera field of view", line_num, line);
		printf(CAMERA_FOV_OOR);
		return ;
	}
	if (flags->cam.other)
	{
		printf(GENERIC_ERROR, "camera", line_num, line);
		printf(CAMERA_SYNTAX);
		return ;
	}

	if (flags->light.coords)
	{
		printf(GENERIC_ERROR, "light position", line_num, line);
		printf(LIGHT_SYNTAX);
		return ;
	}
	if (flags->light.intensity_range)
	{
		printf(LIGHT_INTENSITY_OOR, "light", line_num, line);
		return ;
	}
	if (flags->light.intensity_other)
	{
		printf(GENERIC_ERROR, "light intensity", line_num, line);
		printf(LIGHT_SYNTAX);
		return ;
	}
	if (flags->light.max_lights)
	{
		printf(MAX_ELEMENT_ERROR, LIGHT_MAX, "lights");
		return ;
	}
	if (flags->light.other)
	{
		printf(GENERIC_ERROR, "light", line_num, line);
		printf(LIGHT_SYNTAX);
		return ;
	}
	print_color_error(&flags->light.color, line, line_num, "light color");
	if (flags->shape.max_shapes)
	{
		printf(MAX_ELEMENT_ERROR, SHAPE_MAX, "shapes");
	}
	t_shape *shape = &scene->shapes[scene->count.shapes];
	if (shape->type == SPHERE)
	{
		print_color_error(&flags->shape.color, line, line_num, "sphere color");
		if (flags->shape.origin)
		{
			printf(GENERIC_ERROR, "sphere origin", line_num, line);
			printf(SPHERE_SYNTAX);
			return ;
		}
		if (flags->shape.diameter_range)
		{
			printf(POSITIVE_VALUE, "Sphere diameter", line_num, line);
			return ;
		}
		if (flags->shape.diameter_other)
		{
			printf(GENERIC_ERROR, "sphere diameter", line_num, line);
			printf(SPHERE_SYNTAX);
			return ;
		}
		if (flags->shape.other)
		{
			printf(GENERIC_ERROR, "sphere", line_num, line);
			printf(SPHERE_SYNTAX);
			return ;
		}
	}
	if (shape->type == CUBE)
	{
		print_color_error(&flags->shape.color, line, line_num, "cube color");
		if (flags->shape.origin)
		{
			printf(GENERIC_ERROR, "cube origin", line_num, line);
			printf(CUBE_SYNTAX);
			return ;
		}
		if (flags->shape.side_len_range)
		{
			printf(POSITIVE_VALUE, "Cube side length", line_num, line);
			return ;
		}
		if (flags->shape.side_len_other)
		{
			printf(GENERIC_ERROR, "cube side length", line_num, line);
			printf(CUBE_SYNTAX);
			return ;
		}
		if (flags->shape.other)
		{
			printf(GENERIC_ERROR, "cube", line_num, line);
			printf(CUBE_SYNTAX);
			return ;
		}
	}
	print_orient_error(&flags->shape.orient, line, line_num, "shape orientation");
	if (flags->shape.origin)
		printf("fsddgv\n");
	if (flags->shape.diameter_other)
		printf("fsddgv\n");
	if (flags->shape.diameter_range)
		printf("fsddgv\n");
	if (flags->shape.height_other)
		printf("fsddgv\n");
	if (flags->shape.height_range)
		printf("fsddgv\n");
	if (flags->shape.other)
		printf("fsddgv\n");
	if (flags->shape.side_len_other)
		printf("fsddgv\n");
	if (flags->shape.side_len_range)
		printf("fsddgv\n");
}


bool	find_error(t_error_flags *err)
{
	return (ft_memchr(err, 1, sizeof(t_error_flags)));
}
