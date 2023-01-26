/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mathRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 10:38:18 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/26 20:09:30 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATHRT_H
# define MATHRT_H

# define RAD_TO_DEG 57.2957795131
# define DEG_TO_RAD 0.01745329252

/**
 * @brief 4 dimensional vector
 */
typedef struct s_vector	t_vector;
struct s_vector
{
	float	x;
	float	y;
	float	z;
	float	w;
};

float	vec_magnitude(const t_vector *vec);
float	dot_product(const t_vector *v1, const t_vector *v2);
void	normalize_vec(t_vector *vec);
void	cross_product(t_vector *res, const t_vector *v1, const t_vector *v2);
float	vec_distance(const t_vector *v1, const t_vector *v2);

void	scale_vec(t_vector *res, const t_vector *v, float scale);
void	sub_vec(t_vector *res, const t_vector *v1, const t_vector *v2);
void	negate_vec(t_vector *res, const t_vector *v);
void	add_vec(t_vector *res, const t_vector *v1, const t_vector *v2);

void	reflect_vector(t_vector *res, t_vector *in_vector, t_vector *normal);

typedef float			t_mat4[4][4];
typedef float			t_mat3[3][3];
typedef float			t_mat2[2][2];

void	identity_matrix(t_mat4 *mat);
void	transpose_matrix(t_mat4 *mat);
void	translate_matrix(t_mat4 *mat, float x, float y, float z);
void	scaling_matrix(t_mat4 *mat, float x, float y, float z);

void	rotation_matrix_x(t_mat4 *mat, float r);
void	rotation_matrix_y(t_mat4 *mat, float r);
void	rotation_matrix_z(t_mat4 *mat, float r);
void	axis_angle(t_mat4 *rot_mat, const t_vector *ax, float angle);

void	mat_vec_multiply(t_vector *res, const t_mat4 *mat,
			const t_vector *vec);
void	mat_multiply(t_mat4 *res, const t_mat4 *m1, const t_mat4 *m2);

void	submat4(t_mat3 *res, const t_mat4 *mat, int row, int col);
float	det3(const t_mat3 *mat3);
void	mat_inverse(t_mat4 *res, const t_mat4 *mat);

#endif
