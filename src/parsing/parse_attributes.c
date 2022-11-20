/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_attributes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 16:31:38 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/20 16:31:44 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

/**
 * @brief Parse the red, green, and blue values of a color
 * @param color Color struct to be filled with RGB values
 * @param str Raw string in the form
 * @param success Boolean pointer that is set to false on error
 */
void	parse_color(t_color *color, const char *str, bool *success)
{
	char	**rgb;
	long	res[3];
	bool	parse_success;
	size_t	i;

	parse_success = true;
	rgb = ft_split(str, ',');
	if (!rgb || count_commas(str) != 2 || split_count(rgb) != 3)
	{
		*success = false;
		free_split_array(rgb);
		return ;
	}
	i = -1;
	while (rgb[++i] != NULL)
	{
		res[i] = ft_atol(rgb[i], success);
		if (!is_num(rgb[i], false) || res[i] < 0 || res[i] > 255 || !*success)
			parse_success = false;
	}
	free_split_array(rgb);
	color->r = res[0];
	color->g = res[1];
	color->b = res[2];
	*success = parse_success;
}

/**
 * @brief Parses given coordinates into a vector
 * @param position Pointer to a vector that will be filled with coordinates
 * @param str Raw string to be parsed
 * @param success Boolean pointer that will be set to false on error
 */
void	parse_coordinates(t_vector *position, const char *str, bool *success)
{
	float	res[3];
	char	**coords;
	bool	parse_success;
	size_t	i;

	parse_success = true;
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
			parse_success = false;
	}
	free_split_array(coords);
	position->x = res[0];
	position->y = res[1];
	position->z = res[2];
	*success = parse_success;
}

/**
 * @brief Parses given orientation into a vector
 * @param position Pointer to a vector that will be filled with orientation
 * @param str Raw string to be parsed
 * @param success Boolean pointer that will be set to false on error
 */
void	parse_orientation(t_vector *orientation, const char *str, bool *success)
{
	float	res[3];
	char	**coords;
	size_t	i;
	bool	parse_success;

	parse_success = true;
	coords = ft_split(str, ',');
	if (coords == NULL || count_commas(str) != 2 || split_count(coords) != 3)
	{
		*success = false;
		free_split_array(coords);
		return ;
	}
	i = -1;
	while (coords[++i] != NULL)
	{
		res[i] = ft_atof(coords[i], success);
		if (!is_num(coords[i], true) || !*success || res[i] < -1 || res[i] > 1)
			parse_success = false;
	}
	free_split_array(coords);
	orientation->x = res[0];
	orientation->y = res[1];
	orientation->z = res[2];
	*success = parse_success;
}
