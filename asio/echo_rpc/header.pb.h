// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: header.proto

#ifndef PROTOBUF_INCLUDED_header_2eproto
#define PROTOBUF_INCLUDED_header_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_header_2eproto 

namespace protobuf_header_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_header_2eproto
namespace Header {
class RpcHeader;
class RpcHeaderDefaultTypeInternal;
extern RpcHeaderDefaultTypeInternal _RpcHeader_default_instance_;
}  // namespace Header
namespace google {
namespace protobuf {
template<> ::Header::RpcHeader* Arena::CreateMaybeMessage<::Header::RpcHeader>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace Header {

// ===================================================================

class RpcHeader : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Header.RpcHeader) */ {
 public:
  RpcHeader();
  virtual ~RpcHeader();

  RpcHeader(const RpcHeader& from);

  inline RpcHeader& operator=(const RpcHeader& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  RpcHeader(RpcHeader&& from) noexcept
    : RpcHeader() {
    *this = ::std::move(from);
  }

  inline RpcHeader& operator=(RpcHeader&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const RpcHeader& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const RpcHeader* internal_default_instance() {
    return reinterpret_cast<const RpcHeader*>(
               &_RpcHeader_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(RpcHeader* other);
  friend void swap(RpcHeader& a, RpcHeader& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline RpcHeader* New() const final {
    return CreateMaybeMessage<RpcHeader>(NULL);
  }

  RpcHeader* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<RpcHeader>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const RpcHeader& from);
  void MergeFrom(const RpcHeader& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(RpcHeader* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string service_name = 1;
  bool has_service_name() const;
  void clear_service_name();
  static const int kServiceNameFieldNumber = 1;
  const ::std::string& service_name() const;
  void set_service_name(const ::std::string& value);
  #if LANG_CXX11
  void set_service_name(::std::string&& value);
  #endif
  void set_service_name(const char* value);
  void set_service_name(const char* value, size_t size);
  ::std::string* mutable_service_name();
  ::std::string* release_service_name();
  void set_allocated_service_name(::std::string* service_name);

  // optional string method_name = 2;
  bool has_method_name() const;
  void clear_method_name();
  static const int kMethodNameFieldNumber = 2;
  const ::std::string& method_name() const;
  void set_method_name(const ::std::string& value);
  #if LANG_CXX11
  void set_method_name(::std::string&& value);
  #endif
  void set_method_name(const char* value);
  void set_method_name(const char* value, size_t size);
  ::std::string* mutable_method_name();
  ::std::string* release_method_name();
  void set_allocated_method_name(::std::string* method_name);

  // optional int32 args_size = 3;
  bool has_args_size() const;
  void clear_args_size();
  static const int kArgsSizeFieldNumber = 3;
  ::google::protobuf::int32 args_size() const;
  void set_args_size(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:Header.RpcHeader)
 private:
  void set_has_service_name();
  void clear_has_service_name();
  void set_has_method_name();
  void clear_has_method_name();
  void set_has_args_size();
  void clear_has_args_size();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr service_name_;
  ::google::protobuf::internal::ArenaStringPtr method_name_;
  ::google::protobuf::int32 args_size_;
  friend struct ::protobuf_header_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// RpcHeader

// optional string service_name = 1;
inline bool RpcHeader::has_service_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RpcHeader::set_has_service_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RpcHeader::clear_has_service_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RpcHeader::clear_service_name() {
  service_name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_service_name();
}
inline const ::std::string& RpcHeader::service_name() const {
  // @@protoc_insertion_point(field_get:Header.RpcHeader.service_name)
  return service_name_.GetNoArena();
}
inline void RpcHeader::set_service_name(const ::std::string& value) {
  set_has_service_name();
  service_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Header.RpcHeader.service_name)
}
#if LANG_CXX11
inline void RpcHeader::set_service_name(::std::string&& value) {
  set_has_service_name();
  service_name_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Header.RpcHeader.service_name)
}
#endif
inline void RpcHeader::set_service_name(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_service_name();
  service_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Header.RpcHeader.service_name)
}
inline void RpcHeader::set_service_name(const char* value, size_t size) {
  set_has_service_name();
  service_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Header.RpcHeader.service_name)
}
inline ::std::string* RpcHeader::mutable_service_name() {
  set_has_service_name();
  // @@protoc_insertion_point(field_mutable:Header.RpcHeader.service_name)
  return service_name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* RpcHeader::release_service_name() {
  // @@protoc_insertion_point(field_release:Header.RpcHeader.service_name)
  if (!has_service_name()) {
    return NULL;
  }
  clear_has_service_name();
  return service_name_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void RpcHeader::set_allocated_service_name(::std::string* service_name) {
  if (service_name != NULL) {
    set_has_service_name();
  } else {
    clear_has_service_name();
  }
  service_name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), service_name);
  // @@protoc_insertion_point(field_set_allocated:Header.RpcHeader.service_name)
}

// optional string method_name = 2;
inline bool RpcHeader::has_method_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RpcHeader::set_has_method_name() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RpcHeader::clear_has_method_name() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RpcHeader::clear_method_name() {
  method_name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_method_name();
}
inline const ::std::string& RpcHeader::method_name() const {
  // @@protoc_insertion_point(field_get:Header.RpcHeader.method_name)
  return method_name_.GetNoArena();
}
inline void RpcHeader::set_method_name(const ::std::string& value) {
  set_has_method_name();
  method_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Header.RpcHeader.method_name)
}
#if LANG_CXX11
inline void RpcHeader::set_method_name(::std::string&& value) {
  set_has_method_name();
  method_name_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Header.RpcHeader.method_name)
}
#endif
inline void RpcHeader::set_method_name(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_method_name();
  method_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Header.RpcHeader.method_name)
}
inline void RpcHeader::set_method_name(const char* value, size_t size) {
  set_has_method_name();
  method_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Header.RpcHeader.method_name)
}
inline ::std::string* RpcHeader::mutable_method_name() {
  set_has_method_name();
  // @@protoc_insertion_point(field_mutable:Header.RpcHeader.method_name)
  return method_name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* RpcHeader::release_method_name() {
  // @@protoc_insertion_point(field_release:Header.RpcHeader.method_name)
  if (!has_method_name()) {
    return NULL;
  }
  clear_has_method_name();
  return method_name_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void RpcHeader::set_allocated_method_name(::std::string* method_name) {
  if (method_name != NULL) {
    set_has_method_name();
  } else {
    clear_has_method_name();
  }
  method_name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), method_name);
  // @@protoc_insertion_point(field_set_allocated:Header.RpcHeader.method_name)
}

// optional int32 args_size = 3;
inline bool RpcHeader::has_args_size() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void RpcHeader::set_has_args_size() {
  _has_bits_[0] |= 0x00000004u;
}
inline void RpcHeader::clear_has_args_size() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void RpcHeader::clear_args_size() {
  args_size_ = 0;
  clear_has_args_size();
}
inline ::google::protobuf::int32 RpcHeader::args_size() const {
  // @@protoc_insertion_point(field_get:Header.RpcHeader.args_size)
  return args_size_;
}
inline void RpcHeader::set_args_size(::google::protobuf::int32 value) {
  set_has_args_size();
  args_size_ = value;
  // @@protoc_insertion_point(field_set:Header.RpcHeader.args_size)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace Header

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_header_2eproto