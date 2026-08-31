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

t_request   *heap_top(t_heap *heap)
{
    if (heap->size == 0)
        return NULL;
    return &heap->data[0];
}

void    heap_pop(t_heap *heap)
{
    heap->data[0] = heap->data[-1];
    size--;
    shift_down(heap, 0);
}

//void    shift_down(t_heap *heap, int index)
//{
//    int smallest;
//
//    smallest = 
//}
    