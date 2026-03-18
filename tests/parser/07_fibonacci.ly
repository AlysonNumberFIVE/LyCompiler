// Tests: fibonacci – var declarations, while loop with assignments, if/else, return
func fibonacci(n i64) -> i64 {
    var a i64 = 0;
    var b i64 = 1;
    var tmp i64;
    var i i64 = 0;
    while (i == n) {
        tmp = b;
        b = a + b;
        a = tmp;
        i = i + 1;
    }
    return a;
}
