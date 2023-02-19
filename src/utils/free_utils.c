/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 10:14:05 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/19 20:37:02 by hsarhan          ###   ########.fr       */
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

void	free_textures(t_scene *scene)
{
	int	i;

	i = -1;
	while (++i < scene->count.shapes && scene->shapes != NULL)
	{
		free_texture(&scene->shapes[i], scene->shapes[i].diffuse_tex);
		free_texture(&scene->shapes[i], scene->shapes[i].normal_tex);
	}
}

/**
 * @brief Frees a scene struct
 * @param scene Pointer to scene struct to be freed
 */
void	free_scene(t_scene *scene)
{
	int	i;

	if (scene == NULL)
		return ;
	if (scene->disp != NULL && scene->disp->mlx != NULL)
	{
		mlx_destroy_image(scene->disp->mlx, scene->disp->display_img);
		mlx_destroy_image(scene->disp->mlx, scene->disp->render_img);
		mlx_destroy_image(scene->disp->mlx, scene->disp->edit_img);
		mlx_destroy_window(scene->disp->mlx, scene->disp->win);
		mlx_destroy_display(scene->disp->mlx);
		free(scene->disp->mlx);
	}
	free_textures(scene);
	if (scene->lights != NULL)
		free(scene->lights);
	if (scene->shapes != NULL)
		free(scene->shapes);
	if (scene->sem_loading != NULL)
		sem_close(scene->sem_loading);
	sem_unlink("/loading");
	free(scene);
}

void	free_texture(t_shape *shape, t_color **texture)
{
	int	i;

	if (texture == NULL)
		return ;
	i = 0;
	while (i < shape->tex_height)
	{
		free(texture[i]);
		i++;
	}
	free(texture);
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
