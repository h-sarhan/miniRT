/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 09:54:26 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/04 17:32:40 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Returns a vector's magnitude
 * @param vec A vector
 * @return vec's magnitude
 */
double	vec_magnitude(const t_vector *vec)
{
	return (sqrtf(vec->x * vec->x + vec->y * vec->y \
			+ vec->z * vec->z + vec->w * vec->w));
}

float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

/**
 * @brief Normalizes a vector
 * @param vec Vector to be normalized
 */
void	normalize_vec(t_vector *vec)
{
	float	mag;

	mag = vec_magnitude(vec);
	if (fabs(mag) > 0.001)
	{
		scale_vec(vec, vec, Q_rsqrt(vec->x * vec->x + vec->y * vec->y \
			+ vec->z * vec->z + vec->w * vec->w));
	}
}

/**
 * @brief Returns the dot product of two vectors
 * @param v1 First vector
 * @param v2 Second vector
 * @return The dot product of v1 and v2
 */
double	dot_product(const t_vector *v1, const t_vector *v2)
{
	return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z + v1->w * v2->w);
}

/**
 * @brief Finds the cross product between two vectors and stores the result in
 * res
 * @param res The result of the cross product will be stored in this vector
 * @param v1 First operand
 * @param v2 Second operand
 */
void	cross_product(t_vector *res, const t_vector *v1, const t_vector *v2)
{
	res->x = v1->y * v2->z - v1->z * v2->y;
	res->y = v1->z * v2->x - v1->x * v2->z;
	res->z = v1->x * v2->y - v1->y * v2->x;
}
