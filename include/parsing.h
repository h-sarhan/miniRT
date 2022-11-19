/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:45:41 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/19 16:33:15 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <fcntl.h>
# include <libft.h>
# include <scene.h>
# include <stdio.h>
# include <utils.h>
# define RED "\x1b[31m"
# define RESET "\x1b[0m"
# define YELLOW "\x1b[33m"

# define LIGHT_MAX 20
# define SHAPE_MAX 100

t_scene	*parse_scene(int fd);
size_t	split_count(char **split);
void	print_scene(const t_scene *scene);
void	free_scene(t_scene *scene);
void	parse_light(t_scene *scene, char **splitted, bool *success);
bool	is_whitespace(const char c);
bool	all_whitespace(const char *str);
size_t	count_commas(const char *str);
bool	is_num(const char *str, bool decimal);
bool	is_shape(const char *identifier);
void	*camera_parse_error(char *line, size_t line_num, t_scene *scene,
			char **splitted);
void	*ambient_parse_error(char *line, size_t line_num, t_scene *scene,
			char **splitted);
void	*unknown_identifier_error(char *line, size_t line_num, t_scene *scene,
			char **splitted);
void	*shape_parse_error(char *line, size_t line_count, t_scene *scene,
			char **splitted);
void	*light_parse_error(char *line, size_t line_count, t_scene *scene,
			char **splitted);
bool	check_orientation(const t_vector *orientation, size_t line_num,
			const char *line, const char *element);
bool	check_color(const t_color *color, size_t line_num, const char *line,
			const char *element);
bool	check_orientation(const t_vector *orientation, size_t line_num,
			const char *line, const char *element);
bool	is_shape(const char *identifier);
bool	check_color(const t_color *color, size_t line_num, const char *line,
			const char *element);
bool	check_orientation(const t_vector *orientation, size_t line_num,
			const char *line, const char *element);
#endif