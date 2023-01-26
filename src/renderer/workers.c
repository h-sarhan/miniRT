/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   workers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:36:01 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/26 20:09:30 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	init_workers(t_worker *workers, t_scene *scene)
{
	unsigned int	i;

	i = 0;
	while (i < NUM_THREADS)
	{
		workers[i].height = scene->settings.render_h;
		workers[i].width = scene->settings.render_w;
		workers[i].addr = scene->disp->render_addr;
		if (scene->settings.edit_mode == true)
		{
			workers[i].height = scene->settings.edit_h;
			workers[i].width = scene->settings.edit_w;
			workers[i].addr = scene->disp->edit_addr;
		}
		workers[i].max_workers = NUM_THREADS;
		workers[i].worker_id = i;
		workers[i].scene = scene;
		workers[i].y_start = (workers[i].height / (float)NUM_THREADS) * i;
		workers[i].y_end = (workers[i].height / (float)NUM_THREADS) * (i + 1);
		workers[i].y_scale_start = (scene->settings.disp_h \
			/ (float)NUM_THREADS) * i;
		workers[i].y_scale_end = (scene->settings.disp_h \
			/ (float)NUM_THREADS) * (i + 1);
		i++;
	}
}

void	run_workers(t_worker *workers, t_scene *scene, bool loading, void *func)
{
	pthread_t	threads[NUM_THREADS];
	int			i;

	i = -1;
	while (++i < NUM_THREADS)
		pthread_create(&threads[i], NULL, func, &workers[i]);
	if (loading == true)
		display_loading_bar(scene);
	i = -1;
	while (++i < NUM_THREADS)
		pthread_join(threads[i], NULL);
}
