import pytest

import pyarrow as pa
import numpy as np

from arrow_ext import ext


def test_madd():
    a = pa.array([1, 2, 3], type=pa.float64())
    b = pa.array([0, 2, 4], type=pa.float64())
    c = pa.array([1, 1, 5], type=pa.float64())
    assert ext.madd(a, b, c).to_pylist() == [1, 4, 23]

    with pytest.raises(ValueError):
        assert ext.madd(a[1:], b, c).to_pylist() == [1, 4, 23]
    print("test1 done")


def test_sum():
    x = pa.array([1,2,3.1])
    assert ext.sum(x) == np.sum(np.array(x))
    assert ext.sum(x[1:]) == np.sum(np.array(x[1:]))
    print("test2 done")

print("start test")
test_madd()
test_sum()
