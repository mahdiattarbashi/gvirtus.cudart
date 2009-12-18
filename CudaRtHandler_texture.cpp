#include <cuda_runtime_api.h>
#include "CudaUtil.h"
#include "CudaRtHandler.h"

CUDA_ROUTINE_HANDLER(BindTexture) {
    Buffer *out = new Buffer();
    size_t *offset = out->Delegate<size_t>();
    *offset = *(input_buffer->Assign<size_t>());
    char * texrefHandler = input_buffer->AssignString();
    textureReference *guestTexref = input_buffer->Assign<textureReference>();
    textureReference *texref = pThis->GetTexture(texrefHandler);
    memmove(texref, guestTexref, sizeof(textureReference));
    char * devPtrHandler =
        input_buffer->Assign<char>(CudaUtil::MarshaledDevicePointerSize);
    //void *devPtr = pThis->GetDevicePointer(devPtrHandler);
    void *devPtr = CudaUtil::UnmarshalPointer(devPtrHandler);
    cudaChannelFormatDesc *desc = input_buffer->Assign<cudaChannelFormatDesc>();
    size_t size = input_buffer->Get<size_t>();

    cudaError_t exit_code = cudaBindTexture(offset, texref, devPtr, desc, size);

    return new Result(exit_code, out);
}

#if 0
CUDA_ROUTINE_HANDLER(BindTexture2D) {
    Buffer *out = new Buffer();
    size_t *offset = out->Delegate<size_t>();
    *offset = *(input_buffer->Assign<size_t>());
    char * texrefHandler = input_buffer->AssignString();
    textureReference *guestTexref = input_buffer->Assign<textureReference>();
    textureReference *texref = pThis->GetTexture(texrefHandler);
    memmove(texref, guestTexref, sizeof(textureReference));
    char * devPtrHandler =
        input_buffer->Assign<char>(CudaUtil::MarshaledDevicePointerSize);
    void *devPtr = pThis->GetDevicePointer(devPtrHandler);
    cudaChannelFormatDesc *desc = input_buffer->Assign<cudaChannelFormatDesc>();
    size_t width = input_buffer->Get<size_t>();
    size_t height = input_buffer->Get<size_t>();
    size_t pitch = input_buffer->Get<size_t>();

    cudaError_t exit_code = cudaBindTexture2D(offset, texref, devPtr, desc, width,
            height, pitch);

    return new Result(exit_code, out);
}
#endif

CUDA_ROUTINE_HANDLER(BindTextureToArray) {
    char * texrefHandler = input_buffer->AssignString();
    textureReference *guestTexref = input_buffer->Assign<textureReference>();
    char * arrayHandler =
        input_buffer->Assign<char>(CudaUtil::MarshaledDevicePointerSize);
    cudaChannelFormatDesc *desc = input_buffer->Assign<cudaChannelFormatDesc>();

    textureReference *texref = pThis->GetTexture(texrefHandler);
    memmove(texref, guestTexref, sizeof(textureReference));

    //cudaArray *array = (cudaArray *) pThis->GetDevicePointer(arrayHandler);
    cudaArray *array = (cudaArray *) CudaUtil::UnmarshalPointer(arrayHandler);

    cudaError_t exit_code = cudaBindTextureToArray(texref, array, desc);

    return new Result(exit_code);
}

CUDA_ROUTINE_HANDLER(GetChannelDesc) {
    cudaChannelFormatDesc *guestDesc =
            input_buffer->Assign<cudaChannelFormatDesc>();
    char * arrayHandler =
        input_buffer->Assign<char>(CudaUtil::MarshaledDevicePointerSize);
    //cudaArray *array = (cudaArray *) pThis->GetDevicePointer(arrayHandler);
    cudaArray *array = (cudaArray *) CudaUtil::UnmarshalPointer(arrayHandler);
    Buffer *out = new Buffer();
    cudaChannelFormatDesc *desc = out->Delegate<cudaChannelFormatDesc>();
    memmove(desc, guestDesc, sizeof(cudaChannelFormatDesc));

    cudaError_t exit_code = cudaGetChannelDesc(desc, array);

    return new Result(exit_code, out);
}

CUDA_ROUTINE_HANDLER(GetTextureAlignmentOffset) {
    Buffer *out = new Buffer();
    size_t *offset = out->Delegate<size_t>();
    *offset = *(input_buffer->Assign<size_t>());
    char * texrefHandler = input_buffer->AssignString();
    textureReference *guestTexref = input_buffer->Assign<textureReference>();
    textureReference *texref = pThis->GetTexture(texrefHandler);
    memmove(texref, guestTexref, sizeof(textureReference));

    cudaError_t exit_code = cudaGetTextureAlignmentOffset(offset, texref);

    return new Result(exit_code, out);
}

CUDA_ROUTINE_HANDLER(GetTextureReference) {
    textureReference *texref;
    char *symbol_handler = input_buffer->AssignString();
    char *symbol = input_buffer->AssignString();

    char *our_symbol = const_cast<char *> (pThis->GetVar(symbol_handler));
    if (our_symbol != NULL)
        symbol = const_cast<char *> (our_symbol);

    cudaError_t exit_code = cudaGetTextureReference(
            (const textureReference ** ) &texref, symbol);

    Buffer *out = new Buffer();
    if(exit_code == cudaSuccess)
        out->AddString(pThis->GetTextureHandler(texref));
    else
        out->AddString("0x0");

    return new Result(exit_code, out);
}


CUDA_ROUTINE_HANDLER(UnbindTexture) {
    char * texrefHandler = input_buffer->AssignString();
    textureReference *texref = pThis->GetTexture(texrefHandler);

    cudaError_t exit_code = cudaUnbindTexture(texref);

    return new Result(exit_code);
}
