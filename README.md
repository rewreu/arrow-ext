# pyarrow-pybind
This used https://github.com/vaexio/vaex-arrow-ext as a template


# Error compiling or at runtime
1. When compiling shows the following error:
```
/usr/bin/ld: cannot find -larrow
/usr/bin/ld: cannot find -larrow_python
```
Checkout if libarrow.so and libarrow_python.so are in folder pa.get_library_dirs().
If they are named as libarrow.so.500 and libarrow_python.so.500, make a copy of the right namez