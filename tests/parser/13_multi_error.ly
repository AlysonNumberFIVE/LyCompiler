func 99problems() -> i64 {
    return 0;
}

func bad_paren -> i64 {
    return 0;
}

func missing_arrow(x i64) i64 {
    return x;
}

func no_brace(x i64) -> i64
    return x;
}

func bad_return_type(x i64) -> 42 {
    return 0;
}

func ;

func after_semicolon(x i64) -> i64 {
    return x;
}

func totally_broken(x i64) {
    return x;
}

func wrong_return_kw(x i64) return i64 {
    return x;
}

func skip_params -> i64 {
    return 0;
}

func 0xdeadbeef(y i64) -> i64 {
    return y;
}

func clean_finale(a i64) -> i64 {
    return a;
}
