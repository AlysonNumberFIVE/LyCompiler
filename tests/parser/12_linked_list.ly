// A linked list using Item nodes.
// malloc and free are assumed to exist as built-in functions.
// Casting is implicit.

struct Item {
    name char*;
    next Item*;
}

// Allocate and initialise a new Item node.
func new_item(name char*) -> Item* {
    var node Item* = malloc(8);
    node->name = name;
    node->next = 0;
    return node;
}

// Append a new Item to the tail of the list.
// Returns the (unchanged) head of the list,
// or the new node if the list was empty.
func append(head Item*, name char*) -> Item* {
    var node Item* = new_item(name);
    if (head == 0) {
        return node;
    }
    var current Item* = head;
    while (current->next != 0) {
        current = current->next;
    }
    current->next = node;
    return head;
}

// Return the length of the list.
func length(head Item*) -> i64 {
    var current Item* = head;
    var count i64 = 0;
    while (current != 0) {
        count = count + 1;
        current = current->next;
    }
    return count;
}

// Walk the list and print each name.
func print_list(head Item*) -> i64 {
    var current Item* = head;
    while (current != 0) {
        printf(current->name);
        current = current->next;
    }
    return 0;
}

// Free every node in the list.
func free_list(head Item*) -> i64 {
    var current Item* = head;
    var tmp Item*;
    while (current != 0) {
        tmp = current->next;
        free(current);
        current = tmp;
    }
    return 0;
}
