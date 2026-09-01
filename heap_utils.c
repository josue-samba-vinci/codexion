int higher_priority(t_request *a, t_request *b)
{
	if (a->key != b->key)
		return (a->key < b->key);
	return (a.coder_id < b.coder_id);
}

void	swap_request(t_request *a, t_request *b)
{
	t_request	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int smallest_child(t_heap *heap, int index)
{
	int	min;
	int	left;
	int	right;

	min = index;
	left = 2 * i + 1;
	right = 2 * i + 2;
	if (left < heap->size
			&& has_priority(heap->data[left], heap->data[min]))
			min = left;
	if (left < heap->size
			&& has_priority(heap->data[right], heap->data[min]))
			min = right;
	return (min);
}

void	sift_up(t_heap *heap, int i)
{
	int	parent;

	if (i == 0)
		return ;
	parent = (i - 1) / 2;
	if (!request_is_before(heap->data[i], heap->data[parent]))
		return ;
	swap_request(&heap->data[i], &heap->data[parent]);
	sift_up(heap, parent);
}

int	heap_push(t_heap *heap, t_request req)
{
	if (heap->size == heap->capacity)
		return (0);
	heap->data[heap->size] = req;
	heap->size++;
	sift_up(heap, heap->size - 1);
	return (1);
}