// Tests: variable declarations – no initializer, int initializer, pointer with string
func test_vars(x i64) -> i64 {
    var a i64;
    var b i64 = 42;
    var s char* = "hello";
    return b;
}
