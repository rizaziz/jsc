/*
 * Copyright (C) 2019 Apple Inc. All rights reserved.
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

#pragma once

#if ENABLE(WEB_AUTHN)

#include <wtf/text/WTFString.h>

namespace WebCore {

struct AuthenticationExtensionsClientInputs {
    String appid;
    bool googleLegacyAppidSupport;
    bool credProps;

    template<class Encoder> void encode(Encoder&) const;
    template<class Decoder> static std::optional<AuthenticationExtensionsClientInputs> decode(Decoder&);

    WEBCORE_EXPORT Vector<uint8_t> toCBOR() const;
    WEBCORE_EXPORT static std::optional<AuthenticationExtensionsClientInputs> fromCBOR(const Vector<uint8_t>&);
};

template<class Encoder>
void AuthenticationExtensionsClientInputs::encode(Encoder& encoder) const
{
    encoder << appid << googleLegacyAppidSupport;
}

template<class Decoder>
std::optional<AuthenticationExtensionsClientInputs> AuthenticationExtensionsClientInputs::decode(Decoder& decoder)
{
    AuthenticationExtensionsClientInputs result;

    std::optional<String> appid;
    decoder >> appid;
    if (!appid)
        return std::nullopt;
    result.appid = WTFMove(*appid);

    std::optional<bool> googleLegacyAppidSupport;
    decoder >> googleLegacyAppidSupport;
    if (!googleLegacyAppidSupport)
        return std::nullopt;
    result.googleLegacyAppidSupport = WTFMove(*googleLegacyAppidSupport);

    return result;
}

} // namespace WebCore

#endif // ENABLE(WEB_AUTHN)
