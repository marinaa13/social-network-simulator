#ifndef FEED_H
#define FEED_H

int verify_friend(matrix_graph_t *graph, int id_1, int id_2);

void get_all_reposts(tree_node_t *root, int user_id, int post_id,
					 char *post_title);

void get_all_friends_reposts(tree_node_t *root, matrix_graph_t *graph,
							 int user_id);

void feed_function(doubly_linked_list_t *posts_list, matrix_graph_t *graph);

void view_profile_function(doubly_linked_list_t *posts_list);

void friends_repost_function(doubly_linked_list_t *posts_list,
							 matrix_graph_t *graph);

int is_empty(int *arr, int nodes);

void bk_rec_calc_max(matrix_graph_t *graph, int *R, int *P, int *X,
					 int nodes, int *maxi);

void bk_rec_af_max(matrix_graph_t *graph, int *R, int *P, int *X,
				   int nodes, int maxi);

void bk(matrix_graph_t *graph, int start_node);

void handle_input_feed(char *input, matrix_graph_t *graph,
					   doubly_linked_list_t *posts_list);
#endif // FEED_H
