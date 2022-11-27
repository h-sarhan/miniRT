/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:33:33 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/27 13:56:32 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include "miniRT.h"

/**
 * @brief 4 dimensional vector
 */
typedef struct s_vector	t_vector;
struct s_vector
{
	double	x;
	double	y;
	double	z;
	double	w;
};

// vector_arithmetic.c
void	add_vec(t_vector *res, const t_vector *v1, const t_vector *v2);
void	sub_vec(t_vector *res, const t_vector *v1, const t_vector *v2);
void	negate_vec(t_vector *res, const t_vector *v);
void	scale_vec(t_vector *res, const t_vector *v, double scale);

// vector_operations.c
double	vec_magnitude(const t_vector *vec);
void	normalize_vec(t_vector *vec);
double	dot_product(const t_vector *v1, const t_vector *v2);
void	cross_product(t_vector *res, const t_vector *v1, const t_vector *v2);

#endif