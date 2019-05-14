import numpy
import pathlib

folder = pathlib.Path("files")
folder.mkdir(exist_ok=True)

def prod(xs):
    i = 1
    for x in xs:
        i *= x
    return i

for dtype in "?", "i1", "i2", "i4", "i8", "u1", "u2", "u4", "u8", "f4", "f8", "c8", "c16":
    for order in "C", "F":
        for shape in (1,), (3,), (1, 2), (2, 2), (2, 3), (1, 1, 2), (1, 2, 3), (2, 2, 2), (2, 3, 5), (1, 1, 1, 2), (1, 1, 2, 3), (1, 2, 3, 5), (2, 2, 2, 2, 2), (2, 3, 5, 7), (1, 1, 1, 1, 1), (2, 2, 2, 2, 2):
            for byte_order in "<", ">":
                dt = f"{byte_order}{dtype}"
                arr = numpy.asarray(numpy.arange(0, prod(shape)).reshape(shape), dtype=f"{byte_order}{dtype}", order=order)
                numpy.save(folder / f"{dtype}_{byte_order}_{order}_{shape}.npy", arr)
