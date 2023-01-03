/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 10:14:05 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/04 01:32:01 by hsarhan          ###   ########.fr       */
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

char	*ft_strjoin_free(char *s1, char *s2, int fre)
{
	char	*str;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	str = ft_strjoin(s1, s2);
	if (fre == 1 || fre == 3)
		free(s1);
	if (fre == 2 || fre == 3)
		free(s2);
	return (str);
}

char	*ft_strtrim_free(char *s, char *set)
{
	char	*str;

	if (s == NULL)
		return (NULL);
	str = ft_strtrim(s, set);
	free(s);
	return (str);
}
