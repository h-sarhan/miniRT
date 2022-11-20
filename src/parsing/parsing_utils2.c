/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 10:10:43 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/20 16:23:52 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

/**
 * @brief Returns whether an identifier is shape
 * @param identifier 
 * @return True if the identifier is a shape
 */
bool	is_shape(const char *identifier)
{
	return (ft_strcmp(identifier, "sp") == 0
		|| ft_strcmp(identifier, "pl") == 0
		|| ft_strcmp(identifier, "cy") == 0
		|| ft_strcmp(identifier, "cu") == 0);
}

// ! COMMENT THIS LATER
bool	check_color(const t_color *color, size_t line_num, const char *line,
	const char *element)
{
	if (color->r < 0 || color->r > 255
		|| color->g < 0 || color->g > 255
		|| color->b < 0 || color->b > 255)
	{
		printf(YELLOW"Error with parsing %s color on line #%ld\n"
			RED"->\t%s\n"RESET, element, line_num, line);
		if (color->r < 0 || color->r > 255)
			printf(YELLOW"The red value is out of range\n"RESET);
		if (color->g < 0 || color->g > 255)
			printf(YELLOW"The green value is out of range\n"RESET);
		if (color->b < 0 || color->b > 255)
			printf(YELLOW"The blue value is out of range\n"RESET);
		return (true);
	}
	return (false);
}

// ! WRITE COMMENTS LATER
bool	check_orientation(const t_vector *orientation, size_t line_num,
	const char *line, const char *element)
{
	if (orientation->x < 0.0 || orientation->x > 1.0
		|| orientation->y < 0.0 || orientation->y > 1.0
		|| orientation->z < 0.0 || orientation->z > 1.0)
	{
		printf(YELLOW"Error with parsing %s orientation on line #%ld\n"
			RED"->\t%s\n"RESET, element, line_num, line);
		if (orientation->x < 0.0 || orientation->x > 1.0)
			printf(YELLOW"The x value is out of range\n"RESET);
		if (orientation->y < 0.0 || orientation->y > 1.0)
			printf(YELLOW"The y value is out of range\n"RESET);
		if (orientation->z < 0.0 || orientation->z > 1.0)
			printf(YELLOW"The z value is out of range\n"RESET);
		return (true);
	}
	return (false);
}
