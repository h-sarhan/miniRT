/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_attributes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 16:31:38 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/05 21:04:53 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	check_color_range(t_color *color, t_color_error *err)
{
	if (err->other == true)
		return ;
	if (color->r > 1 || color->r < 0)
		err->r = true;
	if (color->g > 1 || color->g < 0)
		err->g = true;
	if (color->b > 1 || color->b < 0)
		err->b = true;
}

/**
 * @brief Parse the red, green, and blue values of a color
 * @param color Color struct to be filled with RGB values
 * @param str Raw string in the form
 * @param success Boolean pointer that is set to false on error
 */
void	parse_color(t_color *color, const char *str, t_color_error *errs)
{
	char	**rgb;
	double	res[3];
	size_t	i;
	bool	success;

	success = true;
	rgb = ft_split(str, ',');
	if (!rgb || count_commas(str) != 2 || split_count(rgb) != 3)
	{
		errs->other = true;
		free_split_array(rgb);
		return ;
	}
	i = -1;
	while (rgb[++i] != NULL)
	{
		res[i] = ft_atol(rgb[i], &success) / 255.0f;
		if (!is_num(rgb[i], false) || success == false)
			errs->other = true;
	}
	free_split_array(rgb);
	color->r = res[0];
	color->g = res[1];
	color->b = res[2];
	check_color_range(color, errs);
}

/**
 * @brief Parses given coordinates into a vector
 * @param position Pointer to a vector that will be filled with coordinates
 * @param str Raw string to be parsed
 * @param success Boolean pointer that will be set to false on error
 */
void	parse_coordinates(t_vector *position, const char *str, bool *success)
{
	double	res[3];
	char	**coords;
	size_t	i;

	coords = ft_split(str, ',');
	if (!coords || count_commas(str) != 2 || split_count(coords) != 3)
	{
		*success = false;
		free_split_array(coords);
		return ;
	}
	i = -1;
	while (coords[++i] != NULL)
	{
		res[i] = ft_atof(coords[i], success);
		if (!is_num(coords[i], true) || *success == false)
			*success = false;
	}
	free_split_array(coords);
	position->x = res[0];
	position->y = res[1];
	position->z = res[2];
	position->w = 1;
}

void	check_orientation_vector(t_vector *orientation, t_orient_error *err)
{
	if (err->other == true)
		return ;
	if (orientation->x < -1 || orientation->x > 1)
		err->x = true;
	if (orientation->y < -1 || orientation->y > 1)
		err->y = true;
	if (orientation->z < -1 || orientation->z > 1)
		err->z = true;
	if (vec_magnitude(orientation) == 0)
		err->zero = true;
}

/**
 * @brief Parses given orientation into a vector
 * @param position Pointer to a vector that will be filled with orientation
 * @param str Raw string to be parsed
 * @param success Boolean pointer that will be set to false on error
 */
void	parse_orientation(t_vector *orientation, const char *str,
			t_orient_error *err)
{
	double	res[3];
	char	**xyz;
	size_t	i;
	bool	success;

	success = true;
	xyz = ft_split(str, ',');
	if (xyz == NULL || count_commas(str) != 2 || split_count(xyz) != 3)
	{
		free_split_array(xyz);
		err->other = true;
		return ;
	}
	i = -1;
	while (xyz[++i] != NULL)
	{
		res[i] = ft_atof(xyz[i], &success);
		if (success == false || !is_num(xyz[i], true))
			err->other = true;
	}
	free_split_array(xyz);
	orientation->x = res[0];
	orientation->y = res[1];
	orientation->z = res[2];
	check_orientation_vector(orientation, err);
}
