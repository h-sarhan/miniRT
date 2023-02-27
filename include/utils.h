/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 10:14:56 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/27 12:36:10 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "shape.h"
# include "intersections.h"

void	free_split_array(char **arr);
char	*ft_strtrim_free(char *s, char *set);
void	free_textures(t_scene *scene);
void	free_texture(t_shape *shape, t_color **texture);

double	max3(double n1, double n2, double n3);
double	min3(double n1, double n2, double n3);
double	min(double a, double b);
double	max(double a, double b);

void	transform_ray(t_ray *transformed_ray, const t_ray *ray,
			const t_shape *shape);

char	*ft_strjoin_free(char *s1, char *s2, int fre);
char	*ft_strtrim_free(char *s, char *set);
t_color	int_to_color(int hex_color);
void	handle_color_change(int key, t_scene *scene, t_color *color);

#endif
