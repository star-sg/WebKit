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
#include "HTMLPluginElement.h"

#include <wtf/Gigacage.h>
#include <wtf/PtrTag.h>
#include <wtf/RawPtrTraits.h>


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

JSValue JSHTMLDocument::leakState(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
    VM& vm = globalObject.vm();
    CompleteSubspace& cellSpace = vm.cellSpace;
    Allocator allocator = cellSpace.allocatorForNonVirtual(64, AllocatorForMode::MustAlreadyHaveAllocator);
    LocalAllocator* localAllocator = allocator.localAllocator();
    FreeList& fl = localAllocator->getFreeList();
    BlockDirectory* bd = localAllocator->getBlockDirectory();
    
    int cnt;
    fl.forEach(
       [&] (HeapCell* cell) {
        cnt ++;
        (void)cell;
       });
    
    
    printf("[ MY DEBUG %d ] QUERY FOR 64\n", getpid());
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
    
    (void)globalObject;
    (void)callFrame;
    return JSC::JSValue();
}

JSC::JSValue JSHTMLDocument::leakState_160(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
    VM& vm = globalObject.vm();
    CompleteSubspace& cellSpace = vm.cellSpace;
    Allocator allocator = cellSpace.allocatorForNonVirtual(160, AllocatorForMode::EnsureAllocator);
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
        
        
        printf("[ MY DEBUG %d ] QUERY FOR 160\n", getpid());
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
        printf("[ MY DEBUG %d ] CANNOT QUERY FOR 160\n", getpid());
    }
    
    (void)globalObject;
    (void)callFrame;
    return JSC::JSValue();
}

JSC::JSValue JSHTMLDocument::leakAddr(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
    
    EnsureStillAliveScope argument0 = callFrame.uncheckedArgument(0);
    JSObject *object = jsCast<JSObject*>(argument0.value());
    void *converted_object = bitwise_cast<void *>(object);
    
    printf("[ MY DEBUG %d ] New fake object %p\n", getpid(), converted_object);

    (void)globalObject;
    return JSC::JSValue();
}

JSC::JSValue JSHTMLDocument::leakScriptObject(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
    
    EnsureStillAliveScope argument0 = callFrame.uncheckedArgument(0);
    JSHTMLElement *object = jsCast<JSHTMLElement*>(argument0.value());
    
    HTMLElement& element = object->wrapped();
    auto& pluginElement = downcast<HTMLPlugInElement>(element);
    JSObject* scriptObject = pluginElement.scriptObjectForPluginReplacement();
    
    
    printf("[ MY DEBUG %d ] m_scriptObject = %p\n", getpid(), scriptObject);
    VM& vm = globalObject.vm();
    Identifier ident = Identifier::fromString(vm, "a");
    PropertyName pn(ident);

    return scriptObject->get(&globalObject, pn);

//    (void)globalObject;
//    return JSC::JSValue();
}


JSObject *uaf_obj = nullptr;

JSC::JSValue JSHTMLDocument::cloneObj(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
    EnsureStillAliveScope argument0 = callFrame.uncheckedArgument(0);
    uaf_obj = jsCast<JSObject*>(argument0.value());

    (void)globalObject;
    return JSValue();
}

//JSC::JSValue JSHTMLDocument::triggerUAF(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
//
//    VM& vm = globalObject.vm();
//    Identifier ident = Identifier::fromString(vm, "1");
//    PropertyName pn(ident);
//
//    EnsureStillAliveScope argument0 = callFrame.uncheckedArgument(0);
//    JSObject *object = jsCast<JSObject*>(argument0.value());
//    void *converted_object = bitwise_cast<void *>(object);
//
//    printf("[ MY DEBUG %d ] TRIGGERING ON %p with reclaimed block %p\n", getpid(), uaf_obj, converted_object);
//
//    (void)callFrame;
//    return uaf_obj->get(&globalObject, pn);
//}

using Mytype = CagedBarrierPtr<Gigacage::Primitive, void, tagCagedPtr>;
JSC::JSValue JSHTMLDocument::triggerUAF(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {

//    EnsureStillAliveScope argument0 = callFrame.uncheckedArgument(0);
//    JSObject *object = jsCast<JSObject*>(argument0.value());
//    JSC::JSArrayBufferView *converted_object = bitwise_cast<JSC::JSArrayBufferView *>(object);
//
//    void *vector = converted_object->vector();
//    void *cagedPtr = Gigacage::caged(Gigacage::Primitive, vector);
//
//    printf("[ MY DEBUG %d ] Vector: %p\n", getpid(), vector);
//    printf("[ MY DEBUG %d ] Caged ptr: %p\n", getpid(), cagedPtr);
//
    
    printf("[ MY DEBUG %d ] ArrayType: %u\n", getpid(), JSC::JSType::ArrayType);
    
    (void)callFrame;
    (void)globalObject;
    return JSC::JSValue();
}

} // namespace WebCore
