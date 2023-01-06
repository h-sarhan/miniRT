/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   element_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 13:02:58 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/06 13:12:37 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// returns true if printed
bool	print_ambient_light_error(t_ambient_errors *err, const char *line,
			int line_num)
{
	if (err->intensity_other)
	{
		printf(GENERIC_ERROR, "ambient light intensity", line_num, line);
		printf(AMBIENT_LIGHT_SYNTAX);
	}
	else if (err->intensity_range)
		printf(LIGHT_INTENSITY_OOR, "Ambient light", line_num, line);
	else if (print_color_error(&err->color, line, line_num,
			"ambient light color"))
		return (true);
	else if (err->other)
	{
		printf(GENERIC_ERROR, "ambient light", line_num, line);
		printf(AMBIENT_LIGHT_SYNTAX);
	}
	return (ft_memchr(err, 1, sizeof(t_ambient_errors)));
}

// returns true if printed
bool	print_cam_error(t_cam_errors *err, const char *line, int line_num)
{
	if (err->coords)
		printf(GENERIC_ERROR, "camera origin", line_num, line);
	else if (print_orient_error(&err->dir, line, line_num,
			"camera orientation") && err->dir.other)
		printf(CAMERA_SYNTAX);
	else if (err->up_vector)
	{
		printf(GENERIC_ERROR, "camera orientation", line_num, line);
		printf(CAMERA_UP_VECTOR);
	}
	else if (err->fov_range || err->fov_other)
	{
		printf(GENERIC_ERROR, "camera field of view", line_num, line);
		if (err->fov_range)
			printf(CAMERA_FOV_OOR);
	}
	else if (err->other)
		printf(GENERIC_ERROR, "camera", line_num, line);
	if (err->coords || err->fov_other || err->other)
		printf(CAMERA_SYNTAX);
	return (ft_memchr(err, 1, sizeof(t_cam_errors)));
}

// returns true if printed
bool	print_light_error(t_light_errors *err, const char *line, int line_num)
{
	if (err->coords)
		printf(GENERIC_ERROR, "light position", line_num, line);
	else if (err->intensity_range)
		printf(LIGHT_INTENSITY_OOR, "light", line_num, line);
	else if (err->intensity_other)
		printf(GENERIC_ERROR, "light intensity", line_num, line);
	else if (err->max_lights)
		printf(MAX_ELEMENT_ERROR, LIGHT_MAX, "lights");
	else if (err->other)
		printf(GENERIC_ERROR, "light", line_num, line);
	else if (print_color_error(&err->color, line, line_num,
			"light color") && err->other)
		printf(LIGHT_SYNTAX);
	if (err->coords || err->intensity_other || err->other)
		printf(LIGHT_SYNTAX);
	return (ft_memchr(err, 1, sizeof(t_light_errors)));
}
