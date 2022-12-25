/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:45:41 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/26 01:05:23 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# define RED "\x1b[31m"
# define RESET "\x1b[0m"
# define YELLOW "\x1b[33m"
# define MAGENTA "\e[0;35m"
# define GREEN  "\x1B[32m"
# define LIGHT_MAX 20
# define SHAPE_MAX 100

bool	check_color(const t_color *color, size_t line_num, const char *line,
			const char *element);
bool	check_orientation(const t_vector *orientation, size_t line_num,
			const char *line, const char *element);

bool	parse_ambient(t_scene *scene, char **splitted, size_t line_num,
			char *line);
bool	parse_camera(t_scene *scene, char **splitted, char *line,
			size_t line_num);
bool	parse_light(t_scene *scene, char **splitted, char *line,
			size_t line_num);

bool	parse_shape(t_scene *scene, char **splitted, size_t line_num,
			char *line);

bool	parse_settings(t_scene *scene, const char *settings_start,
			size_t line_num, int fd);

void	*unknown_identifier(char *line, size_t line_num, t_scene *scene,
			char **splitted);
void	*shape_parse_error(char *line, size_t line_num, t_scene *scene,
			char **splitted);
void	*light_parse_error(char *line, size_t line_num, t_scene *scene);
void	*ambient_parse_error(char *line, size_t line_num, t_scene *scene);
void	*camera_parse_error(char *line, size_t line_num, t_scene *scene,
			bool invalid_coords);

void	parse_coordinates(t_vector *position, const char *str, bool *success);
void	parse_orientation(t_vector *orientation, const char *str,
			bool *success);
void	parse_color(t_color *color, const char *str, bool *success);

bool	is_settings(const char *line);
bool	is_shape(const char *identifier);

bool	is_num(const char *str, bool decimal);
size_t	count_commas(const char *str);
size_t	split_count(char **split);

t_scene	*parse_scene(int fd);

#endif
