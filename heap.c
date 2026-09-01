/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josamba- <josamba-@student.42belgium.be>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-04-29 08:30:31 by josamba-          #+#    #+#             */
/*   Updated: 2026-04-29 08:30:31 by josamba-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_request	*heap_top(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (&heap->data[0]);
}

void	shift_down(t_heap *heap, int index)
{
	int	min;

	min = smallest_child(heap, index);
	if (min == index)
		return ;
	swap_request(&heap->data[index], &heap->data[min]);
	shift_down(heap, min);
}

void	heap_pop(t_heap *heap)
{
	heap->data[0] = heap->data[-1];
	size--;
	shift_down(heap, 0);
}

void	shift_up(t_heap *heap, int index)
{
	int	parent;

	if (index == 0)
		return ;
	parent = (i - 1) / 2;
	if (!higher_priority(heap->data[i], heap->data[parent]))
		return ;
	swap_request(&heap->data[i], &heap->data[parent]);
	shift_up(heap, parent);
}

int	heap_push(t_heap *heap, t_request request)
{
	if (heap->size == heap->capacity)
		return (0);
	heap->data[heap->size] = request;
	heap->size++;
	shift_up(heap, heap->size - 1)
	return (1);
}
