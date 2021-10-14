//
//  JSAnalyserNodeCustom.cpp
//  WebCore
//
//  Created by Minh Tuan Do on 09/10/2021.
//

#pragma once

#include "JSAnalyserNode.h"
#include <JavaScriptCore/JSCInlines.h>
#include <JavaScriptCore/JSCast.h>
#include "JSHTMLElement.h"
#include "HTMLPlugInElement.h"
#include <JavaScriptCore/VM.h>
#include <JavaScriptCore/CompleteSubspace.h>
#include <JavaScriptCore/AllocatorForMode.h>
#include <JavaScriptCore/Allocator.h>
#include <JavaScriptCore/LocalAllocator.h>
#include <JavaScriptCore/FreeList.h>

#include <stdio.h>

namespace WebCore {

JSC::JSValue JSAnalyserNode::evil_func(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {

    EnsureStillAliveScope argument0 = callFrame.uncheckedArgument(0);
    JSHTMLElement* jsHTMLElement = jsCast<JSHTMLElement*>(argument0.value());
    HTMLElement& element = jsHTMLElement->wrapped();	
    auto& pluginElement = downcast<HTMLPlugInElement>(element);
    auto* scriptObject = pluginElement.scriptObjectForPluginReplacement();
    void *converted_scriptObject = bitwise_cast<void *>(scriptObject);
    
    VM& vm = globalObject.vm();
    CompleteSubspace& cellSpace = vm.cellSpace;
    Allocator allocator = cellSpace.allocatorForNonVirtual(64, AllocatorForMode::MustAlreadyHaveAllocator);
    LocalAllocator* localAllocator = allocator.localAllocator();
    FreeList& fl = localAllocator->getFreeList();
    
    printf("[ MY DEBUG %d ] QUERY\n", getpid());
    printf("[ MY DEBUG %d ] Current block on allocator: %p\n", getpid(), localAllocator->getCurrentBlock());
    printf("[ MY DEBUG %d ] Last active block on allocator: %p\n", getpid(), localAllocator->getLastActiveBlock());
    printf("[ MY DEBUG %d ] FreeList of Allocator (%p) in cellSpace\n", getpid(), localAllocator);
    printf("[ MY DEBUG %d ] \tpayloadEnd = %p\n", getpid(), fl.m_payloadEnd);
    printf("[ MY DEBUG %d ] \tremaining = %u\n", getpid(), fl.m_remaining);
    printf("[ MY DEBUG %d ] \toriginalSize = %u\n", getpid(), fl.m_originalSize);
    printf("[ MY DEBUG %d ] Freed cells:\n", getpid());
    
    fl.forEach(
       [&] (HeapCell* cell) {
            void *converted_cell = bitwise_cast<void *>(cell);
            printf("[ MY DEBUG %d ] \tCell: %p\n", getpid(), converted_cell);
            if (converted_cell == converted_scriptObject)
                printf("[ MY DEBUG %d ] THE ONE IS ALREADY INSIDE FREELIST\n", getpid());
       });
    
    
    (void)globalObject;
    (void)scriptObject;
    (void)fl;
    return JSC::JSValue();
}

JSC::JSValue JSAnalyserNode::angel_func(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {

    EnsureStillAliveScope argument0 = callFrame.uncheckedArgument(0);
    JSHTMLElement* jsHTMLElement = jsCast<JSHTMLElement*>(argument0.value());
    HTMLElement& element = jsHTMLElement->wrapped();
    auto& pluginElement = downcast<HTMLPlugInElement>(element);
    auto* scriptObject = pluginElement.scriptObjectForPluginReplacement();
    void *converted_scriptObject = bitwise_cast<void *>(scriptObject);
    
    EnsureStillAliveScope argument1 = callFrame.uncheckedArgument(1);
    JSObject* object = jsCast<JSObject*>(argument1.value());
    void *converted_object = bitwise_cast<void *>(object);
    
    VM& vm = globalObject.vm();
    auto* methodTable = object->methodTable(vm);
//    auto* structure = object->structure(vm);
//    JSObject* proto = structure->storedPrototypeObject();
    
//    printf("[ MY DEBUG %d ] New fake object (%p) has structure at %p, and its prototype at %p\n", getpid(), converted_object, structure, proto);
    printf("[ MY DEBUG %d ] New fake object %p, has methodTable at %p\n", getpid(), converted_object, methodTable);
    
    if (converted_scriptObject == converted_object) {
        printf("[ MY DEBUG %d ] RECLAIMED SUCCESSFULLY: %p\n", getpid(), converted_object);
    }
    
    (void)globalObject;
    return JSC::JSValue();
}


} // namespace WebCore
