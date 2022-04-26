/*
 * Copyright (C) 2007-2017 Apple Inc. All rights reserved.
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

#include "config.h"
#include "JSHTMLDocument.h"
#include "JSWebGLBuffer.h"
#include "JSWebGLRenderingContext.h"
#include "WebGLRenderingContextBase.h"

namespace WebCore {
using namespace JSC;

JSValue toJSNewlyCreated(JSGlobalObject* lexicalGlobalObject, JSDOMGlobalObject* globalObject, Ref<HTMLDocument>&& passedDocument)
{
    auto& document = passedDocument.get();
    auto* wrapper = createWrapper<HTMLDocument>(globalObject, WTFMove(passedDocument));
    reportMemoryForDocumentIfFrameless(*lexicalGlobalObject, document);
    return wrapper;
}

JSValue toJS(JSGlobalObject* lexicalGlobalObject, JSDOMGlobalObject* globalObject, HTMLDocument& document)
{
    if (auto* wrapper = cachedDocumentWrapper(*lexicalGlobalObject, *globalObject, document))
        return wrapper;
    return toJSNewlyCreated(lexicalGlobalObject, globalObject, Ref<HTMLDocument>(document));
}

JSC::JSValue JSHTMLDocument::dumpWebGLBuffer(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
    EnsureStillAliveScope argument0 = callFrame.uncheckedArgument(0);
    JSWebGLBuffer *object = jsCast<JSWebGLBuffer*>(argument0.value());
    void *converted_object = bitwise_cast<void *>(object);
    
    printf("[ MY DEBUG ] Object was located in %p\n", converted_object);
    
    void *converted_raw = bitwise_cast<void *>(JSWebGLBuffer::toWrapped(globalObject.vm(), object));
    printf("[ MY DEBUG ] -> Raw = %p\n", converted_raw);

//    (void)globalObject;
    return JSC::JSValue();
}

JSC::JSValue JSHTMLDocument::dumpArray(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
    EnsureStillAliveScope argument0 = callFrame.uncheckedArgument(0);
    void *converted_object = bitwise_cast<void *>(argument0.value());
    uint64_t addr = (uint64_t)converted_object;

    (void)globalObject;
    return JSValue(addr);
}

JSC::JSValue JSHTMLDocument::dumpHeap(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
    unsigned int size = convert<IDLUnsignedLong>(globalObject, callFrame.uncheckedArgument(0));
    VM& vm = globalObject.vm();
    CompleteSubspace& space = vm.primitiveGigacageAuxiliarySpace();
    Allocator allocator = space.allocatorFor(size, AllocatorForMode::EnsureAllocator);
    LocalAllocator* localAllocator = allocator.localAllocator();
    if (localAllocator != nullptr) {
        FreeList& fl = localAllocator->getFreeList();
        BlockDirectory* bd = localAllocator->getBlockDirectory();

        int cnt;
        fl.forEach(
           [&] (HeapCell* cell) {
            cnt ++;
            (void)cell;
           });


        printf("[ MY DEBUG ] QUERY FOR %u\n", size);
        printf("[ MY DEBUG ] FreeList of Allocator (%p) in cellSpace\n", localAllocator);
        printf("[ MY DEBUG ] Number of free cells = %d\n", cnt);
        printf("[ MY DEBUG ] \tpayloadEnd = %p\n", fl.m_payloadEnd);
        printf("[ MY DEBUG ] \tremaining = %u\n", fl.m_remaining);
        printf("[ MY DEBUG ] \toriginalSize = %u\n", fl.m_originalSize);
        bd->dumpBlocks();

        /*
        printf("[ MY DEBUG ] Freed cells:\n");
        fl.forEach(
           [&] (HeapCell* cell) {
                void *converted_cell = bitwise_cast<void *>(cell);
                printf("[ MY DEBUG ] \tCell: %p\n", converted_cell);
           });
        */
    } else {
        printf("[ MY DEBUG ] CANNOT QUERY FOR %u\n", size);
    }
    (void)size;
    (void)space;
    return JSC::JSValue();
}

} // namespace WebCore
