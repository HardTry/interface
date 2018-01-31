#ifdef _WINDOWS
#define MS_NO_COREDLL
#endif


#ifndef SHARED_LIBRARY_EXPORT
#  ifdef _WINDOWS
#    define SHARED_LIBRARY_EXPORT     __declspec(dllexport)
#    define SHARED_LIBRARY_IMPORT     __declspec(dllimport)
#  elif defined(_UNIX)
#    define SHARED_LIBRARY_EXPORT
#    define SHARED_LIBRARY_IMPORT
#    define SHARED_LIBRARY_HIDDEN
#  endif
#endif


#if defined(CPY1_LIBRARY)
#  define CPY1_API SHARED_LIBRARY_EXPORT
#else
#  define CPY1_API SHARED_LIBRARY_IMPORT
#endif


#include <Python.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

int CPY1_API run_multiply(void) //PyObject* pFunc, const int p1, const int p2)
{
    // Py_Initialize();
    // printf("Hello World %lx, %d, %d\n", (uint64_t)pFunc, p1, p2);

    PyObject *pArgs = 0, *pValue1 = 0, *pValue2 = 0;

    // printf ("PyTuple_New ... ");
    // pArgs = PyTuple_New(2);
    // printf (" OK\n");

    int pp1 = 2, pp2 = 3;
    printf ("PyInt_FromLong(p1) ... \n");
    pValue1 = PyInt_FromLong(pp1);
    printf (" OK\n");

    printf ("PyInt_FromLong(p2) ... \n");
    pValue2 = PyInt_FromLong(pp2);
    printf (" OK\n");

    /* pValue reference stolen here: */
    PyTuple_SetItem(pArgs, 0, pValue1);

    /* pValue reference stolen here: */
    PyTuple_SetItem(pArgs, 1, pValue2);

    // printf("Check funciont ... ");
    // if (pFunc && PyCallable_Check(pFunc)) {
    //     printf (" OK\n");
    //
    //     if (!pValue) {
    //         Py_DECREF(pArgs);
    //         fprintf(stderr, "Cannot convert argument\n");
    //         return 1;
    //     }
    //     pValue = PyObject_CallObject(pFunc, pArgs);
    //     Py_DECREF(pArgs);
    //     if (pValue != NULL) {
    //         printf("Result of call: %ld\n", PyInt_AsLong(pValue));
    //         Py_DECREF(pValue);
    //     }
    //     else {
    //         PyErr_Print();
    //         fprintf(stderr,"Call failed\n");
    //         return 1;
    //     }
    // }
    // Py_Finalize();
    return 0;
}

#ifdef __cplusplus
}
#endif //__cplusplus
