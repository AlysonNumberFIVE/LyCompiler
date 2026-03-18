// Tests: if / else if / else chains
func test_if(x i64) -> i64 {
    if (x == 42) {
        var a i64 = 1;
    } else if (x == 0) {
        var b i64 = 2;
    } else {
        var c i64 = 3;
    }
    return x;
}
