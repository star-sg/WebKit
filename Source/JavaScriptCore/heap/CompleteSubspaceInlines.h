/*
 * Copyright (C) 2018-2020 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#pragma once

#include "CompleteSubspace.h"
#include "VM.h"

namespace JSC {

ALWAYS_INLINE void* CompleteSubspace::allocateNonVirtual(VM& vm, size_t size, GCDeferralContext* deferralContext, AllocationFailureMode failureMode)
{
    if constexpr (validateDFGDoesGC)
        vm.heap.verifyCanGC();

    if (Allocator allocator = allocatorForNonVirtual(size, AllocatorForMode::AllocatorIfExists)) {
        auto result = allocator.allocate(vm.heap, deferralContext, failureMode);
        if (!strcmp(this->name(), "JSCell")) {
            LocalAllocator* localAllocator = allocator.localAllocator();
            FreeList& fl = localAllocator->getFreeList();

            int cnt = 0;
            fl.forEach(
               [&] (HeapCell* cell) {
                cnt++;
                (void)cell;
               });

            printf("[ MY DEBUG %d ] ALLOCATE NON VIRTUAL %lu\n", getpid(), size);
//            printf("[ MY DEBUG %d ] Current block on allocator: %p\n", getpid(), localAllocator->getCurrentBlock());
//            printf("[ MY DEBUG %d ] Last active block on allocator: %p\n", getpid(), localAllocator->getLastActiveBlock());
            printf("[ MY DEBUG %d ] New pointer in JSCell ( %p ) was allocated at %p by allocator %p\n", getpid(), this, result, localAllocator);
//            printf("[ MY DEBUG %d ] FreeList of Allocator (%p) in cellSpace\n", getpid(), localAllocator);
//            printf("[ MY DEBUG %d ] \tNumber of free cells = %d\n", getpid(), cnt);
//            printf("[ MY DEBUG %d ] \tpayloadEnd = %p\n", getpid(), fl.m_payloadEnd);
//            printf("[ MY DEBUG %d ] \tremaining = %u\n", getpid(), fl.m_remaining);
//            printf("[ MY DEBUG %d ] \toriginalSize = %u\n", getpid(), fl.m_originalSize);
            printf("[ MY DEBUG %d ] ===============================================================\n", getpid());
        }
        return result;
    }
    return allocateSlow(vm, size, deferralContext, failureMode);
}

} // namespace JSC

