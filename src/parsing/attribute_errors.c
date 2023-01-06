/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attribute_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 13:01:46 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/06 13:02:43 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// returns true if printed
bool	print_color_error(t_color_error *err, const char *line, int line_num,
			char *element)
{
	if (err->r || err->g || err->b || err->other)
		printf(GENERIC_ERROR, element, line_num, line);
	if (err->r)
		printf(RED_OOR);
	else if (err->g)
		printf(GREEN_OOR);
	else if (err->b)
		printf(BLUE_OOR);
	return (ft_memchr(err, 1, sizeof(t_color_error)));
}

// returns true if printed
bool	print_orient_error(t_orient_error *err, const char *line, int line_num,
			char *element)
{
	if (err->x || err->y || err->z || err->other || err->zero)
		printf(GENERIC_ERROR, element, line_num, line);
	if (err->x)
		printf(ORIENT_X_OOR);
	else if (err->y)
		printf(ORIENT_Y_OOR);
	else if (err->z)
		printf(ORIENT_Z_OOR);
	else if (err->zero)
		printf(ORIENT_ZERO);
	return (ft_memchr(err, 1, sizeof(t_orient_error)));
}
