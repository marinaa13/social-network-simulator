#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "posts.h"
#include "users.h"

// Function to create a doubly linked list
doubly_linked_list_t*
dll_create(unsigned int data_size)
{
	doubly_linked_list_t *list = malloc(sizeof(*list));
	DIE(!list, "Malloc failed");

	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	list->data_size = data_size;

	return list;
}

// Function to get the nth node from the list
dll_node_t*
dll_get_nth_node(doubly_linked_list_t *list, unsigned int n)
{
	dll_node_t *aux = list->head;

	if (n >= list->size)
		n = n % list->size;

	if (list->size == 0)
		return NULL;

	for (unsigned int i = 0; i < n; i++)
		aux = aux->next;

	return aux;
}

// Function to add a new node to the list on the nth position
void dll_add_nth_node(doubly_linked_list_t *list, unsigned int n,
					  const void *new_data)
{
	dll_node_t *curr, *new_node;

	if (!list)
		return;

	if (n > list->size)
		n = list->size;

	new_node = malloc(sizeof(*new_node));
	DIE(!new_node, "Failed malloc");
	new_node->data = (void *)new_data;

	if (list->size == 0) {
		new_node->next = NULL;
		new_node->prev = NULL;
		list->head = new_node;
		list->tail = new_node;
	} else if (n == 0) {
		new_node->next = list->head;
		list->head->prev = new_node;
		new_node->prev = NULL;
		list->head = new_node;
	} else if (n == list->size) {
		new_node->next = NULL;
		new_node->prev = list->tail;
		list->tail->next = new_node;
		list->tail = new_node;
	} else {
		curr = dll_get_nth_node(list, n - 1);

		new_node->next = curr->next;
		new_node->prev = curr;
		curr->next = new_node;

		new_node->next->prev = new_node;
	}

	list->size++;
}

// Function to remove the nth node from the list
dll_node_t *dll_remove_nth_node(doubly_linked_list_t *list, unsigned int n)
{
	dll_node_t *removed, *before_removed;

	if (!list || !list->head)
		return NULL;

	if (n > list->size - 1)
		n = list->size - 1;

	if (n == 0) {		// se schimba head-ul
		removed = list->head;
		list->head = list->head->next;
		if (list->head) // check if list is not empty after removal
			list->head->prev = NULL;
	} else if (n == list->size - 1) {	// se schimba tail-ul
		removed = list->tail;
		list->tail = list->tail->prev;
		if (list->tail) // check if list is not empty after removal
			list->tail->next = NULL;
	} else {
		removed = dll_get_nth_node(list, n);
		before_removed = dll_get_nth_node(list, n - 1);
		before_removed->next = removed->next;

		removed->next->prev = before_removed;
	}

	list->size--;
	return removed;
}

// Function to remove a given node from the list
void dll_remove_node(doubly_linked_list_t *list, dll_node_t *node)
{
	dll_node_t *prev, *curr;

	if (!list || !node)
		return;

	if (node == list->head) {
		list->head = node->next;
		if (list->head)
			list->head->prev = NULL;
		free(node);
		list->size--;
		return;
	}
	curr = list->head;
	while (curr != node) {
		prev = curr;
		curr = curr->next;
	}
	if (curr->next)
		curr->next->prev = prev;
	else if (curr == list->tail)
		list->tail = prev;
	prev->next = curr->next;

	free(node);
	list->size--;
}

// Function to compare 2 integers from the posts list
int compare_function_ints_list(void *a, void *b)
{
	int int_a = *((int *)a);
	tree_node_t *tree_root = (tree_node_t *)b;
	post_t *post = (post_t *)tree_root->value;
	int int_b = post->id;

	if (int_a == int_b)
		return 0;
	else if (int_a < int_b)
		return -1;
	else
		return 1;
}

// Function to compare 2 integers from a post's list of reposts
int compare_function_ints_tree(void *a, void *b)
{
	int int_a = *((int *)a);
	post_t *post = (post_t *)b;
	int int_b = post->id;

	if (int_a == int_b)
		return 0;
	else if (int_a < int_b)
		return -1;
	else
		return 1;
}

// Function to get a node from the list by its data
dll_node_t *dll_get_node(doubly_linked_list_t *list, void *data,
						 int (*compare_function)(void *, void *))
{
	dll_node_t *curr;

	if (!list)
		return NULL;

	curr = list->head;
	while (curr) {
		if (compare_function(data, curr->data) == 0)
			return curr;
		curr = curr->next;
	}

	return NULL;
}

// Function to get the size of the list
unsigned int dll_get_size(doubly_linked_list_t *list)
{
	if (!list)
		return -1;

	return list->size;
}

// Function to free the data of a post struct
void free_struct_data(void *data)
{
	post_t *post = (post_t *)data;

	if (post->title)
		free(post->title);

	if (post->users_liked)
		free(post->users_liked);

	free(post);
}

// Function to free the list
void dll_free(doubly_linked_list_t **pp_list,
			  void (*free_tree)(tree_node_t *, void (*)(void *)))
{
	dll_node_t *curr;

	if (!pp_list || !*pp_list)
		return;

	while (dll_get_size(*pp_list) > 0) {
		curr = dll_remove_nth_node(*pp_list, 0);

		free_tree((tree_node_t *)curr->data, free_struct_data);
		curr->data = NULL;

		free(curr);
		curr = NULL;
	}

	free(*pp_list);
	*pp_list = NULL;
}

// Function to create a tree node
tree_node_t *create_node(void *value)
{
	tree_node_t *node = malloc(sizeof(*node));
	DIE(!node, "Failed node malloc");

	node->value = value;
	node->children = NULL;
	node->n_children = 0;

	return node;
}

// Function to create a tree(by creating the root)
tree_node_t *create_tree(void *root_content)
{
	tree_node_t *root_node = create_node(root_content);
	return root_node;
}

// Recursive function to get a tree node by its value
tree_node_t *get_tree_node(tree_node_t *node, void *value,
						   int (*compare_function)(void *, void *))
{
	if (compare_function(value, node->value) == 0)
		return node;

	for (int i = 0; i < node->n_children; i++) {
		tree_node_t *found = get_tree_node(node->children[i], value,
										   compare_function);

		if (found)
			return found;
	}

	return NULL;
}

// Recursive function to get the parent of a given tree node
tree_node_t *get_tree_node_parent(tree_node_t *node, void *value,
								  int (*compare_function)(void *, void *))
{
	for (int i = 0; i < node->n_children; i++) {
		if (compare_function(value, node->children[i]->value) == 0)
			return node;

		tree_node_t *found = get_tree_node_parent(node->children[i], value,
												  compare_function);

		if (found)
			return found;
	}

	return NULL;
}

// Function to add a child to a tree node
void add_child(tree_node_t *parent, void *value)
{
	// Create the new child node
	tree_node_t *child = create_node(value);

	parent->n_children++;
	tree_node_t **aux = realloc(parent->children, (parent->n_children
								* sizeof(*parent->children)));
	DIE(!aux, "Failed realloc");
	parent->children = aux;

	// Add the child
	parent->children[parent->n_children - 1] = child;
}

// Recursive function to free a tree
void free_tree(tree_node_t *node, void (*free_struct_data)(void *))
{
	if (!node)
		return;

	for (int i = 0; i < node->n_children; i++)
		free_tree(node->children[i], free_struct_data);

	free(node->children);
	free_struct_data(node->value);
	free(node);
}

// Recursive function to get all the parents of a repost (in the parents array)
void get_all_parents_ids(tree_node_t *root, tree_node_t *node, int **parents,
						 int *n)
{
	if (!node)
		return;

	post_t *post = (post_t *)node->value;
	tree_node_t *parent = get_tree_node_parent(root, &post->id,
											   compare_function_ints_tree);

	if (!parent)
		return;

	int *aux = realloc((*parents), ((*n) + 1) * sizeof(int));
	DIE(!aux, "Failed realloc");
	(*parents) = aux;
	post_t *parent_post = (post_t *)parent->value;

	// Storing the id of each parent
	(*parents)[(*n)] = parent_post->id;
	(*n)++;

	get_all_parents_ids(root, parent, parents, n);
}

// Function to compare the parents arrays of 2 reposts
// and return the first common repost (and 0 if there is none)
int compare_parents(int *parents1, int n1, int *parents2, int n2)
{
	n1--;
	n2--;
	int i = 0, j = 0;

	while (i <= n1 && j <= n2) {
		while (n1 - i >= n2 - j && i <= n1) {
			if (parents1[i] == parents2[j])
				return parents1[i];
			i++;
		}

		while (n1 - i < n2 - j && j <= n2) {
			if (parents1[i] == parents2[j])
				return parents1[j];
			j++;
		}
	}

	return 0;
}

// Recursive function to print all the children of a post tree/repost subtree
void print_children(tree_node_t *node)
{
	if (!node)
		return;

	post_t *post = (post_t *)node->value;
	char *user_name = get_user_name(post->user_id);
	printf("Repost #%d by %s\n", post->id, user_name);

	for (int i = 0; i < node->n_children; i++)
		print_children(node->children[i]);
}

// Recursive function to get max number of likes in a post tree/repost subtree
void get_max_likes(tree_node_t *node, int *max_likes, int *max_id)
{
	if (!node)
		return;

	post_t *post = (post_t *)node->value;
	if (post->num_likes > *max_likes) {
		*max_likes = post->num_likes;
		*max_id = post->id;
	}

	for (int i = 0; i < node->n_children; i++)
		get_max_likes(node->children[i], max_likes, max_id);
}

// Function to add a new post to the list
void create_function(int *num_posts, doubly_linked_list_t *posts_list)
{
	// Parsing the input
	char *user_name = strtok(NULL, "\n ");
	char *post_title = strtok(NULL, "\n");
	int user_id = get_user_id(user_name);

	// Creating the new post
	post_t *new_post = malloc(sizeof(*new_post));
	DIE(!new_post, "Failed malloc");
	new_post->user_id = user_id;
	new_post->title = strdup(post_title);
	DIE(!new_post->title, "Failed strdup");
	new_post->id = (*num_posts);
	new_post->num_likes = 0;
	new_post->users_liked = NULL;

	// Adding the new post to the list
	tree_node_t *new_tree_root = create_tree(new_post);
	dll_add_nth_node(posts_list, posts_list->size, new_tree_root);
	printf("Created %s for %s\n", new_post->title, user_name);

	(*num_posts)++;
}

// Function to add a new repost to a post
void repost_function(int *num_posts, doubly_linked_list_t *posts_list)
{
	// Parsing the input
	char *user_name = strtok(NULL, "\n ");
	int post_id = atoi(strtok(NULL, "\n "));
	char *aux_str = strtok(NULL, "\n ");
	int repost_id;
	if (!aux_str)
		repost_id = -1;
	else
		repost_id = atoi(aux_str);
	int user_id = get_user_id(user_name);

	// Creating the new repost
	post_t *new_post = malloc(sizeof(*new_post));
	DIE(!new_post, "Failed malloc");
	new_post->user_id = user_id;
	new_post->title = NULL;
	new_post->id = (*num_posts);
	new_post->num_likes = 0;
	new_post->users_liked = NULL;

	// Adding the new repost to the list
	dll_node_t *post_node = dll_get_node(posts_list, &post_id,
									   compare_function_ints_list);
	tree_node_t *repost_tree_root = (tree_node_t *)(post_node->data);

	if (repost_id != -1) {
		tree_node_t *repost_node = get_tree_node(repost_tree_root,
								   &repost_id, compare_function_ints_tree);
		// Adding the new repost to the repost
		add_child(repost_node, new_post);
	} else {
		// Adding the new repost to the original post
		add_child(repost_tree_root, new_post);
	}

	printf("Created repost #%d for %s\n", (*num_posts), user_name);
	(*num_posts)++;
}

// Function to find the first common repost of 2 reposts
void common_repost_function(doubly_linked_list_t *posts_list)
{
	// Parsing the input
	int post_id = atoi(strtok(NULL, "\n "));
	int repost_id1 = atoi(strtok(NULL, "\n "));
	int repost_id2 = atoi(strtok(NULL, "\n "));

	// Getting the 2 reposts
	dll_node_t *post_node = dll_get_node(posts_list, &post_id,
									   compare_function_ints_list);
	tree_node_t *post_tree_root = (tree_node_t *)post_node->data;

	tree_node_t *repost_node1 = get_tree_node(post_tree_root, &repost_id1,
											  compare_function_ints_tree);
	tree_node_t *repost_node2 = get_tree_node(post_tree_root, &repost_id2,
											  compare_function_ints_tree);

	// Getting the parents arrays of the 2 reposts
	int *parents1 = malloc(sizeof(int));
	DIE(!parents1, "Failed malloc");
	int *parents2 = malloc(sizeof(int));
	DIE(!parents2, "Failed malloc");
	int n1 = 1, n2 = 1;

	parents1[0] = repost_id1;
	parents2[0] = repost_id2;

	get_all_parents_ids(post_tree_root, repost_node1, &parents1, &n1);
	get_all_parents_ids(post_tree_root, repost_node2, &parents2, &n2);

	// Comparing the parents arrays to find the common repost
	int common_parent = compare_parents(parents1, n1, parents2, n2);
	printf("The first common repost of %d and %d is %d\n", repost_id1,
		   repost_id2, common_parent);

	free(parents1);
	free(parents2);
}

// Function to like or unlike a post or a repost
void like_function(doubly_linked_list_t *posts_list)
{
	// Parsing the input
	char *user_name = strtok(NULL, "\n ");
	int post_id = atoi(strtok(NULL, "\n "));
	char *aux_str = strtok(NULL, "\n ");
	int repost_id;
	if (!aux_str)
		repost_id = -1;
	else
		repost_id = atoi(aux_str);
	int user_id = get_user_id(user_name);

	// Getting the post
	dll_node_t *post_node = dll_get_node(posts_list, &post_id,
									   compare_function_ints_list);
	tree_node_t *post_tree_root = (tree_node_t *)post_node->data;
	post_t *post_aux = (post_t *)post_tree_root->value;
	char *title = post_aux->title;

	if (repost_id != -1) {
		// Getting the repost
		tree_node_t *repost_node = get_tree_node(post_tree_root,
								   &repost_id, compare_function_ints_tree);
		post_tree_root = repost_node;
	}

	// Liking or unliking the post/repost
	post_t *post = (post_t *)post_tree_root->value;
	if (post->num_likes == 0) {
		// If the post/repost has no likes, I create the array of users
		// and add the current one
		post->users_liked = malloc(sizeof(int));
		DIE(!post->users_liked, "Failed malloc");
		post->users_liked[0] = user_id;
		post->num_likes++;

		if (repost_id == -1)
			printf("User %s liked post %s\n", user_name, title);
		else
			printf("User %s liked repost %s\n", user_name, title);
	} else {
		// If the post/repost has likes, I search for the user in the array
		int poz = -1;
		for (int i = 0; i < post->num_likes; i++)
			if (post->users_liked[i] == user_id) {
				poz = i;
				break;
			}

		// If the user isn,t in the array, I add him and increase the likes
		if (poz == -1) {
			post->num_likes++;
			int *aux = realloc(post->users_liked,
							   post->num_likes * sizeof(int));
			DIE(!aux, "Failed realloc");
			post->users_liked = aux;
			post->users_liked[post->num_likes - 1] = user_id;

			if (repost_id == -1)
				printf("User %s liked post %s\n", user_name, title);
			else
				printf("User %s liked repost %s\n", user_name, title);
		} else {
			// If the user is in the array, I remove him by shifting the
			// remaining users and decrease the number of likes
			for (int i = poz; i < post->num_likes - 1; i++)
				post->users_liked[i] = post->users_liked[i + 1];
			post->num_likes--;

			int *aux = realloc(post->users_liked,
							   post->num_likes * sizeof(int));
			DIE(!aux && post->num_likes, "Failed realloc");
			post->users_liked = aux;

			if (repost_id == -1)
				printf("User %s unliked post %s\n", user_name, title);
			else
				printf("User %s unliked repost %s\n", user_name, title);
		}
	}
}

// Function to determine if a post got ratio'd
void ratio_function(doubly_linked_list_t *posts_list)
{
	// Parsing the input
	int post_id = atoi(strtok(NULL, "\n "));

	// Getting the post
	dll_node_t *post_node = dll_get_node(posts_list, &post_id,
									   compare_function_ints_list);
	tree_node_t *post_tree_root = (tree_node_t *)post_node->data;

	// Getting the repost with the max likes
	post_t *post = (post_t *)post_tree_root->value;
	int max_likes = post->num_likes;
	int max_id = post->id;
	get_max_likes(post_tree_root, &max_likes, &max_id);

	// If the original post has the most likes, print that
	if (max_likes == post->num_likes)
		printf("The original post is the highest rated\n");
	else
		printf("Post %d got ratio'd by repost %d\n", post->id, max_id);
}

// Function to delete a post or a repost
void delete_function(doubly_linked_list_t *posts_list)
{
	// Parsing the input
	int post_id = atoi(strtok(NULL, "\n "));
	char *aux_str = strtok(NULL, "\n ");
	int repost_id;
	if (!aux_str)
		repost_id = -1;
	else
		repost_id = atoi(aux_str);

	// Getting the post
	dll_node_t *post_node = dll_get_node(posts_list, &post_id,
									   compare_function_ints_list);
	tree_node_t *post_tree_root = (tree_node_t *)post_node->data;
	post_t *post = (post_t *)post_tree_root->value;
	char *title = post->title;

	// If I have to delete a post, I free the repost tree and remove the post
	if (repost_id == -1) {
		free_tree(post_tree_root, free_struct_data);
		dll_remove_node(posts_list, post_node);
		printf("Deleted %s\n", title);
	} else {
		// Getting the repost
		tree_node_t *repost_node = get_tree_node(post_tree_root,
								   &repost_id, compare_function_ints_tree);

		// Getting the repost's parent so that I can
		// remove the subtree starting from the repost
		tree_node_t *parent_node = get_tree_node_parent(post_tree_root,
								   &repost_id, compare_function_ints_tree);
		parent_node->n_children--;

		// Shifting the remaining reposts to the left
		for (int i = 0; i < parent_node->n_children; i++) {
			if (parent_node->children[i] == repost_node) {
				for (int j = i; j < parent_node->n_children; j++)
					parent_node->children[j] = parent_node->children[j + 1];
				break;
			}
		}

		// Deleting the repost and the repost's subtree
		free_tree(repost_node, free_struct_data);

		printf("Deleted repost #%d of post %s\n", repost_id, title);
	}
}

// Function to get the likes of a post or a repost
void get_likes_function(doubly_linked_list_t *posts_list)
{
	// Parsing the input
	int post_id = atoi(strtok(NULL, "\n "));
	char *aux_str = strtok(NULL, "\n ");
	int repost_id;
	if (!aux_str)
		repost_id = -1;
	else
		repost_id = atoi(aux_str);

	// Getting the post
	dll_node_t *post_node = dll_get_node(posts_list, &post_id,
									   compare_function_ints_list);
	tree_node_t *post_tree_root = (tree_node_t *)post_node->data;

	if (repost_id != -1) {
		// Getting the repost and printing the number of likes
		tree_node_t *repost_node = get_tree_node(post_tree_root,
								   &repost_id, compare_function_ints_tree);
		post_t *repost = (post_t *)repost_node->value;

		printf("Repost #%d has %d likes\n", repost_id, repost->num_likes);
	} else {
		// Printing the number of likes of the post
		post_t *post = (post_t *)post_tree_root->value;

		printf("Post %s has %d likes\n", post->title, post->num_likes);
	}
}

// Function to get the reposts of a post
void get_reposts_function(doubly_linked_list_t *posts_list)
{
	// Parsing the input
	int post_id = atoi(strtok(NULL, "\n "));
	char *aux_str = strtok(NULL, "\n ");
	int repost_id;
	if (!aux_str)
		repost_id = -1;
	else
		repost_id = atoi(aux_str);

	// Getting the post
	dll_node_t *repost_tree_root = dll_get_node(posts_list, &post_id,
											  compare_function_ints_list);
	tree_node_t *post_node = (tree_node_t *)repost_tree_root->data;

	// Printing the post
	if (repost_id == -1) {
		post_t *post = (post_t *)post_node->value;
		char *user_name = get_user_name(post->user_id);
		printf("%s - Post by %s\n", post->title, user_name);
	} else {
		// Getting the repost and printing it
		tree_node_t *repost_node = get_tree_node(post_node,
								   &repost_id, compare_function_ints_tree);
		post_t *repost = (post_t *)repost_node->value;
		char *user_name = get_user_name(repost->user_id);
		printf("Repost #%d by %s\n", repost->id, user_name);
		post_node = repost_node;
	}

	// Printing all of the reposts of the post/repost
	for (int i = 0; i < post_node->n_children; i++)
		print_children(post_node->children[i]);
}

void handle_input_posts(char *input, int *num_posts,
						doubly_linked_list_t *posts_list)
{
	char *commands = strdup(input);
	DIE(!commands, "Failed strdup");
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	// Calling the respective function for each command
	if (!strcmp(cmd, "create"))
		create_function(num_posts, posts_list);
	else if (!strcmp(cmd, "repost"))
		repost_function(num_posts, posts_list);
	else if (!strcmp(cmd, "common-repost"))
		common_repost_function(posts_list);
	else if (!strcmp(cmd, "like"))
		like_function(posts_list);
	else if (!strcmp(cmd, "ratio"))
		ratio_function(posts_list);
	else if (!strcmp(cmd, "delete"))
		delete_function(posts_list);
	else if (!strcmp(cmd, "get-likes"))
		get_likes_function(posts_list);
	else if (!strcmp(cmd, "get-reposts"))
		get_reposts_function(posts_list);

	free(commands);
}
