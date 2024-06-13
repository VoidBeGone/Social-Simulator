/**
 * Assignment 3 - Wacky Marketing
 *
 * Please read the comments below carefully, they describe your task in detail.
 *
 * There are also additional notes and clarifications on Quercus.
 *
 * Any clarifications and corrections will be posted to Piazza so please keep an
 * eye on the forum!
 *
 * Unauthorized distribution or posting is strictly prohibited. You must seek
 * approval from course staff before uploading and sharing with others.
 *
 * Starter code:  (Updated by) Charles Xu, 2024
 *             (Originally by) William Song, Mustafa Quraish, 2020
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 1024

#ifndef MAT_SIZE
#define MAT_SIZE 3  // A small graph
#endif

typedef struct user_struct {
    char name[MAX_STR_LEN];
    struct friend_node_struct *friends;
    struct brand_node_struct *brands;
    bool visited;
} User;

typedef struct friend_node_struct {
    User *user;
    struct friend_node_struct *next;
} FriendNode;

typedef struct brand_node_struct {
    char brand_name[MAX_STR_LEN];
    struct brand_node_struct *next;
} BrandNode;

typedef struct helper_struct {
    User *user;
    int dgc;
    struct helper_struct *next;
}helper_struct;

// Note: These are global variables.
FriendNode *allUsers = NULL;
int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Given the head of a linked list of FriendNode, this function checks if
 * a given user's name exists in the list. Returns true if the user's name
 * is found, and false otherwise.
 *
 * @param head Pointer to the head of the linked list of FriendNode.
 * @param node Pointer to the user whose name is to be searched in the list.
 * @return true if the user's name exists in the list, false otherwise.
 */
bool in_friend_list(FriendNode *head, User *node) {
    for (FriendNode *cur = head; cur != NULL; cur = cur->next) {
        if (strcmp(cur->user->name, node->name) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * Given the head of a linked list of BrandNode, this function checks if
 * a given brand's name exists in the list. Returns true if the brand's name
 * is found, and false otherwise.
 *
 * @param head Pointer to the head of the linked list of BrandNode.
 * @param name Pointer to the brand name to be searched in the list.
 * @return true if the brand's name exists in the list, false otherwise.
 */
bool in_brand_list(BrandNode *head, char *name) {
    for (BrandNode *cur = head; cur != NULL; cur = cur->next) {
        if (strcmp(cur->brand_name, name) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * Given the head of a linked list of FriendNode, this function inserts
 * a given user into the linked list in alphabetical order of names. If
 * the user already exists, no modifications are made and the original
 * list is returned. Returns the head of the new linked list after insertion.
 *
 * @param head Pointer to the head of the linked list of FriendNode.
 * @param node Pointer to the user to be inserted into the list.
 * @return Pointer to the head of the new linked list after insertion.
 */
FriendNode *insert_into_friend_list(FriendNode *head, User *node) {
    if (node == NULL) return head;

    if (in_friend_list(head, node)) {
        printf("User already in list\n");
        return head;
    }

    FriendNode *fn = calloc(1, sizeof(FriendNode));
    fn->user = node;
    fn->next = NULL;

    if (head == NULL) return fn;

    if (strcmp(head->user->name, node->name) > 0) {
        fn->next = head;
        return fn;
    }

    FriendNode *cur;
    for (cur = head;
         cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
         cur = cur->next)
        ;

    fn->next = cur->next;
    cur->next = fn;

    return head;
}

/**
 * Given the head of a linked list of BrandNode, this function inserts
 * a given brand into the linked list in alphabetical order of names. If
 * the brand already exists, no modifications are made and the original
 * list is returned. Returns the head of the new linked list after insertion.
 *
 * @param head Pointer to the head of the linked list of BrandNode.
 * @param node Pointer to the brand name to be inserted into the list.
 * @return Pointer to the head of the new linked list after insertion.
 */
BrandNode *insert_into_brand_list(BrandNode *head, char *node) {
    if (node == NULL) return head;

    if (in_brand_list(head, node)) {
        printf("Brand already in list\n");
        return head;
    }

    BrandNode *fn = calloc(1, sizeof(BrandNode));
    strcpy(fn->brand_name, node);
    fn->next = NULL;

    if (head == NULL) return fn;

    if (strcmp(head->brand_name, node) > 0) {
        fn->next = head;
        return fn;
    }

    BrandNode *cur;
    for (cur = head;
         cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
         cur = cur->next)
        ;

    fn->next = cur->next;
    cur->next = fn;

    return head;
}

/**
 * Given the head of a linked list of FriendNode, this function removes
 * a given user from the linked list. If the user does not exist in the list,
 * no modifications are made and the original list is returned. Returns the
 * head of the new linked list after deletion.
 *
 * @param head Pointer to the head of the linked list of FriendNode.
 * @param node Pointer to the user to be removed from the list.
 * @return Pointer to the head of the new linked list after deletion.
 */
FriendNode *delete_from_friend_list(FriendNode *head, User *node) {
    if (node == NULL) return head;

    if (!in_friend_list(head, node)) {
        printf("User not in list\n");
        return head;
    }

    if (strcmp(head->user->name, node->name) == 0) {
        FriendNode *temp = head->next;
        free(head);
        return temp;
    }

    FriendNode *cur;
    for (cur = head; cur->next->user != node; cur = cur->next)
        ;

    FriendNode *temp = cur->next;
    cur->next = temp->next;
    free(temp);

    return head;
}

/**
 * Given the head of a linked list of BrandNode, this function removes
 * a given brand from the linked list. If the brand does not exist in the list,
 * no modifications are made and the original list is returned. Returns the
 * head of the new linked list after deletion.
 *
 * @param head Pointer to the head of the linked list of BrandNode.
 * @param node Pointer to the brand name to be removed from the list.
 * @return Pointer to the head of the new linked list after deletion.
 */
BrandNode *delete_from_brand_list(BrandNode *head, char *node) {
    if (node == NULL) return head;

    if (!in_brand_list(head, node)) {
        printf("Brand not in list\n");
        return head;
    }

    if (strcmp(head->brand_name, node) == 0) {
        BrandNode *temp = head->next;
        free(head);
        return temp;
    }

    BrandNode *cur;
    for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
        ;

    BrandNode *temp = cur->next;
    cur->next = temp->next;
    free(temp);

    return head;
}

/**
 * Given a user, this function prints their name, friends, and liked brands.
 *
 * @param user Pointer to the user whose data is to be printed.
 */
void print_user_data(User *user) {
    printf("User name: %s\n", user->name);

    printf("Friends:\n");
    for (FriendNode *f = user->friends; f != NULL; f = f->next) {
        printf("   %s\n", f->user->name);
    }

    printf("Brands:\n");
    for (BrandNode *b = user->brands; b != NULL; b = b->next) {
        printf("   %s\n", b->brand_name);
    }
}

/**
 * Given a brand name, this function returns the index of the brand inside
 * the brand_names array. If the brand doesn't exist in the array, it
 * returns -1.
 *
 * @param name Pointer to the brand name.
 * @return The index of the brand in the brand_names array, or -1
 * if it doesn't exist.
 */
int get_brand_index(char *name) {
    for (int i = 0; i < MAT_SIZE; i++) {
        if (strcmp(brand_names[i], name) == 0) {
            return i;
        }
    }

    printf("Brand '%s' not found\n", name);
    return -1;  // Not found
}

/**
 * Given a brand name, this function prints its name, index inside the
 * brand_names array, and the names of other similar brands.
 *
 * @param brand_name Pointer to the brand name.
 */
void print_brand_data(char *brand_name) {
    int idx = get_brand_index(brand_name);
    if (idx < 0) {
        printf("Brand '%s' not in the list.\n", brand_name);
        return;
    }

    printf("Brand name: %s\n", brand_name);
    printf("Brand idx: %d\n", idx);

    printf("Similar brands:\n");
    for (int i = 0; i < MAT_SIZE; i++) {
        if (brand_adjacency_matrix[idx][i] == 1 &&
            strcmp(brand_names[i], "") != 0) {
            printf("   %s\n", brand_names[i]);
        }
    }
}

/**
 * Given a file name, this function reads from the file and populates the brand
 * list and brand matrix.
 *
 * @param file_name Pointer to the name of the file to be read.
 */
void populate_brand_matrix(char *file_name) {
    // Read the file
    char buff[MAX_STR_LEN];
    FILE *f = fopen(file_name, "r");
    fscanf(f, "%s", buff);
    char *line = buff;
    // Load up the brand_names matrix
    for (int i = 0; i < MAT_SIZE; i++) {
        if (i == MAT_SIZE - 1) {
            strcpy(brand_names[i], line);
            break;
        }
        int index = strchr(line, ',') - line;
        strncpy(brand_names[i], line, index);
        line = strchr(line, ',') + sizeof(char);
    }
    // Load up the brand_adjacency_matrix
    for (int x = 0; x < MAT_SIZE; x++) {
        fscanf(f, "%s", buff);
        for (int y = 0; y < MAT_SIZE; y++) {
            int value = (int)buff[y * 2];
            if (value == 48) {
                value = 0;
            } else {
                value = 1;
            }
            brand_adjacency_matrix[x][y] = value;
        }
    }
}

/**
 * Given a name, this function creates a new user on the platform. The user is
 * then inserted into the allUsers linked list. If a user already existed with
 * the same name, no action is taken, and NULL is returned instead.
 *
 * @param name Pointer to the name of the user to be created.
 * @return Pointer to the created user, or NULL if a user with
 * the same name already exists.
 */
User *create_user(char *name) {
    // TODO: Complete this function.
    // we have a global variable allusers 
    if (name == NULL){return NULL;}
    // this is to check if name exist
    User *new_user = NULL;
    new_user = (User *)malloc(sizeof(User));
    if (new_user == NULL){return NULL;}
    strcpy(new_user->name, name);
    new_user->friends = NULL;
    new_user->brands = NULL;
    new_user->visited = false;
    //just simply creating a new_user 
    if (in_friend_list(allUsers, new_user)){
        free(new_user);
        return NULL;
        }
        // if the thing is in friends list this means we need to delete everything we created
    // checking if user is in the friend list already 
    allUsers = insert_into_friend_list(allUsers, new_user);
    //putting the user into the friends list in global var
    return new_user;
}

/**
 * Removes a given user from the platform. The user is removed from the allUsers
 * linked list and the friend lists of any users that they belong to. Returns 0
 * if the user was successfully removed. If the user does not exist, returns -1
 * instead.
 *
 * @param user Pointer to the user to be removed.
 * @return 0 if the user was successfully removed,
 * -1 if the user does not exist.
 */
int delete_user(User *user) {
    // TODO: Complete this function.
    if (user == NULL){return -1;}
    if (allUsers == NULL){return -1;}
    // ok so the sanity check i used are checking if allusers has people in it and if user exist
    //now under this I am going trhough every user friends and deletein gthem before deleteing them from the big one
    if (in_friend_list(allUsers, user)){
        FriendNode *p = allUsers;
        while (p!=NULL){
            if (in_friend_list(p->user->friends, user)){
                // I NEED TO DELETE THE USER FROM EVERY OTHER USES
                p->user->friends = delete_from_friend_list(p->user->friends, user);
            }
            p = p->next;
        }
        BrandNode *m = user->brands;
        BrandNode *q = m;
        while (m != NULL){
            m=m->next;
            free(q);
            q = NULL;
            q=m;
        }
        FriendNode *l = user->friends;
        FriendNode *n = l;
        while(l!=NULL){
            l = l->next;
            free(n);
            n=NULL;
            n=l;
        }
        allUsers = delete_from_friend_list(allUsers, user);
        user->friends = NULL;
        user->brands = NULL;
        free(user);
        user = NULL;
        return 0;
    }   
    return -1;
}

/**
 * Given a pair of valid users, this function creates a friendship between them.
 * The user's friends list must remain in alphabetical order. Returns 0 if the
 * friendship was successfully created. Returns -1 if the pair were already
 * friends.
 *
 * @param user Pointer to the user initiating the friendship.
 * @param friend Pointer to the user to be added as a friend.
 * @return 0 if the friendship was successfully created,
 * -1 if the pair were already friends.
 */
int add_friend(User *user, User *friend) {
    // TODO: Complete this function.
    // NOTE THESE ARE USER POINTER DUMBASS
    if (user == NULL || friend == NULL){return -1;}
    if (!(in_friend_list(allUsers, user)) || !(in_friend_list(allUsers, friend))){return -1;}
    // checking if both things are not null, then checking if it actually exist in the allusers
    //note i have to add friends on both sides 
    //so i had to add friend to user and then user to friend 
    if (!(in_friend_list(user->friends,friend))){
        user->friends = insert_into_friend_list(user->friends, friend);
        friend->friends = insert_into_friend_list(friend->friends, user);
        return 0;
    }
    return -1;
}

/**
 * Given a pair of valid users, this function removes their friendship. The
 * user's friends list must remain in alphabetical order. Returns 0 if the pair
 * are no longer friends. Returns -1 if the pair were not friends to begin with.
 *
 * @param user Pointer to the user initiating the removal of friendship.
 * @param friend Pointer to the user to be removed as a friend.
 * @return 0 if the friendship was successfully removed,
 * -1 if the pair were not friends.
 */
int remove_friend(User *user, User *friend) {
    // TODO: Complete this function.
    if (user == NULL || friend == NULL){return -1;}
    if (!(in_friend_list(allUsers, user)) || !(in_friend_list(allUsers, friend))){return -1;}
    // checking if both things are not null, then checking if it actually exist in the allusers
    if (in_friend_list(user->friends,friend)){
        user->friends = delete_from_friend_list(user->friends, friend);
            if (in_friend_list(friend->friends, user)){
                friend->friends = delete_from_friend_list(friend->friends,user);}
        return 0;
    }

    return -1;
}



/**
 * Given a valid user and the name of a brand, this function creates a link
 * between the user and the brand. The user's brands list must remain in
 * alphabetical order. Returns 0 if the link was successfully created. Returns
 * -1 if the link already existed or if the brand name is invalid.
 *
 * @param user Pointer to the user.
 * @param brand_name Pointer to the name of the brand to follow.
 * @return 0 if the link was successfully created,
 * -1 if the link already existed or the brand name is invalid.
 */
bool is_brand_valid(char *brand_test){
    for (int i = 0; i<MAT_SIZE;i++){
        if (strcmp(brand_names[i], brand_test) == 0){
            return true;
        }
    }
    return false;
}

int follow_brand(User *user, char *brand_name) {
    // TODO: Complete this function.
    if (user == NULL){return -1;}
    if (brand_name == NULL){return -1;}
    if (!in_friend_list(allUsers, user)){return -1;}
    if (in_brand_list(user->brands, brand_name)){return -1;}
    if(!is_brand_valid(brand_name)){return -1;}
    user->brands = insert_into_brand_list(user->brands, brand_name);
    return 0;
}

/**
 * Given a valid user and the name of a brand, this function removes the link
 * between the user and the brand. The user's brands list must remain in
 * alphabetical order. Returns 0 if the link was successfully removed. Returns
 * -1 if the link did not previously exist or if the brand name is invalid.
 *
 * @param user Pointer to the user.
 * @param brand_name Pointer to the name of the brand to unfollow.
 * @return 0 if the link was successfully removed, -1 if the link did not
 * previously exist or the brand name is invalid.
 */
int unfollow_brand(User *user, char *brand_name) {
    // TODO: Complete this function.
    if (user == NULL || brand_name == NULL){return -1;}
    if (!in_friend_list(allUsers, user)){return -1;}
    if (!in_brand_list(user->brands, brand_name)){return -1;}
    user->brands = delete_from_brand_list(user->brands, brand_name);
    return 0;
}

/**
 * Given a pair of valid users, this function returns the number of mutual
 * friends between them. A mutual friend is a user that exists in the friends
 * list of both User a and User b.
 *
 * @param a Pointer to the first user.
 * @param b Pointer to the second user.
 * @return The number of mutual friends between the two users.
 */
int get_mutual_friends(User *a, User *b) {
    // TODO: Complete this function.
    //so what I want to do is count the ammount of user that both a and b share
    if (a == NULL || b == NULL){return 0;}
    if (!in_friend_list(allUsers, a) || !in_friend_list(allUsers, b)){return -1;}
    FriendNode *a1 = a->friends;
    FriendNode *b1= b->friends;
    int count = 0;
    while (a1 != NULL) {
        FriendNode *temp_b = b1;  
        while (temp_b != NULL) {

            if (temp_b->user == a1->user) {
                count++;
                break; 
            }
            temp_b = temp_b->next;
        }
        a1 = a1->next;
    }
    return count;
}

/**
 * Given a pair of valid users, this function returns the degree of connection
 * between both users. The degree of connection is the shortest number of steps
 * it takes to get from one user to the other. If a connection cannot be formed,
 * returns -1.
 *
 * @param a Pointer to the first user.
 * @param b Pointer to the second user.
 * @return The degree of connection between the two users, or -1 if no
 * connection can be formed.
 */
helper_struct *creating_helper_struct(User *user, int x){
    if (user == NULL){return NULL;}
    helper_struct *node = NULL;
    node = (helper_struct *)malloc(sizeof(helper_struct));
    if (node == NULL){return NULL;}

    node ->user = user;
    node->next = NULL;
    node->dgc = x;
    return node;
}

helper_struct *adding_to_struct(helper_struct *head, helper_struct *new_node){
    //just adding people the to end of the queue
    if (head == NULL){return NULL;}
    if (new_node == NULL){return head;}
    helper_struct *q = head;
    helper_struct *p;
    while (q!=NULL){
        p = q;
        q = q->next;
    }
    p->next = new_node;
    new_node->next = NULL;  
    return head;
}

helper_struct *delete_users_from_struct(helper_struct *head){
    if (head == NULL){return NULL;}
    helper_struct *p = head->next;
    head->user->visited = true;
    return p;
}

void order66(helper_struct *head){
    helper_struct *p = head;
    while (p != NULL){
        helper_struct *temp = p;
        p = p->next;
        if (temp->user != NULL) {
            temp->user->visited = false;
            temp->user = NULL;
        }
        free(temp);
    }
}
int get_degree_helper(User *b, helper_struct *head) {
    if (head == NULL) {
        return -1; 
    }

    if (in_friend_list(head->user->friends, b)) {
        return head->dgc;
    } else {
        FriendNode *p = head->user->friends;
        int dgc = head->dgc + 1;
        while (p != NULL) {
            if (!(p->user->visited)) {
                helper_struct *new_node = creating_helper_struct(p->user, dgc);
                head = adding_to_struct(head, new_node); 
            }
            p = p->next;
        }
        head = delete_users_from_struct(head); 
        return get_degree_helper(b, head); 
    }
}

int get_degrees_of_connection(User *a, User *b) {
    if (a == NULL || b == NULL) {
        return -1;
    }
    if (!in_friend_list(allUsers, a) || !in_friend_list(allUsers, b)) {
        return -1;
    }
    if (strcmp(a->name, b->name) == 0) {
        return 0;
    }
    a->visited = true; 
    helper_struct *a1 = creating_helper_struct(a, 1);
    int result = get_degree_helper(b, a1);
    order66(a1);
    return result;
}






/**
 * Given two brand names, this function marks the two brands as similar in the
 * brand_adjacency_matrix variable. If either brand name is invalid, no action
 * is taken.
 *
 * @param brandNameA Pointer to the first brand name.
 * @param brandNameB Pointer to the second brand name.
 */
void connect_similar_brands(char *brandNameA, char *brandNameB) {
    // TODO: Complete this function.
    // okay so what do i want to do, if the brands are smiliar I want to mark that in the brand adjancey martrix 
    //first sanity check that brands names 
    if (brandNameA == NULL || brandNameB == NULL){return;}

    //now I have to figure out what makes a brand smililar
    int index_a;
    int index_b;
    int quick_release = 0;
    for (int i = 0; i < MAT_SIZE; i++){
        if (quick_release == 2){break;}
        if (strcmp(brand_names[i], brandNameA) == 0){
            index_a = i;
            quick_release++;
        }
        if (strcmp(brand_names[i], brandNameB) == 0){
            index_b = i;
            quick_release++;
        }
    }
    if (quick_release == 2){
        brand_adjacency_matrix[index_a][index_b] = 1;
        brand_adjacency_matrix[index_b][index_a] = 1;
    }
    return;
}

/**
 * Given two brand names, this function marks the two brands as different (not
 * similar) in the brand_adjacency_matrix variable. If either brand name is
 * invalid, no action is taken.
 *
 * @param brandNameA Pointer to the first brand name.
 * @param brandNameB Pointer to the second brand name.
 */
void remove_similar_brands(char *brandNameA, char *brandNameB) {
    // TODO: Complete this function.
    if (brandNameA == NULL || brandNameB == NULL){return;}

    //now I have to figure out what makes a brand smililar
    int index_a;
    int index_b;
    int quick_release = 0;
    for (int i = 0; i < MAT_SIZE; i++){
        if (quick_release == 2){break;}
        if (strcmp(brand_names[i], brandNameA) == 0){
            index_a = i;
            quick_release++;
        }
        if (strcmp(brand_names[i], brandNameB) == 0){
            index_b = i;
            quick_release++;
        }
    }
    if (quick_release == 2){
        brand_adjacency_matrix[index_a][index_b] = 0;
        brand_adjacency_matrix[index_b][index_a] = 0;
    }
    return;
}

/**
 * Given a user, suggest a new friend for them.
 *
 * To find the best match, the new suggested friend should have the highest
 * number of mutually liked brands amongst all other valid candidates.
 *
 * If a tie needs to be broken, select the user with the name that comes first
 * in reverse-alphanumerical order.
 *
 * The suggested friend must be a valid user, cannot be the user themself, nor
 * cannot be someone that they're already friends with.
 *
 * If the user is already friends with everyone on the platform, returns NULL.
 *
 * At times, a similarity rating of 0 is the best option.
 *
 * @param user Pointer to the user for whom a friend is being suggested.
 * @return Pointer to the suggested friend, or NULL if no suitable friend is
 * found.
 */
User *get_suggested_friend(User *user) {
    // TODO: Complete this function.
    if (user == NULL || allUsers == NULL){return NULL;}
    if (!in_friend_list(allUsers, user)){return NULL;}
    //what I want to do
    //have a variable that store how many similar brands someone ahs 
    //then you just loop throuhg the global one and change htat variable 
    int connection = -1;
    User *closest = NULL;
    FriendNode *helper = allUsers;
    while (helper != NULL){
        User *p = helper->user; 
        BrandNode *p1 = p->brands;
        int check = 0;
        while (p1 != NULL){
            BrandNode *u = user->brands;
            //maybe do both sides later;
            if (!(in_friend_list(user->friends, p))){
                while (u != NULL){
                    if (in_brand_list(u, p1->brand_name)){
                        check++;
                    }
                    u=u->next;
                }
            }
            p1=p1->next;
        }
        if (connection <=  check && p != user && !in_friend_list(user->friends, p) && p != NULL){
            if (check > connection || (check == connection && (strcmp(p->name, closest->name) > 0))) {
            closest = p;
            connection = check;
        }
        }
        helper = helper->next;
    }
    return closest;
}

/**
 * Given a user and a positive integer n, this function adds n suggested friends
 * using the get_suggested_friend() function. There might not be enough users on
 * the platform to satisfy n completely, so it returns the amount of friends
 * successfully added.
 *
 * @param user Pointer to the user for whom suggested friends are being added.
 * @param n The number of suggested friends to add.
 * @return The number of friends successfully added.
 */
int add_suggested_friends(User *user, int n) {
    // TODO: Complete this function.
    if (user == NULL){return 0;}
    if (!(in_friend_list(allUsers,user))){return 0;}
    int count = 0;
    for (int i = 0; i<n; i++){
        User *helper = get_suggested_friend(user);
        if (helper != NULL){
            if (add_friend(user,helper) == 0){
                count ++;
            }
        }
    }
    return count;
}

/**
 * Given a user and a positive integer n, this function suggests and follows n
 * new brands for them.
 *
 * To find the best matches, suggested brands have the most similarities with
 * the brands that the user already follows.
 *
 * If a tie needs to be broken, the brands with the names that comes first in
 * reverse-alphanumerical order is selected.
 *
 * The suggested brand must be a valid brand and cannot be a brand that the user
 * already follows.
 *
 * There might not be enough brands on the platform to satisfy n completely, so
 * the function returns the amount of brands successfully followed.
 *
 * At times, a similarity rating of 0 is the best option.
 *
 * @param user Pointer to the user for whom brands are being suggested.
 * @param n The number of suggested brands to follow.
 * @return The number of brands successfully followed.
 */
int get_brand_size(User *user){
    int count = 0;
    BrandNode *user_brand = user->brands;
    while (user_brand != NULL){
        count ++;
        user_brand = user_brand->next;
    }
    return count;
} 

void get_suggested_brand_helper(User *user, int *arr){
    BrandNode *user_brand = user->brands;
    int index = 0;
    while (user_brand != NULL){
        int brand_index = get_brand_index(user_brand->brand_name);
        arr[index] = brand_index;
        user_brand = user_brand->next;
        index++;
    }
}
bool helper_final_bool(char *new_brand, char helper_final_char[][MAX_STR_LEN], int n){
    if (strcmp(new_brand, "") == 0)
        return true;

    for (int i = 0; i < n; i++) {
        if (strcmp(helper_final_char[i], new_brand) == 0)
            return false;
    }
    return true;
}

int get_suggested_brands(User *user, int n) {
    if (user == NULL) {
        return 0;
    }

    int size = get_brand_size(user);
    int *arr = malloc(size * sizeof(int));
    if (arr == NULL) {
        return 0;
    }
    get_suggested_brand_helper(user, arr);

    char (*helper_final_char)[MAX_STR_LEN] = malloc(n * sizeof(*helper_final_char));
    if (helper_final_char == NULL) {
        free(arr);
        return 0;
    }

    int current_position = 0;
    for (int l = 0; l < n; l++) {
        int best = -1;
        char helper[MAX_STR_LEN] = "";
        for (int i = 0; i < MAT_SIZE; i++) {
            if (!in_brand_list(user->brands, brand_names[i])) {
                int best_helper = 0;
                for (int k = 0; k < size; k++) {
                    if (brand_adjacency_matrix[arr[k]][i] == 1 && brand_adjacency_matrix[i][arr[k]] == 1) {
                        best_helper++;
                    }
                }
                if (helper_final_bool(brand_names[i], helper_final_char, current_position)) {
                    if (best_helper >= best) {
                        if (best_helper > best) {
                            strcpy(helper, brand_names[i]);
                            best = best_helper;
                        } else if (strcmp(brand_names[i], helper) >= 0) {
                            strcpy(helper, brand_names[i]);
                            best = best_helper;
                        }
                    }
                }

            }
        }
        strcpy(helper_final_char[l], helper);
        current_position++;
    }

    int count = 0;
    for (int i = 0; i < n; i++) {
        if (follow_brand(user, helper_final_char[i]) == 0) {
            count++;
        }
    }

    free(arr);
    free(helper_final_char);
    return count;
}


int follow_suggested_brands(User *user, int n) {
    // TODO: Complete this function.
    if (user == NULL){return 0;}
    if (!in_friend_list(allUsers, user)){return 0;}
    int please = get_suggested_brands(user, n) ;
    return please;
}