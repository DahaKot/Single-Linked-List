#include "list.h"

//what if there are many conditions in if block?
//error from node construct

int Test();
int Test1();
int Test2();
int Test3();
int Test4();
int Test5();

int main() {
    Test();

    return 0;
}

int Test() {
    Test1()     ? printf("TEST1 FAILED\n") : printf("TEST1 PASSED\n");
    Test2()     ? printf("TEST2 FAILED\n") : printf("TEST2 PASSED\n");
    Test3()     ? printf("TEST3 FAILED\n") : printf("TEST3 PASSED\n");
    Test4()     ? printf("TEST4 FAILED\n") : printf("TEST4 PASSED\n");
    Test5()     ? printf("TEST5 FAILED\n") : printf("TEST5 PASSED\n");
    return 0;   
}

//Test1: basic functions
int Test1() {
    struct List *list = List_Construct();
    if (list == NULL) {
        return 1;
    }
    List_Dump(list);

    int err = 0;

    err += List_Add_Last(list, "Hello");
    err += List_Add_Last(list, "world");
    err += List_Insert(list, "!", 2);

    if (List_Count_Len(list) != list->len) {
        err++;
    }
    
    err += List_Remove_n_Node(list, 2);
    err += List_Remove_n_Node(list, 0);
    err += List_Insert(list, "!", 1);
    err += List_Add_First(list, "Hello");
    err += List_Add_First(list, "Im new list and i wanna say: ");
    err += List_Remove_n_Node(list, 2);

    if (List_Find(list, "!") == NULL ||
        strcmp(List_Find(list, "!")->data, "!") != 0) {
        err++;
    }

    List_Iterate(list, node_print);

    List_Dump(list);

    List_Destruct(&list);

    return err;
}

//Test2: invalid arg: list
int Test2() {
    struct List *list = List_Construct();
    List_Destruct(&list);
    
    int err = 1;
    err *= List_Add_Last(list, "\n");
    err *= List_Add_First(list, "\t");
    err *= List_Remove_n_Node(list, 2);
    err *= List_Insert(list, "hey", 5);

    //these functions return pointers
    if (List_Find(list, "Hello")) {
        err *= 2;
    }
    if (List_n_Find(list, 3)) {
        err *= 2;
    }
    
    return !err;
}

//Test3: certain assigments to certain pointers
int Test3() {
    struct List *err_list = List_Construct();
    if (err_list == NULL) {
        return 1;
    }
    List_Add_Last(err_list, "hey");

    //list->len == 1 && (list->head != list->tail)
    err_list->tail = NULL;
    int err = List_Ok(err_list);
    if (!err) {
        err_list->tail = err_list->head;
        List_Destruct(&err_list);

        return err;
    }
    //back to normal; save temp not to lose memory
    err_list->tail = err_list->head;
    List_Add_Last(err_list, "you");
    struct Node *temp = err_list->head->next;

    //check if all pointers are not NULL
    err_list->head->next = NULL;
    err = List_Ok(err_list);
    if (!err) {
        err_list->head->next = temp;
        List_Destruct(&err_list);

        return err;
    }

    //back to normal
    err_list->head->next = temp;
    List_Add_Last(err_list, "!");
    //list->len != 0 && list->tail == NULL))
    err_list->tail->next = (struct Node *) 7;
    err = List_Ok(err_list);
    if (!err) {
        err_list->tail->next = NULL;
        List_Destruct(&err_list);

        return err;
    }
    err_list->tail->next = NULL;

    List_Destruct(&err_list);
    List_Destruct(&err_list);//yes, twice

    return 0;
}

//Test4: invalid arguments
int Test4() {
    struct List *err_list = List_Construct();
    if (err_list == NULL) {
        return 1;
    }
    //next three has already been tested
    List_Insert(err_list, "hey", 0);
    List_Add_First(err_list, "!");
    List_Insert(err_list, "aaaa", 1);

    int err = 1;
    err *= List_Insert(err_list, "you", 100);//*=3
    err *= List_Remove_n_Node(err_list, 100);//*=3

    //these functions return pointers
    if (List_n_Find(err_list, 100)) {
        err *= 2;
    }
    if (List_Find(err_list, ",,,")) {
        err *= 2;
    }

    List_Destruct(&err_list);

    return err != 9;
}

//Test5: loop
int Test5() {
    struct List *err_list = List_Construct();
    if (err_list == NULL) {
        return 1;
    }
    //next three has already been tested
    List_Insert(err_list, "hey", 0);
    List_Add_First(err_list, "!");
    List_Insert(err_list, "aaaa", 1);
    List_Insert(err_list, "aaaa", 1);
    
    struct Node *temp = err_list->head->next->next;
    err_list->head->next->next = err_list->head;
    int err = List_Ok(err_list);
    if (err != LIST_ERR) {
        return 1;
    }   

    err_list->head->next->next = temp;
    List_Destruct(&err_list); 
    return 0;
}