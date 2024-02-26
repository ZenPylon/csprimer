#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *cvarint_encode(PyObject *self, PyObject *args)
{
    long long n;
    if (!PyArg_ParseTuple(args, "i", &n))
        return NULL;

    long long part;
    int i = 0;

    // Varint specifies max size of 10-bytes
    char encoded_bytes[10];
    while (n > 0)
    {
        part = n & 0x7f;
        n >>= 7;
        if (n)
            part |= 0x80;

        encoded_bytes[i] = part;
        i++;
    }
    return Py_BuildValue("y#", encoded_bytes, i)
}

static PyObject *cvarint_decode(PyObject *self, PyObject *args)
{
}

static PyMethodDef CVarintMethods[] = {
    {"encode", cvarint_encode, METH_VARARGS, "Encode an integer as varint."},
    {"decode", cvarint_decode, METH_VARARGS,
     "Decode varint bytes to an integer."},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef cvarintmodule = {
    PyModuleDef_HEAD_INIT, "cvarint",
    "A C implementation of protobuf varint encoding", -1, CVarintMethods};

PyMODINIT_FUNC PyInit_cvarint(void) { return PyModule_Create(&cvarintmodule); }
