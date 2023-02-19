/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_bar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 17:33:31 by hsarhan           #+#    #+#             */
/*   Updated: 2023/02/19 22:32:36 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	update_loading_bar(t_worker *worker, int *line_counter)
{
	if (worker->scene->settings.edit_mode == false
		&& (*line_counter == (worker->y_end - worker->y_start) / 15))
	{
		sem_post(worker->scene->sem_loading);
		*line_counter = 0;
	}
}

void	display_loading_bar(t_scene *scene)
{
	int	sem_counter;
	int	load;

	sem_counter = 0;
	if (scene->settings.edit_mode == false)
	{
		ft_putstr_fd("[", 1);
		load = 0;
		while (load < NUM_THREADS * 5)
		{
			ft_putstr_fd(RED".", 1);
			load++;
		}
		ft_putstr_fd(RESET"]", 1);
		ft_putstr_fd("\r", 1);
		ft_putstr_fd("[", 1);
		while (sem_counter < NUM_THREADS * 5)
		{
			ft_putstr_fd(GREEN"=", 1);
			sem_wait(scene->sem_loading);
			sem_counter++;
		}
		ft_putstr_fd(RESET"]\n", 1);
	}
}

void	show_light_banner(t_scene *scene)
{
	mlx_string_put(scene->disp->mlx, scene->disp->win,
		scene->settings.disp_w * .9, 20,
		0xffffff, "LIGHT MODE");
}
