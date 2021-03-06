/**
 * Appcelerator Titanium License
 * This source code and all modifications done by Appcelerator
 * are licensed under the Apache Public License (version 2) and
 * are Copyright (c) 2009 by Appcelerator, Inc.
 */

/*
 *  Copyright (C) 2005, 2008 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"
#include "PropertySlot.h"

#include "TiFunction.h"
#include "TiGlobalObject.h"

namespace TI {

TiValue PropertySlot::functionGetter(TiExcState* exec) const
{
    // Prevent getter functions from observing execution if an exception is pending.
    if (exec->hadException())
        return exec->exception();

    CallData callData;
    CallType callType = m_data.getterFunc->getCallData(callData);
    if (callType == CallTypeHost)
        return callData.native.function(exec, m_data.getterFunc, thisValue(), exec->emptyList());
    ASSERT(callType == CallTypeJS);
    // FIXME: Can this be done more efficiently using the callData?
    return asFunction(m_data.getterFunc)->call(exec, thisValue(), exec->emptyList());
}

} // namespace TI
