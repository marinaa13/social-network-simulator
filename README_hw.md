**Echipa tema 3 SD:**
- **Baranga Roxana Mary - roxana_mary.baranga**
- **Simion Marina Crenguta - marina.simion05**

Each command has its own function, plus some helper ones, and they follow the
same pattern: parse the input (the command's arguments), execute the command,
print the output/the success message. 

1. **Task 1 - Users and Friends**
	The first task implements a social media user management system that allows
users to add friends, delete friends, get the distance between two accounts,
get suggestions for friends, see common friends, see number of friends and see
the most popular account. The data structure used is a graph implemented with
an adjancecy matrix. The functions in users.c are for implementing the graph
and the functions in friends.c are for implementing the social media commands.
	The *add_friend* function adds a friend to the user's list of friends, by
setting the corresponding cells in the matrix to 1. (calling the *mg_add_edge*
function)
	The *remove_friend* function removes a friend from the user's list of
friends, by setting the corresponding cells in the matrix to 0. (calling the
*mg_remove_edge* function)
	The *distance_friend* function calculates the distance between 2 users, by
calling the *mg_dist* function that uses the BFS algorithm to find the shortest
path between 2 nodes.
	The *suggestions_friend* function finds the suggestions for friends for a
user, by calling the *mg_grandkids* function that uses the BFS algorithm to
find the friends of friends of a user.
	The *common_people* function finds the common friends of 2 users, by
checking the adjacency matrix, calling the function *mg_has_edge*.
	The *friends* function returns the number of friends of a user, by calling
the function *mg_has_edge* and counting the number of 1s in the row
of the user.
	The *influencer* function returns the most popular account, by calling the
functions *friends* and *mg_has_edge* and using a frequency array to store the
number of friends of each user.
 
2. **Task 2 - Posts and reposts**
	This task implements a social media post management system that allows
users to create posts, repost existing posts, like/unlike posts, delete posts,
and retrieve information about them. The data structures used are a doubly
linked list for storing posts and a tree for managing reposts. Therefore, the
first functions included are the ones for the data structures. The informations
about a post/repost are stored using a struct. At the end of the program, in
the main function, all of the allocated memory is freed.
	The *create* function creates a new post with its corresponding ID, by
adding a new node to the end of the list to store the information about
the post.
	The *repost* function takes an ID and creates a repost of the given
post/repost, by adding a new child in the tree of the post in the right
position.
	The *common repost* function return the first common post/repost of the 2
given reposts. It first gets the array of parents, starting from the bottom and
going up the tree to the root, and then iterates through both the arrays at the
same time, stopping when the values that are being parsed are equal
(the common parent was found).
	The *like* function likes/dislikes the given post/repost. If the given user
had already liked the post, then his like will be retracted. For this check to
be possible, the struct contains an array of int to store the IDs of the users
who liked a post. When a users dislikes a post, his ID is deleted from the
array and the number of likes is decremented.
	The *ratio* function determines if a post has been ratio'd or not. Ratio
means that the number of likes of a repost is greater than the number of likes
of the initial post, so the function checks this fact for each of the reposts.
	The *delete post/repost* deletes a post/repost and frees the memory. When a
post is deleted, the entire tree of reposts is also erased.
	The *get likes* function prints the number of likes that a post/repost has,
while the *get reposts* function prints the entire tree of reposts that a
post/repost has, in the order of the IDs.
 
 
3.  **Task 3 - Social Media**
	Combining the first 2 tasks into simulating different parts of a social
media app: a user's feed and profile, the friends that reposted a post and a
user's group of friends. 
	The *feed* function iterates through the tree of posts starting from the
end, to get the most recent *n* posts in order. It retrieves the posts made by
the given user or his friends.
	The *view profile* function iterates through the tree of posts starting 
from the beginning, to get the posts in chronological order. It firstly prints
all the posts made by a user, and then the reposts.
	The *friends that reposted* function gets the user who made the post with
the given ID and prints all the user's friends who reposted it.
    The *bk* function implements the Bron-Kerbosch algorithm to find the
maximal cliques in a graph. It uses the recursive function *bk_rec_calc_max* to
find all the cliques that contain a given vertex, saves in the variable maxi
the maximum number of vertices in a clique and calls the recursive function
*bk_rec_af_max* that prints the clique that has the maximum number of vertices.
The R array is used to store the vertices that are in the current clique and
the P array is used to store the vertices that are candidates to be added to
the clique. The X array is used to store the vertices that are not candidates
to be added to the clique because they were considered previously.