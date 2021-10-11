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
    
    VM& vm = globalObject.vm();
    CompleteSubspace& cellSpace = vm.cellSpace;
    Allocator allocator = cellSpace.allocatorForNonVirtual(64, AllocatorForMode::MustAlreadyHaveAllocator);
    LocalAllocator* localAllocator = allocator.localAllocator();
    FreeList& fl = localAllocator->getFreeList();
    
    (void)globalObject;
    (void)scriptObject;
    (void)fl;
    return JSC::JSValue();
}

} // namespace WebCore
