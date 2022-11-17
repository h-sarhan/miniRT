/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:01:06 by hsarhan           #+#    #+#             */
/*   Updated: 2022/11/17 15:47:38 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		return (1);
	}
	bool success = true;
	t_scene *scene = parse(argv[1], &success);
}
