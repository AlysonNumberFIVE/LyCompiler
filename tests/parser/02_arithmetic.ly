// Tests: binary expressions – arithmetic, comparison, operator precedence
func test_expr(a i64, b i64) -> i64 {
    var c i64 = a + b * 2;
    var d i64 = a - b + 1;
    var e i64 = a == b;
    return c;
}
