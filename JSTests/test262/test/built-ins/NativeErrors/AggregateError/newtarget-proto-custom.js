// Copyright (C) 2019 Leo Balter. All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.

/*---
esid: sec-aggregate-error
description: >
  Use a custom NewTarget prototype
info: |
  AggregateError ( errors, message )

  1. If NewTarget is undefined, let newTarget be the active function object, else let newTarget be NewTarget.
  2. Let O be ? OrdinaryCreateFromConstructor(newTarget, "%AggregateError.prototype%", « [[ErrorData]], [[AggregateErrors]] »).
  ...
  6. Return O.

  OrdinaryCreateFromConstructor ( constructor, intrinsicDefaultProto [ , internalSlotsList ] )

  ...
  2. Let proto be ? GetPrototypeFromConstructor(constructor, intrinsicDefaultProto).
  3. Return ObjectCreate(proto, internalSlotsList).

  GetPrototypeFromConstructor ( constructor, intrinsicDefaultProto )

  ...
  3. Let proto be ? Get(constructor, "prototype").
  4. If Type(proto) is not Object, then
    a. Let realm be ? GetFunctionRealm(constructor).
    b. Set proto to realm's intrinsic object named intrinsicDefaultProto.
  Return proto.
features: [AggregateError, Reflect.construct]
---*/

var custom = { x: 42 };
var newt = new Proxy(function() {}, {
  get(t, p) {
    if (p === 'prototype') {
      return custom;
    }

    return t[p];
  }
});

var obj = Reflect.construct(AggregateError, [[]], newt);

assert.sameValue(Object.getPrototypeOf(obj), custom);
assert.sameValue(obj.x, 42);
