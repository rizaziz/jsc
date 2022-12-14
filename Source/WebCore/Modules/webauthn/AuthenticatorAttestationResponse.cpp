/*
 * Copyright (C) 2019-2021 Apple Inc. All rights reserved.
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
#include "AuthenticatorAttestationResponse.h"

#if ENABLE(WEB_AUTHN)

#include "AuthenticatorResponseData.h"
#include "CBORReader.h"
#include "WebAuthenticationUtils.h"

namespace WebCore {

Ref<AuthenticatorAttestationResponse> AuthenticatorAttestationResponse::create(Ref<ArrayBuffer>&& rawId, Ref<ArrayBuffer>&& attestationObject, AuthenticatorAttachment attachment, Vector<AuthenticatorTransport>&& transports)
{
    return adoptRef(*new AuthenticatorAttestationResponse(WTFMove(rawId), WTFMove(attestationObject), attachment, WTFMove(transports)));
}

Ref<AuthenticatorAttestationResponse> AuthenticatorAttestationResponse::create(const Vector<uint8_t>& rawId, const Vector<uint8_t>& attestationObject, AuthenticatorAttachment attachment, Vector<AuthenticatorTransport>&& transports)
{
    return create(ArrayBuffer::create(rawId.data(), rawId.size()), ArrayBuffer::create(attestationObject.data(), attestationObject.size()), attachment, WTFMove(transports));
}

AuthenticatorAttestationResponse::AuthenticatorAttestationResponse(Ref<ArrayBuffer>&& rawId, Ref<ArrayBuffer>&& attestationObject, AuthenticatorAttachment attachment, Vector<AuthenticatorTransport>&& transports)
    : AuthenticatorResponse(WTFMove(rawId), attachment)
    , m_attestationObject(WTFMove(attestationObject))
    , m_transports(WTFMove(transports))
{
}

AuthenticatorResponseData AuthenticatorAttestationResponse::data() const
{
    auto data = AuthenticatorResponse::data();
    data.isAuthenticatorAttestationResponse = true;
    data.attestationObject = m_attestationObject.copyRef();
    data.transports = m_transports;
    return data;
}

RefPtr<ArrayBuffer> AuthenticatorAttestationResponse::getAuthenticatorData() const
{
    auto decodedResponse = cbor::CBORReader::read(convertArrayBufferToVector(m_attestationObject.ptr()));
    if (!decodedResponse || !decodedResponse->isMap()) {
        ASSERT_NOT_REACHED();
        return nullptr;
    }
    const auto& attObjMap = decodedResponse->getMap();
    auto it = attObjMap.find(cbor::CBORValue("authData"));
    if (it == attObjMap.end() || !it->second.isByteString()) {
        ASSERT_NOT_REACHED();
        return nullptr;
    }
    auto authData = it->second.getByteString();
    return ArrayBuffer::tryCreate(authData.data(), authData.size());
}

} // namespace WebCore

#endif // ENABLE(WEB_AUTHN)
