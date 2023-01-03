/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 17:41:51 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/03 20:01:07 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	print_errors(t_scene *scene)
{
	(void)scene;
	printf("Error found\n");
}


bool	find_error(t_error_flags *err)
{
	return (ft_memchr(err, 1, sizeof(t_error_flags)));
}
