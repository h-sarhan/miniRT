/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 10:14:05 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/02 21:08:06 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * @brief Frees an array allocated by split
 * @param arr Array to free
 */
void	free_split_array(char **arr)
{
	size_t	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/**
 * @brief Frees a scene struct
 * @param scene Pointer to scene struct to be freed
 */
void	free_scene(t_scene *scene)
{
	printf("FREE FUNCTION DOES NOT FREE EVERYTHING\n");
	if (scene->lights)
		free(scene->lights);
	if (scene->shapes)
		free(scene->shapes);
	free(scene);
}
