#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "posts.h"
#include "friends.h"
#include "feed.h"

int verify_friend(matrix_graph_t *graph, int id_1, int id_2)
{
	return mg_has_edge(graph, id_1, id_2);
}

// Function that prints the reposts made by the user with the id user_id
void get_all_reposts(tree_node_t *root, int user_id, int post_id,
					 char *post_title)
{
	if (!root)
		return;

	post_t *post = (post_t *)root->value;
	if (post->user_id == user_id && post->id != post_id)
		printf("Reposted: %s\n", post_title);

	for (int i = 0; i < root->n_children; i++)
		get_all_reposts(root->children[i], user_id, post_id, post_title);
}

// Function that prints the friends that reposted the user
// with the id user_id's post
void get_all_friends_reposts(tree_node_t *root, matrix_graph_t *graph,
							 int user_id)
{
	if (!root)
		return;

	post_t *post = (post_t *)root->value;
	char *user_name = get_user_name(post->user_id);

	if (verify_friend(graph, user_id, post->user_id))
		printf("%s\n", user_name);

	for (int i = 0; i < root->n_children; i++)
		get_all_friends_reposts(root->children[i], graph, user_id);
}

// Function that prints the feed of a user
// (the most recent n posts made by the user and his friends)
void feed_function(doubly_linked_list_t *posts_list, matrix_graph_t *graph)
{
	// Parse the input
	char *user_name = strtok(NULL, "\n ");
	int feed_size = atoi(strtok(NULL, "\n "));
	int user_id = get_user_id(user_name);

	// Iterate through the posts list starting from the most recent one
	dll_node_t *post_node = posts_list->tail;
	int counter = 0;

	while (counter < feed_size && post_node) {
		tree_node_t *post_tree_root = (tree_node_t *)post_node->data;
		post_t *post = (post_t *)post_tree_root->value;

		// Check if the post was made by the user or by a friend
		if (post->user_id == user_id ||
			verify_friend(graph, user_id, post->user_id)) {
			char *user_name = get_user_name(post->user_id);
			counter++;

			printf("%s: %s\n", user_name, post->title);
		}

		post_node = post_node->prev;
	}
}

// Function that prints the posts and reposts of a user
void view_profile_function(doubly_linked_list_t *posts_list)
{
	// Parse the input
	char *user_name = strtok(NULL, "\n ");
	int user_id = get_user_id(user_name);

	dll_node_t *post_node = posts_list->head;

	// Iterate through the posts list and print the posts made by the user
	// in chronological order
	while (post_node) {
		tree_node_t *post_tree_root = (tree_node_t *)post_node->data;
		post_t *post = (post_t *)post_tree_root->value;

		if (post->user_id == user_id)
			printf("Posted: %s\n", post->title);

		post_node = post_node->next;
	}

	post_node = posts_list->head;

	// Iterate thorugh every post's repost tree and print the reposts
	// made by the user
	while (post_node) {
		tree_node_t *post_tree_root = (tree_node_t *)post_node->data;
		post_t *post = (post_t *)post_tree_root->value;

		get_all_reposts(post_tree_root, user_id, post->id, post->title);

		post_node = post_node->next;
	}
}

// Function that prints the friends that reposted a user's post
void friends_repost_function(doubly_linked_list_t *posts_list,
							 matrix_graph_t *graph)
{
	// Parse the input
	char *user_name = strtok(NULL, "\n ");
	int post_id = atoi(strtok(NULL, "\n "));
	int user_id = get_user_id(user_name);

	// Get the post
	dll_node_t *post_node = dll_get_node(posts_list, &post_id,
										 compare_function_ints_list);
	tree_node_t *post_tree_root = (tree_node_t *)post_node->data;

	// Get all the friends that reposted the post
	get_all_friends_reposts(post_tree_root, graph, user_id);
}

int is_empty(int *arr, int nodes)
{
	for (int i = 0; i < nodes; i++) {
		if (arr[i] == 1)
			return 0;
	}
	return 1;
}

void bk_rec_calc_max(matrix_graph_t *graph, int *R, int *P, int *X,
					 int nodes, int *maxi)
{
	if (is_empty(P, nodes) && is_empty(X, nodes)) {
		int count = 0;
		for (int i = 0; i < nodes; i++) {
			if (R[i] == 1)
				count++;
		}
		if (count > *maxi)
			*maxi = count;
	} else {
		for (int i = 0; i < nodes; i++) {
			if (P[i] == 1) {
				int *rnew = (int *)malloc(nodes * sizeof(int));
				int *pnew = (int *)malloc(nodes * sizeof(int));
				int *xnew = (int *)malloc(nodes * sizeof(int));
				memcpy(rnew, R, nodes * sizeof(int));
				memcpy(pnew, P, nodes * sizeof(int));
				memcpy(xnew, X, nodes * sizeof(int));
				rnew[i] = 1;
				for (int j = 0; j < nodes; j++) {
					if (graph->matrix[i][j] == 0) {
						pnew[j] = 0;
						xnew[j] = 0;
					}
				}
				bk_rec_calc_max(graph, rnew, pnew, xnew, nodes, maxi);
				P[i] = 0;
				X[i] = 1;
				free(rnew);
				free(pnew);
				free(xnew);
			}
		}
	}
}

void bk_rec_af_max(matrix_graph_t *graph, int *R, int *P, int *X,
				   int nodes, int maxi)
{
	if (is_empty(P, nodes) && is_empty(X, nodes)) {
		int count = 0;
		for (int i = 0; i < nodes; i++)
			if (R[i] == 1)
				count++;
		if (count == maxi)
			for (int i = 0; i < nodes; i++)
				if (R[i] == 1)
					printf("%s\n", get_user_name(i));
	} else {
		for (int i = 0; i < nodes; i++) {
			if (P[i] == 1) {
				int *rnew = (int *)malloc(nodes * sizeof(int));
				int *pnew = (int *)malloc(nodes * sizeof(int));
				int *xnew = (int *)malloc(nodes * sizeof(int));
				memcpy(rnew, R, nodes * sizeof(int));
				memcpy(pnew, P, nodes * sizeof(int));
				memcpy(xnew, X, nodes * sizeof(int));
				rnew[i] = 1;
				for (int j = 0; j < nodes; j++) {
					if (graph->matrix[i][j] == 0) {
						pnew[j] = 0;
						xnew[j] = 0;
					}
				}
				bk_rec_af_max(graph, rnew, pnew, xnew, nodes, maxi);
				P[i] = 0;
				X[i] = 1;
				free(rnew);
				free(pnew);
				free(xnew);
			}
		}
	}
}

void bk(matrix_graph_t *graph, int start_node)
{
	int *R = (int *)calloc(graph->nodes, sizeof(int));
	int *P = (int *)calloc(graph->nodes, sizeof(int));
	int *X = (int *)calloc(graph->nodes, sizeof(int));
	for (int i = 0; i < graph->nodes; i++) {
		if (mg_has_edge(graph, start_node, i) == 1)
			P[i] = 1;
		else
			P[i] = 0;
		R[i] = 0;
		X[i] = 0;
	}
	R[start_node] = 1;
	printf("The closest friend group of %s is:\n", get_user_name(start_node));
	int maxi = 0;
	bk_rec_calc_max(graph, R, P, X, graph->nodes, &maxi);
	free(R);
	free(P);
	free(X);
	R = (int *)calloc(graph->nodes, sizeof(int));
	P = (int *)calloc(graph->nodes, sizeof(int));
	X = (int *)calloc(graph->nodes, sizeof(int));
	for (int i = 0; i < graph->nodes; i++) {
		if (mg_has_edge(graph, start_node, i) == 1)
			P[i] = 1;
		else
			P[i] = 0;
		R[i] = 0;
		X[i] = 0;
	}
	R[start_node] = 1;
	bk_rec_af_max(graph, R, P, X, graph->nodes, maxi);
	free(R);
	free(P);
	free(X);
}

void handle_input_feed(char *input, matrix_graph_t *graph,
					   doubly_linked_list_t *posts_list)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "feed"))
		feed_function(posts_list, graph);
	else if (!strcmp(cmd, "view-profile"))
		view_profile_function(posts_list);
	else if (!strcmp(cmd, "friends-repost"))
		friends_repost_function(posts_list, graph);
	else if (!strcmp(cmd, "common-group"))
	{
		char *nume = strtok(NULL, "\n ");
		bk(graph, get_user_id(nume));
	}
	free(commands);
}
