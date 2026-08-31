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