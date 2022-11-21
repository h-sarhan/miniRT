/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_inverse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:53:01 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/21 13:00:28 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <matrix.h>

/**
 * @brief Returns a determinant of a 2x2 matrix
 * @param mat 2x2 matrix
 * @return The determinat of the 2x2 matrix
 */
float	det2(const t_mat2 *mat)
{
	return ((*mat)[0][0] * (*mat)[1][1] - (*mat)[0][1] * (*mat)[1][0]);
}

/**
 * @brief Computes a 2x2 submatrix of a 3x3 matrix
 * @param res The submatrix is stored in this parameter
 * @param mat The 3x3 matrix
 * @param row The row where the submatrix is present
 * @param col The column where the submatrix is present
 */
void	submat3(t_mat2 *res, const t_mat3 *mat, int row, int col)
{
	int	i;
	int	j;
	int	i_sub;
	int	j_sub;

	i = -1;
	i_sub = 0;
	while (++i < 3)
	{
		if (i == row)
			continue ;
		j_sub = 0;
		j = 0;
		while (j < 3)
		{
			if (j == col)
				j++;
			else
				(*res)[i_sub][j_sub++] = (*mat)[i][j++];
		}
		i_sub++;
	}
}

/**
 * @brief Computes a 3x3 submatrix of a 4x4 matrix
 * @param res The submatrix is stored in this parameter
 * @param mat The 4x4 matrix
 * @param row The row where the submatrix is present
 * @param col The column where the submatrix is present
 */
void	submat4(t_mat3 *res, const t_mat4 *mat, int row, int col)
{
	int	i;
	int	j;
	int	i_sub;
	int	j_sub;

	i = -1;
	i_sub = 0;
	while (++i < 4)
	{
		if (i == row)
			continue ;
		j_sub = 0;
		j = 0;
		while (j < 4)
		{
			if (j == col)
				j++;
			else
				(*res)[i_sub][j_sub++] = (*mat)[i][j++];
		}
		i_sub++;
	}
}

