#include "shell.h"

/**
 * add_node - adds a node to the beginning of a linked list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
linked_list *add_node(linked_list **head, const char *str, int num)
{
	linked_list *new_head;

	if (!head)
		return (NULL);
	new_head = malloc(sizeof(linked_list));
	if (!new_head)
		return (NULL);
	_memset((void *)new_head, 0, sizeof(linked_list));
	new_head->num = num;
	if (str)
	{
		new_head->str = _strdup(str);
		if (!new_head->str)
		{
			free(new_head);
			return (NULL);
		}
	}
	new_head->next = *head;
	*head = new_head;
	return (new_head);
}

/**
 * add_node_end - adds a new node to the end of a linked list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
linked_list *add_node_end(linked_list **head, const char *str, int num)
{
	linked_list *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(linked_list));
	if (!new_node)
		return (NULL);
	_memset((void *)new_node, 0, sizeof(linked_list));
	new_node->num = num;
	if (str)
	{
		new_node->str = _strdup(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new_node;
	}
	else
		*head = new_node;
	return (new_node);
}

/**
 * rm_node - removes a node at a specific index from a linked list
 * @head: address of pointer to first node
 * @index: index of node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int rm_node(linked_list **head, unsigned int index)
{
	linked_list *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}
	node = *head;
	while (node)
	{
		if (i == index)
		{
			prev_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		prev_node = node;
		node = node->next;
	}
	return (0);
}

/**
 * free_list -  frees all the memory associated with a linked list
 * @head_ptr: address of pointer to head node
 *
 * Return: void
 */
void free_list(linked_list **head_ptr)
{
	linked_list *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;
	head = *head_ptr;
	node = head;
	while (node)
	{
		next_node = node->next;
		free(node->str);
		free(node);
		node = next_node;
	}
	*head_ptr = NULL;
}
