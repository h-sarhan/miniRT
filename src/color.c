/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 17:06:05 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/18 13:56:35 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <color.h>

/**
 * @brief Print the red, green, and blue values of a color
 * @param color Color to print
 */
void	print_color(const t_color *color)
{
	printf("\tR: %d\n", color->r);
	printf("\tG: %d\n", color->g);
	printf("\tB: %d\n", color->b);
}
