# MSVC ASAN Bug Demo

This is a demo for an apparent bug with the address sanitizer of MSVC. It is essentially a trimmed-down version of [DynaMix](https://github.com/iboB/dynamix) to the minimal state where the bug is reproducible.

The associated issue is reported and tracked [here](https://developercommunity.visualstudio.com/t/Address-sanitizer-in-Release-may-introdu/10314256?)

The bug only manifests in MSVC Release builds (+ `/fsanitize=address` obviously).

As seen in workflow any other combination does not trigger it.