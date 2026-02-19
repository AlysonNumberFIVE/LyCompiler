struct Node {
    var id i64;
    var value i64;
    var next i64; // This will be a pointer in practice
};

func process() -> void {
    struct Node n = Node{id: 1, value: 500, next: 0};
    var ptr Node* = &n;
    
    // Testing the pointer dereference and dot access
    ptr->value = 1000;
}
