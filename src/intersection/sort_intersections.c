/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_intersections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 10:03:33 by hsarhan           #+#    #+#             */
/*   Updated: 2022/12/26 10:17:22 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static int	itx_cmp(const void *s1, const void *s2)
{
	t_intersection	*no1;
	t_intersection	*no2;

	no1 = (t_intersection *) s1;
	no2 = (t_intersection *) s2;
	if (no1->time < no2->time)
		return (-1);
	else if (no1->time > no2->time)
		return (1);
	return (0);
}

// static void	swap_intersections(t_intersect *itx1, t_intersect *itx2)
// {
// 	t_intersect	tmp;

// 	tmp = *itx1;
// 	*itx1 = *itx2;
// 	*itx2 = tmp;
// }

bool	is_sorted(t_intersections *arr)
{
	int	n;

	n = arr->count;
	while (--n >= 1)
	{
		if (arr->arr[n].time < arr->arr[n - 1].time)
			return (false);
	}
	return (true);
}

void	sort_intersections(t_intersections *arr)
{
	qsort(arr, arr->count, sizeof(t_intersection), &itx_cmp);
}
