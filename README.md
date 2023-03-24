# MSVC ASAN Bug Demo

This is a demo for an apparent bug with the address sanitizer of MSVC. It is essentially a trimmed-down version of [DynaMix](https://github.com/iboB/dynamix) to the minimal state where the bug is reproducible.

The associated issue is reported and tracked [here](https://developercommunity.visualstudio.com/t/Address-sanitizer-in-Release-may-introdu/10314256?)

The bug only manifests in MSVC Release builds (+ `/fsanitize=address` obviously).

As seen in workflow any other combination does not trigger it.

## Notes

These are some observations while trimming down the code:

* `c-scratch.c`: if we comment-out line 22, the issue disappears. Line 22 calls a function which throws and exception, which is caught and trasformed to an error return for C.
* `domain.cpp`: if we replace the instances of `std::pmr::vector` with regular `std::vector`, the issue disappears. It seems that the default memory resource has a play in this.
* `domain.cpp`: if we remove the instantiation of `basic_register_l` for `mixin_info` (basically comment out line 113), the issue disappears. Note that this instantiation is never invoked in the demo.
* `domain.cpp`: if we don't remove the instantiation, but remove the `bool` argument of this function, the issue disappears. The bool is always true in the demo.
* `domain.cpp`: if we don't remove the instantiation and don't remove the `bool` argument, but remove the `BAD_ARG_UNLESS` macro at the top of the function, the issue disappears. The macro checks a condition and throws an exception if the condition is false, but it's always true in the demo.
* `domain.cpp`: if we remove the `data_mutex` for `registry` and replace it with a simple instance (switch comments of lines 41 and 42, and remove all calls to `shared_lock` and `unique_lock`), the string lengths are correct, but the address sanitizer reports an error on exit. Perhaps this is a more useful state to test the app?
