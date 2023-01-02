/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 13:01:06 by hsarhan           #+#    #+#             */
/*   Updated: 2023/01/02 15:16:52 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	remove_element_at(t_shape **containers, int idx, int *count)
{
	int	i;

	for(i = idx; i < *count - 1; i++)
	{
		containers[i] = containers[i + 1];
	}
	*count -= 1;
}

bool	includes_shape(t_shape **containers, t_shape *shape, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (containers[i]->id == shape->id)
		{
			return (true);
		}
		i++;
	}
	return (false);
}

void	get_iors(t_intersection *intersection, t_intersections *xs)
{
	t_shape	*containers[SHAPE_MAX];
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (i < xs->count)
	{
		if (fabs(xs->arr[i].time - intersection->time) < 0.01 && xs->arr[i].shape == intersection->shape)
		{
			if (count == 0)
				intersection->n1 = 1.0;
			else
				intersection->n1 = containers[count - 1]->props.ior;
		}
		if (includes_shape(containers, xs->arr[i].shape, count) == true)
		{
			remove_element_at(containers, i, &count);
		}
		else
		{
			containers[count] = xs->arr[i].shape;
			count++;
		}
		if (fabs(xs->arr[i].time - intersection->time) < 0.01 && xs->arr[i].shape == intersection->shape)
		{
			if (count == 0)
				intersection->n2 = 1.0;
			else
				intersection->n2 = containers[count - 1]->props.ior;
			break;
		}
		i++;
	}
}