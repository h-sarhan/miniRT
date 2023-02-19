/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 10:14:56 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/05 21:04:53 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

void	free_split_array(char **arr);
char	*ft_strtrim_free(char *s, char *set);
double	max3(double n1, double n2, double n3);
double	min3(double n1, double n2, double n3);
double	min(double a, double b);
double	max(double a, double b);

void	transform_ray(t_ray *transformed_ray, const t_ray *ray,
			const t_shape *shape);

void	print_vector(const t_vector *vector);
void	print_color(const t_color *color);
void	print_mat4(const t_mat4 *mat);

char	*ft_strjoin_free(char *s1, char *s2, int fre);
char	*ft_strtrim_free(char *s, char *set);
t_color	int_to_color(int hex_color);

#endif
