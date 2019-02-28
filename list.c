#include "list.h"

void *my_calloc (size_t nmemb, size_t size) {
    srand(time(NULL));

    int random_number = rand() % 10;
   
    if (ERR_PROBABILITY * 10 > random_number) {
        return NULL;
    }
    else {
        return calloc(nmemb, size);
    }
}

void *count_len (struct Node *node, va_list args) {
    size_t *len = va_arg(args, size_t *);
    (*len)++;
}


void *node_print(struct Node *node, va_list args) {
    printf("%s\n", node->data);

    return NULL;
}

struct Node *Node_Construct (data_t value) {
    struct Node *new_one = my_calloc(1, sizeof(struct Node));
    if (new_one == NULL) {      //we are out of memory
        return NULL;
    }

    new_one->data = value;
    new_one->next = NULL;

    return new_one;
}

struct List *List_Construct () {
    struct List *new_one = my_calloc(1, sizeof(struct List));
    if (new_one == NULL) {      //we are out of memory
        return NULL;
    }

    new_one->head = NULL;
    new_one->tail = NULL;
    new_one->len = 0;

    return new_one;
}

void Node_Destruct (struct Node **old_one) {
    if (*old_one != NULL) {
        free(*old_one);
        *old_one = NULL;
    }
}

void List_Destruct (struct List **old_one) {
    if (*old_one != NULL) {
        struct Node *node = (*old_one)->head;
        if (node != NULL) {
            struct Node *next = node->next;
            while (node != NULL) {
                next = node->next;
                Node_Destruct(&node);
                node = next;
            }
        }

        free(*old_one);
        *old_one = NULL;
    }
}

int List_Ok (struct List *list) {
    //check for some errors in list's structure
    if (list == NULL || list->len < 0) {
        return LIST_ERR;
    }

    if (list->len == 1 && (list->head != list->tail) ||
        (list->len != 0 && (list->head == NULL || list->tail == NULL)) ) {
        return LIST_ERR;
    }
    if (list->tail != NULL && list->tail->next != NULL) {
        return LIST_ERR;
    }

    //check for loop
    struct Node *list_elem = list->head;
    struct Node *point1 = list_elem;
    struct Node *point2 = list_elem;
    int i = 0;
    while (point1 && point2) {
        if (point1 == point2 && i != 0) {
            return LIST_ERR;
        }

        point1 = point1->next;
        point2 = point2->next;
        if (point2 == NULL || point1 == NULL) {
            break;
        }
        else {
            point2 = point2->next;
        }
        i++;
    }

    if (list->len != List_Count_Len(list)) {
        return LIST_ERR;
    }

    //check if all pointers are not NULL
    list_elem = list->head;
    while (list_elem) {
        if (list_elem != list->tail && list_elem->next == NULL) {
            return LIST_ERR;
        }

        list_elem = list_elem->next;
    }

    return ALL_RIGHT;
}

int List_Add_First (struct List *list, data_t value) {
    int err_num = List_Ok(list);
    if (err_num != ALL_RIGHT) {
        return err_num;
    }

    struct Node *node = Node_Construct(value);
    if (node == NULL) {
        return CONSTRUCT_ERR;
    }

    if (list->len == 0) {
        list->head = node;
        list->tail = node;
    }
    else {
        node->next = list->head;
        list->head = node;
    }

    list->len++;

    return ALL_RIGHT;
}

int List_Add_Last (struct List *list, data_t value) {
    int err_num = List_Ok(list);
    if (err_num != ALL_RIGHT) {
        return err_num;
    }

    struct Node *node = Node_Construct(value);
    if (node == NULL) {
        return CONSTRUCT_ERR;
    }

    if (list->len == 0) {
        list->head = node;
        list->tail = node;
    }
    else {
        list->tail->next = node;
        list->tail = node;
    }

    node->next = NULL;
    list->len++;

    return ALL_RIGHT;
}

int List_Insert (struct List *list, data_t value, size_t n) {
    int err_num = List_Ok(list);
    if (err_num != ALL_RIGHT) {
        return err_num;
    }

    if (n > list->len) {
        return ARG_ERR;
    }
    else if (n == 0) {
        return List_Add_First(list, value);
    }
    else if (n == list->len) {
        return List_Add_Last(list, value);
    }

    struct Node *new_node = Node_Construct(value);
    struct Node *list_elem = List_n_Find(list, n - 1);

    new_node->next = list_elem->next;
    list_elem->next = new_node;

    list->len++;

    return ALL_RIGHT;
}

int List_Remove_n_Node (struct List *list, size_t n) {
    int err_num = List_Ok(list);
    if (err_num != ALL_RIGHT) {
        return err_num;
    }

    if (n > list->len - 1) {
        return ARG_ERR;
    }

    if (n == 0) {
        struct Node *new_head = list->head->next;
        Node_Destruct(&list->head);
        list->head = new_head;

        list->len--;

        return ALL_RIGHT;
    }
    else if (n == list->len - 1) {
        struct Node *new_tail = List_n_Find(list, n - 1);
        Node_Destruct(&list->tail);
        list->tail = new_tail;
        list->tail->next = NULL;

        list->len--;

        return ALL_RIGHT;
    }

    struct Node *prev_node = List_n_Find(list, n - 1);
    struct Node *node = prev_node->next;

    prev_node->next = node->next;

    Node_Destruct(&node);

    list->len--;

    return ALL_RIGHT;
}

struct Node *List_Find (struct List *list, data_t value) {
    if (List_Ok(list) != ALL_RIGHT) {
        return NULL;
    }

    struct Node *list_elem = list->head;

    while (list_elem != NULL) {
        if (strcmp(value, list_elem->data) == 0) {                  //strcmp for strings(char *-s) is important!
            return list_elem;
        }
        list_elem = list_elem->next;
    }

    return NULL;
}

struct Node *List_n_Find (struct List *list, size_t n) {
    if (List_Ok(list) != ALL_RIGHT) {
        return NULL;
    }

    if (n > list->len - 1) {
        return NULL;
    }

    struct Node *node = list->head;
    for (int i = 0; i < n; i++) {
        node = node->next;
    }

    return node;
}

size_t List_Count_Len (struct List *list) {
    size_t length = 0;
    List_Iterate(list, count_len, &length);
    return length;
}

int List_Iterate (struct List *list, void * (*func) (struct Node *node, va_list ), ...) {
    va_list args;
    struct Node *node = list->head;
    while (node != NULL) {
        va_start(args, func);
        func(node, args);
        va_end(args);
        node = node->next;
    }

    return 0;
}

int List_Dump (struct List *list) {
    FILE *Dump_f = fopen("Dump.txt", "w");
    fprintf(Dump_f, "digraph List {\nhead [label = \"List\\nList len = %d\"];\n", list->len);

    struct Node *elem = list->head;
    int i = 2;

    if (elem == NULL) {
        fprintf(Dump_f, "}");
        fclose(Dump_f);

        system ("dot -Tpng Dump.txt -o Dump.png");
        //system("Dump.png");
        return ALL_RIGHT;
    }

    fprintf(Dump_f, "node1 [shape=record, label=\"{<name> node1\\n%p|"
            "<data> data:\\ %s|<next> next\\n%p}}\"];\n", elem, elem->data, elem->next);
    while (elem) {
        elem = elem->next;
        if (elem == NULL) {
            break;
        }
        fprintf(Dump_f, "node%d [shape=record, label=\"{<name> node%d\\n%p|"
                "<data> data:\\ %s|<next> next\\n%p}}\"];\n", i, i, elem, elem->data, elem->next);
        i++;
    }

    i = 1;
    //fprintf(Dump_f, "node [style = filled color = red;\n");
    fprintf(Dump_f, "edge [color = deepskyblue];\n");
    i = 1;
    while (i < list->len && list->len != 1) {
        fprintf(Dump_f, "\"node%d\":next -> \"node%d\":name;\n", i, i+1);
        i++;
    }

    fprintf(Dump_f, "}");
    fclose(Dump_f);

    system ("dot -Tpng Dump.txt -o Dump.png");
    //system("Dump.png");
}

// int List_Cat (struct List *list1, struct List *list2) {
//     int err_num = List_Ok(list1);
//     if (err_num != ALL_RIGHT) {
//         return err_num;
//     }
//     err_num = List_Ok(list2);
//     if (err_num != ALL_RIGHT) {
//         return err_num;
//     }

//     list1->tail->next = list2->head;

//     err_num = List_Ok(list1);
//     if (err_num != ALL_RIGHT) {
//         return err_num;
//     }
//     err_num = List_Ok(list2);
//     if (err_num != ALL_RIGHT) {
//         return err_num;
//     }
// }
//
// int List_AddAfter (struct List *list, struct Node *node, data_t value) {
//     int err_num = List_Ok(list);
//     if (err_num != ALL_RIGHT) {
//         return err_num;
//     }

//     if (node == list->tail) {
//         List_AddLast(list, value);
//     }
//     else {
//         struct Node *new = Node_Construct(value);

//         new->next = node->next;
//         node->next = new;
//     }

//     list->len++;

//     int err_num = List_Ok(list);
//     if (err_num != ALL_RIGHT) {
//         return err_num;
//     }
// }

// int List_Remove_Node (struct List *list, struct Node **node) { // better use List_Remove_n_Node
//     if (*node == NULL || list->len == 0) {
//         return ARG_ERR:
//     }

//     int err_num = List_Ok(list);
//     if (err_num != ALL_RIGHT) {
//         return err_num;
//     }

//     if (list->len == 1) {
//         list->tail = NULL;
//         list->head = NULL;
//     }
//     else if (*node == list->head) {
//         list->head = list->head->next;
//     }
//     else if (*node == list->tail) { //actually works perfectly in else
//         // list->tail = list->tail->prev;
//         // list->tail->next = NULL;
//         //1. Find list->tail->prev
//         //2. list->tail = list->tail->prev
//     }
//     else {//and its too
//         // (*node)->next->prev = (*node)->prev;
//         // (*node)->prev->next = (*node)->next;
//         //1. Find node->prev
//         //2. node->prev->next = node->next
//     }

//     Node_Destruct(node);
//     list->len--;

//     err_num = List_Ok(list);
//     if (err_num != ALL_RIGHT) {
//         return err_num;
//     }
// }