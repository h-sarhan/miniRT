/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 17:41:51 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/03 21:35:42 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"


bool	print_color_error(t_color_error *err, const char *line, int line_num,
			char* element)
{
	if (err->r)
	{
		printf(COLOR_ERROR, element, line_num, line);
		printf(RED_OOR);
		return (false);
	}
	if (err->g)
	{
		printf(COLOR_ERROR, element, line_num, line);
		printf(GREEN_OOR);
		return (false);
	}
	if (err->b)
	{
		printf(COLOR_ERROR, element, line_num, line);
		printf(BLUE_OOR);
		return (false);
	}
	if (err->other)
	{
		printf(COLOR_ERROR, element, line_num, line);
		printf(AMBIENT_LIGHT_SYNTAX);
		return (false);
	}
	return (true);
}

void	print_orient_error(t_orient_error *err, const char *line, int line_num,
			char* element)
{
	(void)line;
	(void)line_num;
	(void)element;
	if (err->x)
		printf("X\n");
	if (err->y)
		printf("y\n");
	if (err->z)
		printf("Z\n");
	if (err->other)
		printf("other\n");
	if (err->zero)
		printf("zero\n");

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
	if (flags->ambient.other)
	{
		printf(GENERIC_ERROR, "ambient light", line_num, line);
		printf(AMBIENT_LIGHT_SYNTAX);
		return ;
	}
		
	if (print_color_error(&flags->ambient.color, line, line_num, "ambient light") == false)
		return ;

	print_color_error(&flags->light.color, line, line_num, "light");
	print_color_error(&flags->shape.color, line, line_num, "shape");
	print_orient_error(&flags->cam.orient, line, line_num, "camera");
	print_orient_error(&flags->shape.orient, line, line_num, "shape");
	if (flags->cam.coords)
		printf("fsddgv\n");
	if (flags->cam.fov)
		printf("fsddgv\n");
	if (flags->cam.up_vector)
		printf("fsddgv\n");
	if (flags->cam.other)
		printf("fsddgv\n");
	if (flags->light.coords)
		printf("fsddgv\n");
	if (flags->light.intensity_range)
		printf("fsddgv\n");
	if (flags->light.intensity_other)
		printf("fsddgv\n");
	if (flags->light.max_lights)
		printf("fsddgv\n");
	if (flags->light.other)
		printf("fsddgv\n");
	if (flags->shape.coordinates)
		printf("fsddgv\n");
	if (flags->shape.diameter_other)
		printf("fsddgv\n");
	if (flags->shape.diameter_range)
		printf("fsddgv\n");
	if (flags->shape.height_other)
		printf("fsddgv\n");
	if (flags->shape.height_range)
		printf("fsddgv\n");
	if (flags->shape.max_shapes)
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
