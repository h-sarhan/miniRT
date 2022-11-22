/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 10:14:56 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/22 18:00:15 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "miniRT.h"

// free_utils.c
void	free_split_array(char **arr);
void	free_scene(t_scene *scene);

// print_utils.c
void	print_scene(const t_scene *scene);
void	print_vector(const t_vector *vector);

#endif