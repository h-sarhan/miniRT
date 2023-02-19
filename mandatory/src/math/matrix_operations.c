/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 10:42:19 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/05 21:04:53 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Multiplies matrix m1 by matrix m2 and stores the result in res
 * @param res Result of m1 * m2 will be stored in this matrix
 * @param m1 First matrix
 * @param m2 Second matrix
 */
void	mat_multiply(t_mat4 *res, const t_mat4 *m1, const t_mat4 *m2)
{
	unsigned char	row;
	t_mat4			temp;

	row = 0;
	while (row < 4)
	{
		temp[row][0] = (*m1)[row][0] * (*m2)[0][0] \
			+ (*m1)[row][1] * (*m2)[1][0]
			+ (*m1)[row][2] * (*m2)[2][0]
			+ (*m1)[row][3] * (*m2)[3][0];
		temp[row][1] = (*m1)[row][0] * (*m2)[0][1] \
			+ (*m1)[row][1] * (*m2)[1][1]
			+ (*m1)[row][2] * (*m2)[2][1]
			+ (*m1)[row][3] * (*m2)[3][1];
		temp[row][2] = (*m1)[row][0] * (*m2)[0][2] \
			+ (*m1)[row][1] * (*m2)[1][2]
			+ (*m1)[row][2] * (*m2)[2][2]
			+ (*m1)[row][3] * (*m2)[3][2];
		temp[row][3] = (*m1)[row][0] * (*m2)[0][3] \
			+ (*m1)[row][1] * (*m2)[1][3]
			+ (*m1)[row][2] * (*m2)[2][3]
			+ (*m1)[row][3] * (*m2)[3][3];
		row++;
	}
	ft_memcpy(res, &temp, sizeof(t_mat4));
}

/**
 * @brief Multiplies a matrix by a vector and stores the result in res
 * @param res Vector that stores the result of a matrix multiplied by a vector
 * @param mat Matrix to be multiplied
 * @param vec Vector to be multiplied
 */
void	mat_vec_multiply(t_vector *res, const t_mat4 *mat,
			const t_vector *vec)
{
	res->x = vec->x * (*mat)[0][0] + vec->y * (*mat)[0][1] \
	+ vec->z * (*mat)[0][2] + vec->w * (*mat)[0][3];
	res->y = vec->x * (*mat)[1][0] + vec->y * (*mat)[1][1] \
	+ vec->z * (*mat)[1][2] + vec->w * (*mat)[1][3];
	res->z = vec->x * (*mat)[2][0] + vec->y * (*mat)[2][1] \
	+ vec->z * (*mat)[2][2] + vec->w * (*mat)[2][3];
	res->w = vec->x * (*mat)[3][0] + vec->y * (*mat)[3][1] \
	+ vec->z * (*mat)[3][2] + vec->w * (*mat)[3][3];
}

/**
 * @brief Sets the matrix to the identity matrix
 * @param mat The matrix to be set to an identity matrix
 */
void	identity_matrix(t_mat4 *mat)
{
	ft_bzero(mat, sizeof(t_mat4));
	(*mat)[0][0] = 1;
	(*mat)[1][1] = 1;
	(*mat)[2][2] = 1;
	(*mat)[3][3] = 1;
}

/**
 * @brief Transposes a given matrix in place
 * @param mat Matrix to be transposed
 */
void	transpose_matrix(t_mat4 *mat)
{
	double			temp;
	unsigned char	row;
	unsigned char	col;

	row = 1;
	while (row < 4)
	{
		col = 0;
		while (col < row)
		{
			temp = (*mat)[row][col];
			(*mat)[row][col] = (*mat)[col][row];
			(*mat)[col][row] = temp;
			col++;
		}
	row++;
	}
}
