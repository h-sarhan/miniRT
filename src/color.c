/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 17:06:05 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/17 18:13:53 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <color.h>

/**
 * @brief Print the red, green, and blue values of a color
 * @param color Color to print
 */
void	print_color(const t_color *color)
{
	printf("R: %u\n", color->r);
	printf("G: %u\n", color->g);
	printf("B: %u\n", color->b);
}
