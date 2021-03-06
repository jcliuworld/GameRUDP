// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: DemoServer.proto

#ifndef PROTOBUF_INCLUDED_DemoServer_2eproto
#define PROTOBUF_INCLUDED_DemoServer_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
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
#define PROTOBUF_INTERNAL_EXPORT_protobuf_DemoServer_2eproto 

namespace protobuf_DemoServer_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[3];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_DemoServer_2eproto
namespace Demo {
class msgRetcodeRsp;
class msgRetcodeRspDefaultTypeInternal;
extern msgRetcodeRspDefaultTypeInternal _msgRetcodeRsp_default_instance_;
class msgUserLoginReq;
class msgUserLoginReqDefaultTypeInternal;
extern msgUserLoginReqDefaultTypeInternal _msgUserLoginReq_default_instance_;
class msgUserLoginRsp;
class msgUserLoginRspDefaultTypeInternal;
extern msgUserLoginRspDefaultTypeInternal _msgUserLoginRsp_default_instance_;
}  // namespace Demo
namespace google {
namespace protobuf {
template<> ::Demo::msgRetcodeRsp* Arena::CreateMaybeMessage<::Demo::msgRetcodeRsp>(Arena*);
template<> ::Demo::msgUserLoginReq* Arena::CreateMaybeMessage<::Demo::msgUserLoginReq>(Arena*);
template<> ::Demo::msgUserLoginRsp* Arena::CreateMaybeMessage<::Demo::msgUserLoginRsp>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace Demo {

// ===================================================================

class msgUserLoginReq : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Demo.msgUserLoginReq) */ {
 public:
  msgUserLoginReq();
  virtual ~msgUserLoginReq();

  msgUserLoginReq(const msgUserLoginReq& from);

  inline msgUserLoginReq& operator=(const msgUserLoginReq& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  msgUserLoginReq(msgUserLoginReq&& from) noexcept
    : msgUserLoginReq() {
    *this = ::std::move(from);
  }

  inline msgUserLoginReq& operator=(msgUserLoginReq&& from) noexcept {
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
  static const msgUserLoginReq& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const msgUserLoginReq* internal_default_instance() {
    return reinterpret_cast<const msgUserLoginReq*>(
               &_msgUserLoginReq_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(msgUserLoginReq* other);
  friend void swap(msgUserLoginReq& a, msgUserLoginReq& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline msgUserLoginReq* New() const final {
    return CreateMaybeMessage<msgUserLoginReq>(NULL);
  }

  msgUserLoginReq* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<msgUserLoginReq>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const msgUserLoginReq& from);
  void MergeFrom(const msgUserLoginReq& from);
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
  void InternalSwap(msgUserLoginReq* other);
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

  // @@protoc_insertion_point(class_scope:Demo.msgUserLoginReq)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_DemoServer_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class msgUserLoginRsp : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Demo.msgUserLoginRsp) */ {
 public:
  msgUserLoginRsp();
  virtual ~msgUserLoginRsp();

  msgUserLoginRsp(const msgUserLoginRsp& from);

  inline msgUserLoginRsp& operator=(const msgUserLoginRsp& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  msgUserLoginRsp(msgUserLoginRsp&& from) noexcept
    : msgUserLoginRsp() {
    *this = ::std::move(from);
  }

  inline msgUserLoginRsp& operator=(msgUserLoginRsp&& from) noexcept {
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
  static const msgUserLoginRsp& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const msgUserLoginRsp* internal_default_instance() {
    return reinterpret_cast<const msgUserLoginRsp*>(
               &_msgUserLoginRsp_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(msgUserLoginRsp* other);
  friend void swap(msgUserLoginRsp& a, msgUserLoginRsp& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline msgUserLoginRsp* New() const final {
    return CreateMaybeMessage<msgUserLoginRsp>(NULL);
  }

  msgUserLoginRsp* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<msgUserLoginRsp>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const msgUserLoginRsp& from);
  void MergeFrom(const msgUserLoginRsp& from);
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
  void InternalSwap(msgUserLoginRsp* other);
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

  // @@protoc_insertion_point(class_scope:Demo.msgUserLoginRsp)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_DemoServer_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class msgRetcodeRsp : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Demo.msgRetcodeRsp) */ {
 public:
  msgRetcodeRsp();
  virtual ~msgRetcodeRsp();

  msgRetcodeRsp(const msgRetcodeRsp& from);

  inline msgRetcodeRsp& operator=(const msgRetcodeRsp& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  msgRetcodeRsp(msgRetcodeRsp&& from) noexcept
    : msgRetcodeRsp() {
    *this = ::std::move(from);
  }

  inline msgRetcodeRsp& operator=(msgRetcodeRsp&& from) noexcept {
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
  static const msgRetcodeRsp& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const msgRetcodeRsp* internal_default_instance() {
    return reinterpret_cast<const msgRetcodeRsp*>(
               &_msgRetcodeRsp_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  void Swap(msgRetcodeRsp* other);
  friend void swap(msgRetcodeRsp& a, msgRetcodeRsp& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline msgRetcodeRsp* New() const final {
    return CreateMaybeMessage<msgRetcodeRsp>(NULL);
  }

  msgRetcodeRsp* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<msgRetcodeRsp>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const msgRetcodeRsp& from);
  void MergeFrom(const msgRetcodeRsp& from);
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
  void InternalSwap(msgRetcodeRsp* other);
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

  // required uint32 dwRet = 1;
  bool has_dwret() const;
  void clear_dwret();
  static const int kDwRetFieldNumber = 1;
  ::google::protobuf::uint32 dwret() const;
  void set_dwret(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:Demo.msgRetcodeRsp)
 private:
  void set_has_dwret();
  void clear_has_dwret();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::uint32 dwret_;
  friend struct ::protobuf_DemoServer_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// msgUserLoginReq

// -------------------------------------------------------------------

// msgUserLoginRsp

// -------------------------------------------------------------------

// msgRetcodeRsp

// required uint32 dwRet = 1;
inline bool msgRetcodeRsp::has_dwret() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void msgRetcodeRsp::set_has_dwret() {
  _has_bits_[0] |= 0x00000001u;
}
inline void msgRetcodeRsp::clear_has_dwret() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void msgRetcodeRsp::clear_dwret() {
  dwret_ = 0u;
  clear_has_dwret();
}
inline ::google::protobuf::uint32 msgRetcodeRsp::dwret() const {
  // @@protoc_insertion_point(field_get:Demo.msgRetcodeRsp.dwRet)
  return dwret_;
}
inline void msgRetcodeRsp::set_dwret(::google::protobuf::uint32 value) {
  set_has_dwret();
  dwret_ = value;
  // @@protoc_insertion_point(field_set:Demo.msgRetcodeRsp.dwRet)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace Demo

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_DemoServer_2eproto
