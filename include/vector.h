/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:33:33 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/20 17:53:07 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

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

#endif