import pathlib
import numpy
import os
import subprocess
import sys

if len(sys.argv) == 2 and sys.argv[1] == "fast" or len(sys.argv) == 1:
        shapes = [(1,), (2,), (3,), (1, 1), (2, 3), (2, 3, 5), (2, 3, 5, 7)]
elif len(sys.argv) == 2 and sys.argv[1] == "slow":
        shapes = [(1,), (2,), (3,), (12094,), (1, 1), (1, 2), (2, 2), (2, 3), (3, 3), (53, 661), (1, 1, 1), (1, 1, 2), (1, 2, 3), (2, 2, 2), (2, 3, 5), (3, 3, 3), (1, 1, 1, 1), (1, 1, 1, 2), (1, 1, 2, 3), (1, 2, 3, 5), (2, 2, 2, 2), (2, 3, 5, 7), (5, 7, 11, 13)]
else:
    print(f"Usage: {sys.argv[0]} [fast|slow]", file=sys.stderr)
    exit(1)

cc_args = [os.environ.get("CC", default="cc"), "-W", "-Wall", "-Werror", "-Wfatal-errors", "-Wno-unused-function", "-I../../include/", "-lm"]
if os.uname().sysname != "OpenBSD":
    cc_args.append("-fsanitize=address,undefined")
if "CFLAGS" in os.environ.keys():
    cc_args.append(os.environ.get("CFLAGS"))
if "LDFLAGS" in os.environ.keys():
    cc_args.append(os.environ.get("LDFLAGS"))

test_folder = pathlib.Path("tests")
test_folder.mkdir(exist_ok=True)

def dtype_to_c_types(dtype):
    if dtype == "?":
        return "CNPY_B", "bool", "b"
    elif dtype == "i1":
        return "CNPY_I1", "int8_t", "i1"
    elif dtype == "i2":
        return "CNPY_I2", "int16_t", "i2"
    elif dtype == "i4":
        return "CNPY_I4", "int32_t", "i4"
    elif dtype == "i8":
        return "CNPY_I8", "int64_t", "i8"
    elif dtype == "u1":
        return "CNPY_U1", "uint8_t", "u1"
    elif dtype == "u2":
        return "CNPY_U2", "uint16_t", "u2"
    elif dtype == "u4":
        return "CNPY_U4", "uint32_t", "u4"
    elif dtype == "u8":
        return "CNPY_U8", "uint64_t", "u8"
    elif dtype == "f4":
        return "CNPY_F4", "float", "f4"
    elif dtype == "f8":
        return "CNPY_F8", "double", "f8"
    elif dtype == "c8":
        return "CNPY_C8", "complex float", "c8"
    elif dtype == "c16":
        return "CNPY_C16", "complex double", "c16"
    else:
        raise ValueError(f"Unknown dtype '{dtype}'")

def byte_order_to_c_byte_order(byte_order):
    if byte_order == "<":
        return "CNPY_LE"
    if byte_order == ">":
        return "CNPY_BE"
    if byte_order == "|":
        return "CNPY_NE"
    else:
        raise ValueError(f"Unknown byte order {byte_order}")

def order_to_c_order(order):
    if order == "c":
        return "CNPY_C_ORDER"
    elif order == "f":
        return "CNPY_FORTRAN_ORDER"
    else:
        raise ValueError(f"Unknown order {order}")

def prod(xs):
    i = 1
    for x in xs:
        i *= x
    return i

def is_effectively_1d(shape):
    number_of_dimensions_larger_than_one = len([d for d in shape if d > 1])
    return number_of_dimensions_larger_than_one < 2

for dtype in "?", "i1", "i2", "i4", "i8", "u1", "u2", "u4", "u8", "f4", "f8", "c8", "c16":
    if dtype in ("?", "i1", "u1"):
        byte_orders = ("|",)
    else:
        byte_orders = ("<", ">")
    for byte_order in byte_orders:
        for shape in shapes:
            if is_effectively_1d(shape):
                orders = "c",
            else:
                orders = ("f", "c")
            for order in orders:
                print(f"{dtype} {byte_order}, {shape} {order}")
                x = numpy.arange(0, prod(shape))
                arr = numpy.asarray(x.reshape(shape, order=order), dtype=f"{byte_order}{dtype}", order=order)

                assert((arr.flags.carray == (order == "c")) and (arr.flags.farray == (order == "f")))

                # Clean files
                for x in "1.npy", "2.npy", "3.npy", "4.npy", "test1.c", "test2.c", "test3.c", "test1", "test2", "test3":
                    try:
                        pathlib.Path(test_folder / x).unlink()
                    except FileNotFoundError:
                        pass

                # check roundtrip numpy -> cnpy -> cnpy -> numpy
                numpy.save(test_folder / "1.npy", arr)
                with open(test_folder / "test1.c", "w") as f:
                    print(f"""
#include <stdio.h>
#include <assert.h>
#include "cnpy.h"

int main(void) {{
  cnpy_array a_in, a_out;
  assert(cnpy_open("{test_folder}/1.npy", false, &a_in) == CNPY_SUCCESS);
  assert(cnpy_create("{test_folder}/2.npy", a_in.byte_order, a_in.dtype, a_in.order, a_in.n_dim, a_in.dims, &a_out) == CNPY_SUCCESS);

  assert(a_in.byte_order == {byte_order_to_c_byte_order(byte_order)});
  assert(a_in.dtype == {dtype_to_c_types(dtype)[0]});
  assert(a_in.order == {order_to_c_order(order)});
  assert(a_in.n_dim == {len(shape)});
  {" ".join(["assert(a_in.dims[{}] == {});".format(i, shape[i]) for i in range(len(shape))])}

  assert(a_out.byte_order == {byte_order_to_c_byte_order(byte_order)});
  assert(a_out.dtype == {dtype_to_c_types(dtype)[0]});
  assert(a_out.order == {order_to_c_order(order)});
  assert(a_out.n_dim == {len(shape)});
  {" ".join(["assert(a_out.dims[{}] == {});".format(i, shape[i]) for i in range(len(shape))])}

  size_t index[CNPY_MAX_DIM];
  cnpy_reset_index(a_in, index);
  size_t i = 0;
  do {{
    {dtype_to_c_types(dtype)[1]} x = cnpy_get_{dtype_to_c_types(dtype)[2]}(a_in, index);
    assert(x == ({dtype_to_c_types(dtype)[1]}) i);
    cnpy_set_{dtype_to_c_types(dtype)[2]}(a_out, index, x);

    i += 1;
  }} while (cnpy_next_index(a_in, index));
  assert(i == {prod(shape)});
}}
""", file=f)
                if subprocess.run(cc_args + [str(test_folder / "test1.c"), "-o", str(test_folder / "test1")]).returncode != 0:
                    raise RuntimeError(f"Compilation of test1 failed")
                if subprocess.run([str(test_folder / "test1")]).returncode != 0:
                    raise RuntimeError(f"Execution of test1 failed")
                arr2 = numpy.load(test_folder / "2.npy")
                assert(arr2.dtype == arr.dtype)
                assert(arr2.shape == shape)
                assert(arr2.flags.carray == arr.flags.carray)
                assert(arr2.flags.farray == arr.flags.farray)
                assert(numpy.all(arr2 == arr))

                print(" round trip numpy -> cnpy -> numpy ok");

                # check roundtrip cnpy -> cnpy
                with open(test_folder / "test2.c", "w") as f:
                    print(f"""
#include <stdio.h>
#include <assert.h>
#include "cnpy.h"

int main(void) {{
  cnpy_array a_out;
  size_t dims[CNPY_MAX_DIM];
  {" ".join(["dims[{}] = {};".format(i, shape[i]) for i in range(len(shape))])}
  assert(cnpy_create("{test_folder}/3.npy", {byte_order_to_c_byte_order(byte_order)}, {dtype_to_c_types(dtype)[0]}, {order_to_c_order(order)}, {len(shape)}, dims, &a_out) == CNPY_SUCCESS);

  assert(a_out.byte_order == {byte_order_to_c_byte_order(byte_order)});
  assert(a_out.dtype == {dtype_to_c_types(dtype)[0]});
  assert(a_out.order == {order_to_c_order(order)});
  assert(a_out.n_dim == {len(shape)});
  {" ".join(["assert(a_out.dims[{}] == {});".format(i, shape[i]) for i in range(len(shape))])}

  size_t index[CNPY_MAX_DIM];
  cnpy_reset_index(a_out, index);
  size_t i = 0;
  do {{
    cnpy_set_{dtype_to_c_types(dtype)[2]}(a_out, index, ({dtype_to_c_types(dtype)[1]}) i);
    i += 1;
  }} while (cnpy_next_index(a_out, index));
  assert(i == {prod(shape)});
}}
""", file=f)
                if subprocess.run(cc_args + [str(test_folder / "test2.c"), "-o", str(test_folder / "test2")]).returncode != 0:
                    raise RuntimeError(f"Compilation of test2 failed")
                if subprocess.run([str(test_folder / "test2")]).returncode != 0:
                    raise RuntimeError(f"Execution of test2 failed")
                with open(test_folder / "2.npy", "rb") as f1, open(test_folder / "3.npy", "rb") as f2:
                    assert(f1.read() == f2.read())

                with open(test_folder / "test3.c", "w") as f:
                    print(f"""
#include <stdio.h>
#include <assert.h>
#include "cnpy.h"

int main(void) {{
  cnpy_array a_in;
  assert(cnpy_open("{test_folder}/3.npy", false, &a_in) == CNPY_SUCCESS);

  assert(a_in.byte_order == {byte_order_to_c_byte_order(byte_order)});
  assert(a_in.dtype == {dtype_to_c_types(dtype)[0]});
  assert(a_in.order == {order_to_c_order(order)});
  assert(a_in.n_dim == {len(shape)});
  {" ".join(["assert(a_in.dims[{}] == {});".format(i, shape[i]) for i in range(len(shape))])}

  size_t index[CNPY_MAX_DIM];
  cnpy_reset_index(a_in, index);
  size_t i = 0;
  do {{
    {dtype_to_c_types(dtype)[1]} x = cnpy_get_{dtype_to_c_types(dtype)[2]}(a_in, index);
    assert(x == ({dtype_to_c_types(dtype)[1]}) i);

    i += 1;
  }} while (cnpy_next_index(a_in, index));
  assert(i == {prod(shape)});
}}
""", file=f)
                if subprocess.run(cc_args + [str(test_folder / "test3.c"), "-o", str(test_folder / "test3")]).returncode != 0:
                    raise RuntimeError(f"Compilation of test3 failed")
                if subprocess.run([str(test_folder / "test3")]).returncode != 0:
                    raise RuntimeError(f"Execution of test3 failed")

                print(" round trip cnpy -> cnpy ok")
