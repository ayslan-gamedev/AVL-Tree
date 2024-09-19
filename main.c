/// This is a program made by Ayslan Vieira Fontes -> mat 2023 0329 8481
/// this programs implement a basic AVL Tree to use for example.
/// This program uses MIT License, so all right be reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// The data used in tree
struct data {
  // int this example, VALUE will be the primary key, (just because work with
  // int is more easy)
  int value;
  char name[255];
};

/// The Tree node
struct node {
  int height;
  struct data data;
  struct node *left;
  struct node *right;
};

int get_height(struct node *N) {
  if (N == NULL)
    return 0;
  return N->height;
}

int set_height(struct node *N) {
  return N->height = 1 + (get_height(N->left) > get_height(N->right)
                              ? get_height(N->left)
                              : get_height(N->right));
}

int get_balance(struct node *N) {
  if (N == NULL)
    return 0;
  return get_height(N->left) - get_height(N->right);
}

/// exchange y for x
struct node *rotate_right(struct node *y) {
  struct node *x = y->left;
  struct node *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = set_height(y);
  x->height = set_height(x);

  return x;
}

/// exchange x for y
struct node *rotate_left(struct node *x) {
  struct node *y = x->right;
  struct node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = set_height(x);
  y->height = set_height(y);

  return y;
}

struct node *balance(struct node *root) {
  root->height = set_height(root);

  int balance = get_balance(root);

  // case 1, imbalance on the left
  if (balance > 1 && get_balance(root->left) >= 0) {
    return rotate_right(root);
  }

  // case 2, imbalance on the right
  if (balance < -1 && get_balance(root->right) <= 0) {
    return rotate_left(root);
  }

  // case 3, imbalance on the left/right
  if (balance > 1 && get_balance(root->left) < 0) {
    root->left = rotate_left(root);
    return rotate_right(root);
  }

  // case 4, imbalance on the right/left
  if (balance < -1 && get_balance(root->right) > 0) {
    root->right = rotate_right(root);
    return rotate_left(root);
  }

  return root;
}

/// Used to start a new tree
struct node *create_node(struct data data) {
  struct node *new_node = (struct node *)malloc(sizeof(struct node));
  if (new_node == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  // insert the values
  new_node->data = data;
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->height = 1;

  return new_node;
}

/// insert new data on the tree
struct node *insert_node(struct node *root, struct data data) {
  if (root == NULL) {
    return create_node(data);
  }

  if (data.value < root->data.value) {
    root->left = insert_node(root->left, data);
  } else {
    root->right = insert_node(root->right, data);
  }

  return balance(root);
}

/// search for a value (uses primary key `value`)
struct node *search_node(struct node *root, int value) {
  if (root == NULL) {
    return NULL;
  }

  // found the value
  if (root->data.value == value) {
    return root;
  }

  // search for the value (Left first, Right, and NULL case unfound)
  if (value < root->data.value && root->left != NULL) {
    return search_node(root->left, value);
  } else if (value > root->data.value && root->right != NULL) {
    return search_node(root->right, value);
  } else {
    return NULL;
  }
}

/// Delete data from the tree
struct node *delete_node(struct node *root, int value) {
  if (root == NULL) {
    return root;
  }

  // search for the value (Left first, right, and founded)
  if (value < root->data.value) {
    return delete_node(root->left, value);
  } else if (value > root->data.value) {
    return delete_node(root->right, value);
  } else {
    // case 1, sample elimination (L R)
    if (root->left == NULL) {
      struct node *tmp = root->right;
      free(root);
      return tmp;
    } else if (root->right == NULL) {
      struct node *tmp = root->left;
      free(root);
      return tmp;
    }

    // case 2, complex alimination (two nodes)
    struct node *tmp = root->right;
    while (tmp && tmp->left != NULL) {
      tmp = tmp->left;
    }

    root->data = tmp->data;
    root->right = delete_node(root->right, value);
  }

  return balance(root);
}

/// Delete all the tree from memory
void free_tree(struct node *root) {
  if (root == NULL)
    return;

  // L R Current
  free_tree(root->left);
  free_tree(root->right);
  free(root);
}

/// END OF THE SYSTEM
/// FORM HERE ITS JUST UI

// use just for tests, ignored in a real case
void print_tree(struct node *root) {
  if (root->right != NULL) {
    printf("R: ");
    print_tree(root->right);
  }

  if (root->left != NULL) {
    printf("L: ");
    print_tree(root->left);
  }

  printf("%s (%d)\n", root->data.name, root->data.value);
}

/// UI to create new data
struct data create_data() {
  struct data tmp;

  printf("Insert name: ");
  fgets(tmp.name, sizeof(tmp.name), stdin);
  tmp.name[strcspn(tmp.name, "\n")] = '\0';

  printf("Insert a random INT: ");
  scanf("%d", &tmp.value);

  return tmp;
}

int main(void) {
  struct node *root = NULL;
  struct data tmp;
  int choice;
  int tmp_value;

  while (1) {
    printf("\nMenu:\n");
    printf("1. Insert data\n");
    printf("2. Search in tree\n");
    printf("3, Delete a data in tree\n");
    printf("4, Print all the tree\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar();

    switch (choice) {
    case 1:
      tmp = create_data();
      root = insert_node(root, tmp);
      break;
    case 2:
      printf("Select a number to search in tree:\n");
      scanf("%d", &tmp_value);
      struct node *found_node = search_node(root, tmp_value);

      if (found_node != NULL) {
        printf("Found: %s with value %d\n", found_node->data.name,
               found_node->data.value);
      } else {
        printf("Value %d not found in the tree.\n", tmp_value);
      }

      break;
    case 3:
      printf("Insert a value to delete from tree:");
      scanf("%d", &tmp_value);
      root = delete_node(root, tmp_value);
      break;

    case 4:
      print_tree(root);
      break;
    case 5:
      printf("Exiting...\n");
      exit(0);
    default:
      printf("Invalid choice. Please try again.\n");
    }
  }

  free_tree(root);
  return EXIT_SUCCESS;
}
/// END OF THE PROGRAM
