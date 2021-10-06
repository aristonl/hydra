#pragma once

#include <std/Types.h>

namespace std {

    namespace Detail {
        template<size_t inline_capacity>
        class ByteBuffer;
    }

    class Bitmap;
    using ByteBuffer = std::Detail::ByteBuffer<32>;
    class GenericLexer;
    class IPv4Address;
    class JsonArray;
    class JsonObject;
    class JsonValue;
    class StackInfo;
    class String;
    class StringBuilder;
    class StringImpl;
    class StringView;
    class Time;
    class URL;
    class FlyString;
    class Utf16View;
    class Utf32View;
    class Utf8View;
    class InputStream;
    class InputMemoryStream;
    class DuplexMemoryStream;
    class OutputStream;
    class InputBitStream;
    class OutputBitStream;
    class OutputMemoryStream;

    template<size_t Capacity>
    class CircularDuplexStream;

    template<typename T>
    class Span;

    template<typename T, size_t Size>
    struct Array;

    template<typename Container, typename ValueType>
    class SimpleIterator;

    using ReadonlyBytes = Span<const u8>;
    using Bytes = Span<u8>;

    template<typename T, std::MemoryOrder DefaultMemoryOrder>
    class Atomic;

    template<typename T>
    class SinglyLinkedList;

    template<typename T>
    class DoublyLinkedList;

    template<typename T, size_t capacity>
    class CircularQueue;

    template<typename T>
    struct Traits;

    template<typename T, typename TraitsForT = Traits<T>, bool IsOrdered = false>
    class HashTable;

    template<typename T, typename TraitsForT = Traits<T>>
    using OrderedHashTable = HashTable<T, TraitsForT, true>;

    template<typename K, typename V, typename KeyTraits = Traits<K>, bool IsOrdered = false>
    class HashMap;

    template<typename K, typename V, typename KeyTraits = Traits<K>>
    using OrderedHashMap = HashMap<K, V, KeyTraits, true>;

    template<typename T>
    class Badge;

    template<typename T>
    class FixedArray;

    template<typename>
    class Function;

    template<typename Out, typename... In>
    class Function<Out(In...)>;

    template<typename T>
    class NonnullRefPtr;

    template<typename T>
    class NonnullOwnPtr;

    template<typename T, size_t inline_capacity = 0>
    class NonnullRefPtrVector;

    template<typename T, size_t inline_capacity = 0>
    class NonnullOwnPtrVector;

    template<typename T>
    class Optional;

    template<typename T>
    struct RefPtrTraits;

    template<typename T, typename PtrTraits = RefPtrTraits<T>>
    class RefPtr;

    template<typename T>
    class OwnPtr;

    template<typename T>
    class WeakPtr;

    template<typename T, size_t inline_capacity = 0>
    requires(!IsRvalueReference<T>) class Vector;

}

using std::Array;
using std::Atomic;
using std::Badge;
using std::Bitmap;
using std::ByteBuffer;
using std::Bytes;
using std::CircularDuplexStream;
using std::CircularQueue;
using std::DoublyLinkedList;
using std::DuplexMemoryStream;
using std::FixedArray;
using std::FlyString;
using std::Function;
using std::GenericLexer;
using std::HashMap;
using std::HashTable;
using std::InputBitStream;
using std::InputMemoryStream;
using std::InputStream;
using std::IPv4Address;
using std::JsonArray;
using std::JsonObject;
using std::JsonValue;
using std::NonnullOwnPtr;
using std::NonnullOwnPtrVector;
using std::NonnullRefPtr;
using std::NonnullRefPtrVector;
using std::Optional;
using std::OutputBitStream;
using std::OutputMemoryStream;
using std::OutputStream;
using std::OwnPtr;
using std::ReadonlyBytes;
using std::RefPtr;
using std::SinglyLinkedList;
using std::Span;
using std::StackInfo;
using std::String;
using std::StringBuilder;
using std::StringImpl;
using std::StringView;
using std::Time;
using std::Traits;
using std::URL;
using std::Utf16View;
using std::Utf32View;
using std::Utf8View;
using std::Vector;
