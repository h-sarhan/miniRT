/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 10:14:56 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/21 12:55:17 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdlib.h>
# include <scene.h>
# include <matrix.h>

void	free_split_array(char **arr);
void	free_scene(t_scene *scene);
void	print_scene(const t_scene *scene);
void	print_mat4(const t_mat4 *mat);
void	print_mat3(const t_mat3 *mat);
void	print_mat2(const t_mat2 *mat);

#endif