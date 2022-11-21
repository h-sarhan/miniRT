/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 10:42:19 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/21 11:09:20 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <matrix.h>

/**
 * @brief Multiplies matrix m1 by matrix m2 and stores the result in res
 * @param res Result of m1 * m2 will be stored in this matrix
 * @param m1 First matrix
 * @param m2 Second matrix
 */
void	mat_multiply(t_matrix *res, const t_matrix *m1, const t_matrix *m2)
{
	unsigned char	row;

	row = 0;
	while (row < 4)
	{
		(*res)[row][0] = (*m1)[row][0] * (*m2)[0][0]
					+ (*m1)[row][1] * (*m2)[1][0]
					+ (*m1)[row][2] * (*m2)[2][0]
					+ (*m1)[row][3] * (*m2)[3][0];
		(*res)[row][1] = (*m1)[row][0] * (*m2)[0][1]
					+ (*m1)[row][1] * (*m2)[1][1]
					+ (*m1)[row][2] * (*m2)[2][1]
					+ (*m1)[row][3] * (*m2)[3][1];
		(*res)[row][2] = (*m1)[row][0] * (*m2)[0][2]
					+ (*m1)[row][1] * (*m2)[1][2]
					+ (*m1)[row][2] * (*m2)[2][2]
					+ (*m1)[row][3] * (*m2)[3][2];
		(*res)[row][3] = (*m1)[row][0] * (*m2)[0][3]
					+ (*m1)[row][1] * (*m2)[1][3]
					+ (*m1)[row][2] * (*m2)[2][3]
					+ (*m1)[row][3] * (*m2)[3][3];
		row++;
	}
}

// /**
//  * @brief Multiplies a matrix by a vector
//  * @param res 
//  * @param mat 
//  * @param vec 
//  */
// void	mat_vec_multiply(t_vector *res, const t_matrix *mat,
// 			const t_vector *vec)
// {
// 	// res->
// }
