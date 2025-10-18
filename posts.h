#ifndef POSTS_H
#define POSTS_H

#define DIE(assertion, call_description)            \
	do                                              \
	{                                               \
		if (assertion)                              \
		{                                           \
			fprintf(stderr, "(%s, %d): ", __FILE__, \
					__LINE__);                      \
			perror(call_description);               \
			exit(errno);                            \
		}                                           \
	} while (0)                                     \

typedef struct post_t
{
	int id;
	char *title;
	int user_id;
	int num_likes;
	int *users_liked;
} post_t;

typedef struct dll_node_t dll_node_t;
struct dll_node_t
{
	void *data;
	dll_node_t *next, *prev;
};

typedef struct doubly_linked_list_t
{
	dll_node_t *head;
	dll_node_t *tail;
	unsigned int data_size;
	unsigned int size;
} doubly_linked_list_t;

typedef struct tree_node_t tree_node_t;
struct tree_node_t
{
	void *value;
	tree_node_t **children;
	int n_children;
	int height;
};

typedef struct tree_t
{
	tree_node_t *root;
} tree_t;

doubly_linked_list_t*
dll_create(unsigned int data_size);

dll_node_t*
dll_get_nth_node(doubly_linked_list_t *list, unsigned int n);

void dll_add_nth_node(doubly_linked_list_t *list, unsigned int n,
					  const void *new_data);

dll_node_t *dll_remove_nth_node(doubly_linked_list_t *list, unsigned int n);

void dll_remove_node(doubly_linked_list_t *list, dll_node_t *node);

int compare_function_ints_list(void *a, void *b);

int compare_function_ints_tree(void *a, void *b);

dll_node_t *dll_get_node(doubly_linked_list_t *list, void *data,
						 int (*compare_function)(void *, void *));

unsigned int dll_get_size(doubly_linked_list_t *list);

void free_struct_data(void *data);

void dll_free(doubly_linked_list_t **pp_list,
			  void (*free_tree)(tree_node_t *, void (*)(void *)));

tree_node_t *create_node(void *value);

tree_node_t *create_tree(void *root_content);

tree_node_t *get_tree_node(tree_node_t *node, void *value,
						   int (*compare_function)(void *, void *));

tree_node_t *get_tree_node_parent(tree_node_t *node, void *value,
								  int (*compare_function)(void *, void *));

void add_child(tree_node_t *parent, void *value);

void free_tree(tree_node_t *node, void (*free_struct_data)(void *));

void get_all_parents_ids(tree_node_t *root, tree_node_t *node, int **parents,
						 int *n);

int compare_parents(int *parents1, int n1, int *parents2, int n2);

void print_children(tree_node_t *node);

void get_max_likes(tree_node_t *node, int *max_likes, int *max_id);

void create_function(int *num_posts, doubly_linked_list_t *posts_list);

void repost_function(int *num_posts, doubly_linked_list_t *posts_list);

void common_repost_function(doubly_linked_list_t *posts_list);

void like_function(doubly_linked_list_t *posts_list);

void ratio_function(doubly_linked_list_t *posts_list);

void delete_function(doubly_linked_list_t *posts_list);

void get_likes_function(doubly_linked_list_t *posts_list);

void get_reposts_function(doubly_linked_list_t *posts_list);

void handle_input_posts(char *input, int *num_posts,
						doubly_linked_list_t *posts_list);

#endif // POSTS_H
