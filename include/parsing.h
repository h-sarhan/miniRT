/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:45:41 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/18 14:47:08 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <fcntl.h>
# include <libft.h>
# include <scene.h>
# include <stdio.h>
# define RED "\x1b[31m"
# define RESET "\x1b[0m"
# define YELLOW "\x1b[33m"

# define LIGHT_MAX 20
# define SHAPE_MAX 100

t_scene	*parse_scene(const char *file_name);
size_t	split_count(char **split);

void	print_scene(const t_scene *scene);

void	free_scene(t_scene *scene);

void	parse_light(t_scene *scene, char **splitted, bool *success);

#endif