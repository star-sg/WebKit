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

#include <stdio.h>

namespace WebCore {

JSC::JSValue JSAnalyserNode::evil_func(JSC::JSGlobalObject& globalObject, JSC::CallFrame& callFrame) {
    EnsureStillAliveScope argument0 = callFrame.uncheckedArgument(0);
    auto argument = convert<IDLInterface<AudioNode>>(globalObject, argument0.value());
    (void)argument;
    return JSC::JSValue();
}

} // namespace WebCore
