// Tests: function calls – multi-arg and single-arg
func test_calls(x i64, y i64) -> i64 {
    foo(x, 42);
    bar(x);
    return x;
}
