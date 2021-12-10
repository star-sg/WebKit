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

JSC::JSValue JSHTMLDocument::leakAddr(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
    
    EnsureStillAliveScope argument0 = callFrame.uncheckedArgument(0);
    JSObject *object = jsCast<JSObject*>(argument0.value());
    void *converted_object = bitwise_cast<void *>(object);
    
    printf("[ MY DEBUG %d ] Object was located in %p\n", getpid(), converted_object);

    (void)globalObject;
    return JSC::JSValue();
}

JSC::JSValue JSHTMLDocument::leakAllocator(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
    unsigned int size = convert<IDLUnsignedLong>(globalObject, callFrame.uncheckedArgument(0));
    VM& vm = globalObject.vm();
    CompleteSubspace& cellSpace = vm.cellSpace;
    Allocator allocator = cellSpace.allocatorForNonVirtual(size, AllocatorForMode::EnsureAllocator);
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
        
        
        printf("[ MY DEBUG %d ] QUERY FOR %u\n", getpid(), size);
        printf("[ MY DEBUG %d ] Current block on allocator: %p\n", getpid(), localAllocator->getCurrentBlock());
        printf("[ MY DEBUG %d ] Last active block on allocator: %p\n", getpid(), localAllocator->getLastActiveBlock());
        printf("[ MY DEBUG %d ] FreeList of Allocator (%p) in cellSpace\n", getpid(), localAllocator);
        printf("[ MY DEBUG %d ] Number of free cells = %d\n", getpid(), cnt);
        printf("[ MY DEBUG %d ] \tpayloadEnd = %p\n", getpid(), fl.m_payloadEnd);
        printf("[ MY DEBUG %d ] \tremaining = %u\n", getpid(), fl.m_remaining);
        printf("[ MY DEBUG %d ] \toriginalSize = %u\n", getpid(), fl.m_originalSize);
        bd->dumpBlocks();

        printf("[ MY DEBUG %d ] Freed cells:\n", getpid());
        fl.forEach(
           [&] (HeapCell* cell) {
                void *converted_cell = bitwise_cast<void *>(cell);
                printf("[ MY DEBUG %d ] \tCell: %p\n", getpid(), converted_cell);
           });
    } else {
        printf("[ MY DEBUG %d ] CANNOT QUERY FOR %u\n", getpid(), size);
    }
    
    return JSC::JSValue();  
}


} // namespace WebCore
