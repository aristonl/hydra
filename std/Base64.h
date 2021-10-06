#pragma once

#include <std/ByteBuffer.h>
#include <std/String.h>
#include <std/StringView.h>

namespace std {

    size_t calculate_base64_decoded_length(const StringView&);

    size_t calculate_base64_encoded_length(ReadonlyBytes);

    ByteBuffer decode_base64(const StringView&);

    String encode_base64(ReadonlyBytes);

}

using std::decode_base64;
using std::encode_base64;
