// Tests: struct member access via -> and .
func test_member(node i64) -> i64 {
    var val i64 = node->value;
    var len i64 = node->inner->length;
    return val;
}
