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
    
    
    printf("[ MY DEBUG %d ] QUERY\n", getpid());
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
    (void)fl;
    return JSC::JSValue();
}

JSC::JSValue JSAnalyserNode::angel_func(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
    
    EnsureStillAliveScope argument1 = callFrame.uncheckedArgument(0);
    JSObject* object = jsCast<JSObject*>(argument1.value());
    void *converted_object = bitwise_cast<void *>(object);
    
    printf("[ MY DEBUG %d ] New fake object %p\n", getpid(), converted_object);
	    
    (void)globalObject;
    return JSC::JSValue();
}

JSC::JSValue JSAnalyserNode::ageing_func(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
    EnsureStillAliveScope argument1 = callFrame.uncheckedArgument(0);
    JSObject* object = jsCast<JSObject*>(argument1.value());
    void *converted_object = bitwise_cast<void *>(object);
    
    VM& vm = globalObject.vm();
    CompleteSubspace& cellSpace = vm.cellSpace;
    Allocator allocator = cellSpace.allocatorForNonVirtual(64, AllocatorForMode::MustAlreadyHaveAllocator);
    LocalAllocator* localAllocator = allocator.localAllocator();
    BlockDirectory* bd = localAllocator->getBlockDirectory();
    bd->queryAge(converted_object);
    
    (void)globalObject;
    return JSC::JSValue();
}

JSC::JSValue JSAnalyserNode::fullGC(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
    printf("[ MY DEBUG %d ] DUMP ALL OPTION\n", getpid());
    JSC::Options::dumpAllOptions(stderr, JSC::Options::DumpLevel::Overridden, "Modified JSC runtime options:");
    GCController::singleton().garbageCollectNow();
    
    (void)globalObject;
    (void)callFrame;
    return JSC::JSValue();
}

} // namespace WebCore
