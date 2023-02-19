/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 15:04:57 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/20 00:45:06 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	get_tb(t_vector *normal, t_vector *tb)
{
	t_vector	up;
	t_vector	z_up;

	ft_bzero(&up, sizeof(t_vector));
	ft_bzero(&z_up, sizeof(t_vector));
	up.y = 1;
	z_up.z = 1;
	cross_product(&tb[0], normal, &up);
	if (vec_magnitude(&tb[0]) < 0.000001)
		cross_product(&tb[0], normal, &z_up);
	normalize_vec(&tb[0]);
	cross_product(&tb[1], normal, &tb[0]);
	normalize_vec(&tb[1]);
}
