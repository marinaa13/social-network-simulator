/**
 * The entrypoint of the homework. Every initialization must be done here
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"

/**
 * Initializez every task based on which task we are running
*/
void init_tasks(void)
{
	#ifdef TASK_1

	#endif

	#ifdef TASK_2

	#endif

	#ifdef TASK_3

	#endif
}

/**
 * Entrypoint of the program, compiled with different defines for each task
*/
int main(void)
{
	init_users();
	matrix_graph_t *graph = mg_create(MAX_PEOPLE);
	init_tasks();

	char *input = (char *)malloc(MAX_COMMAND_LEN);
	doubly_linked_list_t *posts_list = dll_create(sizeof(void *));
	int num_posts = 0;
	num_posts++;

	while (1) {
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns "exit", we reached EOF
		if (!command)
			break;

		#ifdef TASK_1
		handle_input_friends(input, graph);
		#endif

		#ifdef TASK_2
		handle_input_posts(input, &num_posts, posts_list);
		#endif

		#ifdef TASK_3
		handle_input_feed(input, graph, posts_list);
		#endif
	}

	dll_free(&posts_list, free_tree);
	free_users();
	free(input);
	mg_freegraph(graph);

	return 0;
}
