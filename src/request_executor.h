// Copyright 2021-2022, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <memory>

#include "infer_request.h"
#include "infer_response.h"

namespace triton { namespace backend { namespace python {

TRITONSERVER_Error* CreateTritonErrorFromException(
    const PythonBackendException& pb_exception);

struct UserpAndDeviceID {
  // This data structure is used to pass the userp and device ID to the
  // TRITONSERVER_InferenceRequestSetResponseCallback function.
  // We need the buffer device id to allocate memory for the response
  // on the correct device.
  void* userp;
  int32_t buffer_device_id;
};

class RequestExecutor {
  TRITONSERVER_ResponseAllocator* response_allocator_ = nullptr;
  TRITONSERVER_Server* server_;
  std::unique_ptr<SharedMemoryManager>& shm_pool_;

 public:
  std::unique_ptr<InferResponse> Infer(
      const std::shared_ptr<InferRequest>& infer_request,
      TRITONSERVER_InferenceResponse** response, const int32_t device_id);

  RequestExecutor(
      std::unique_ptr<SharedMemoryManager>& shm_pool,
      TRITONSERVER_Server* server);

  ~RequestExecutor();
};
}}}  // namespace triton::backend::python
