/**
 * Appcelerator Titanium License
 * This source code and all modifications done by Appcelerator
 * are licensed under the Apache Public License (version 2) and
 * are Copyright (c) 2009 by Appcelerator, Inc.
 */

/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "JSWeakObjectMapRefPrivate.h"

#include "APICast.h"
#include "APIShims.h"
#include "TiCallbackObject.h"
#include "TiValue.h"
#include "JSWeakObjectMapRefInternal.h"
#include <wtf/HashMap.h>
#include <wtf/RefCounted.h>
#include <wtf/text/StringHash.h>

using namespace WTI;
using namespace TI;

#ifdef __cplusplus
extern "C" {
#endif

JSWeakObjectMapRef JSWeakObjectMapCreate(TiContextRef context, void* privateData, JSWeakMapDestroyedCallback callback)
{
    TiExcState* exec = toJS(context);
    APIEntryShim entryShim(exec);
    RefPtr<OpaqueJSWeakObjectMap> map = OpaqueJSWeakObjectMap::create(privateData, callback);
    exec->lexicalGlobalObject()->registerWeakMap(map.get());
    return map.get();
}

void JSWeakObjectMapSet(TiContextRef ctx, JSWeakObjectMapRef map, void* key, TiObjectRef object)
{
    TiExcState* exec = toJS(ctx);
    APIEntryShim entryShim(exec);
    TiObject* obj = toJS(object);
    if (!obj)
        return;
    ASSERT(obj->inherits(&TiCallbackObject<TiGlobalObject>::info) || obj->inherits(&TiCallbackObject<TiObject>::info));
    map->map().set(key, obj);
}

TiObjectRef JSWeakObjectMapGet(TiContextRef ctx, JSWeakObjectMapRef map, void* key)
{
    TiExcState* exec = toJS(ctx);
    APIEntryShim entryShim(exec);
    return toRef(static_cast<TiObject*>(map->map().get(key)));
}

bool JSWeakObjectMapClear(TiContextRef ctx, JSWeakObjectMapRef map, void* key, TiObjectRef object)
{
    TiExcState* exec = toJS(ctx);
    APIEntryShim entryShim(exec);
    TiObject* obj = toJS(object);
    if (map->map().uncheckedRemove(key, obj))
        return true;
    return false;
}

#ifdef __cplusplus
}
#endif
