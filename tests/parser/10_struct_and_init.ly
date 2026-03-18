struct Point {
    x i64;
    y i64;
}

func make_point(a i64, b i64) -> i64 {
    var p i64 = @Point{x: a, y: b};
    var origin i64 = @Point{x: 0, y: 0};
    return a;
}
