/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 10:38:18 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/21 11:01:13 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_H
# define MATRIX_H

# include <vector.h>

typedef float	t_matrix[4][4];
void	mat_multiply(t_matrix *res, const t_matrix *m1, const t_matrix *m2);

#endif