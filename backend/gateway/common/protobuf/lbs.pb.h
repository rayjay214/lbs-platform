// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: lbs.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_lbs_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_lbs_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3011000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3011000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_lbs_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_lbs_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[3]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_lbs_2eproto;
namespace lbs {
class LbsInfo;
class LbsInfoDefaultTypeInternal;
extern LbsInfoDefaultTypeInternal _LbsInfo_default_instance_;
class LbsList;
class LbsListDefaultTypeInternal;
extern LbsListDefaultTypeInternal _LbsList_default_instance_;
class LbsRec;
class LbsRecDefaultTypeInternal;
extern LbsRecDefaultTypeInternal _LbsRec_default_instance_;
}  // namespace lbs
PROTOBUF_NAMESPACE_OPEN
template<> ::lbs::LbsInfo* Arena::CreateMaybeMessage<::lbs::LbsInfo>(Arena*);
template<> ::lbs::LbsList* Arena::CreateMaybeMessage<::lbs::LbsList>(Arena*);
template<> ::lbs::LbsRec* Arena::CreateMaybeMessage<::lbs::LbsRec>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace lbs {

// ===================================================================

class LbsInfo :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:lbs.LbsInfo) */ {
 public:
  LbsInfo();
  virtual ~LbsInfo();

  LbsInfo(const LbsInfo& from);
  LbsInfo(LbsInfo&& from) noexcept
    : LbsInfo() {
    *this = ::std::move(from);
  }

  inline LbsInfo& operator=(const LbsInfo& from) {
    CopyFrom(from);
    return *this;
  }
  inline LbsInfo& operator=(LbsInfo&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const LbsInfo& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const LbsInfo* internal_default_instance() {
    return reinterpret_cast<const LbsInfo*>(
               &_LbsInfo_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(LbsInfo& a, LbsInfo& b) {
    a.Swap(&b);
  }
  inline void Swap(LbsInfo* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline LbsInfo* New() const final {
    return CreateMaybeMessage<LbsInfo>(nullptr);
  }

  LbsInfo* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<LbsInfo>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const LbsInfo& from);
  void MergeFrom(const LbsInfo& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(LbsInfo* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "lbs.LbsInfo";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_lbs_2eproto);
    return ::descriptor_table_lbs_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kIdFieldNumber = 1,
    kStrengthFieldNumber = 2,
  };
  // string id = 1;
  void clear_id();
  const std::string& id() const;
  void set_id(const std::string& value);
  void set_id(std::string&& value);
  void set_id(const char* value);
  void set_id(const char* value, size_t size);
  std::string* mutable_id();
  std::string* release_id();
  void set_allocated_id(std::string* id);
  private:
  const std::string& _internal_id() const;
  void _internal_set_id(const std::string& value);
  std::string* _internal_mutable_id();
  public:

  // int32 strength = 2;
  void clear_strength();
  ::PROTOBUF_NAMESPACE_ID::int32 strength() const;
  void set_strength(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_strength() const;
  void _internal_set_strength(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:lbs.LbsInfo)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr id_;
  ::PROTOBUF_NAMESPACE_ID::int32 strength_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_lbs_2eproto;
};
// -------------------------------------------------------------------

class LbsList :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:lbs.LbsList) */ {
 public:
  LbsList();
  virtual ~LbsList();

  LbsList(const LbsList& from);
  LbsList(LbsList&& from) noexcept
    : LbsList() {
    *this = ::std::move(from);
  }

  inline LbsList& operator=(const LbsList& from) {
    CopyFrom(from);
    return *this;
  }
  inline LbsList& operator=(LbsList&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const LbsList& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const LbsList* internal_default_instance() {
    return reinterpret_cast<const LbsList*>(
               &_LbsList_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(LbsList& a, LbsList& b) {
    a.Swap(&b);
  }
  inline void Swap(LbsList* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline LbsList* New() const final {
    return CreateMaybeMessage<LbsList>(nullptr);
  }

  LbsList* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<LbsList>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const LbsList& from);
  void MergeFrom(const LbsList& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(LbsList* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "lbs.LbsList";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_lbs_2eproto);
    return ::descriptor_table_lbs_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kInfoFieldNumber = 1,
    kImeiFieldNumber = 2,
    kMethodFieldNumber = 3,
  };
  // repeated .lbs.LbsInfo info = 1;
  int info_size() const;
  private:
  int _internal_info_size() const;
  public:
  void clear_info();
  ::lbs::LbsInfo* mutable_info(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::lbs::LbsInfo >*
      mutable_info();
  private:
  const ::lbs::LbsInfo& _internal_info(int index) const;
  ::lbs::LbsInfo* _internal_add_info();
  public:
  const ::lbs::LbsInfo& info(int index) const;
  ::lbs::LbsInfo* add_info();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::lbs::LbsInfo >&
      info() const;

  // string imei = 2;
  void clear_imei();
  const std::string& imei() const;
  void set_imei(const std::string& value);
  void set_imei(std::string&& value);
  void set_imei(const char* value);
  void set_imei(const char* value, size_t size);
  std::string* mutable_imei();
  std::string* release_imei();
  void set_allocated_imei(std::string* imei);
  private:
  const std::string& _internal_imei() const;
  void _internal_set_imei(const std::string& value);
  std::string* _internal_mutable_imei();
  public:

  // string method = 3;
  void clear_method();
  const std::string& method() const;
  void set_method(const std::string& value);
  void set_method(std::string&& value);
  void set_method(const char* value);
  void set_method(const char* value, size_t size);
  std::string* mutable_method();
  std::string* release_method();
  void set_allocated_method(std::string* method);
  private:
  const std::string& _internal_method() const;
  void _internal_set_method(const std::string& value);
  std::string* _internal_mutable_method();
  public:

  // @@protoc_insertion_point(class_scope:lbs.LbsList)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::lbs::LbsInfo > info_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr imei_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr method_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_lbs_2eproto;
};
// -------------------------------------------------------------------

class LbsRec :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:lbs.LbsRec) */ {
 public:
  LbsRec();
  virtual ~LbsRec();

  LbsRec(const LbsRec& from);
  LbsRec(LbsRec&& from) noexcept
    : LbsRec() {
    *this = ::std::move(from);
  }

  inline LbsRec& operator=(const LbsRec& from) {
    CopyFrom(from);
    return *this;
  }
  inline LbsRec& operator=(LbsRec&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const LbsRec& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const LbsRec* internal_default_instance() {
    return reinterpret_cast<const LbsRec*>(
               &_LbsRec_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(LbsRec& a, LbsRec& b) {
    a.Swap(&b);
  }
  inline void Swap(LbsRec* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline LbsRec* New() const final {
    return CreateMaybeMessage<LbsRec>(nullptr);
  }

  LbsRec* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<LbsRec>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const LbsRec& from);
  void MergeFrom(const LbsRec& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(LbsRec* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "lbs.LbsRec";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_lbs_2eproto);
    return ::descriptor_table_lbs_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kBsidFieldNumber = 1,
    kLonFieldNumber = 2,
    kLatFieldNumber = 3,
    kAccuracyFieldNumber = 4,
  };
  // string bsid = 1;
  void clear_bsid();
  const std::string& bsid() const;
  void set_bsid(const std::string& value);
  void set_bsid(std::string&& value);
  void set_bsid(const char* value);
  void set_bsid(const char* value, size_t size);
  std::string* mutable_bsid();
  std::string* release_bsid();
  void set_allocated_bsid(std::string* bsid);
  private:
  const std::string& _internal_bsid() const;
  void _internal_set_bsid(const std::string& value);
  std::string* _internal_mutable_bsid();
  public:

  // double lon = 2;
  void clear_lon();
  double lon() const;
  void set_lon(double value);
  private:
  double _internal_lon() const;
  void _internal_set_lon(double value);
  public:

  // double lat = 3;
  void clear_lat();
  double lat() const;
  void set_lat(double value);
  private:
  double _internal_lat() const;
  void _internal_set_lat(double value);
  public:

  // double accuracy = 4;
  void clear_accuracy();
  double accuracy() const;
  void set_accuracy(double value);
  private:
  double _internal_accuracy() const;
  void _internal_set_accuracy(double value);
  public:

  // @@protoc_insertion_point(class_scope:lbs.LbsRec)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr bsid_;
  double lon_;
  double lat_;
  double accuracy_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_lbs_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// LbsInfo

// string id = 1;
inline void LbsInfo::clear_id() {
  id_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& LbsInfo::id() const {
  // @@protoc_insertion_point(field_get:lbs.LbsInfo.id)
  return _internal_id();
}
inline void LbsInfo::set_id(const std::string& value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:lbs.LbsInfo.id)
}
inline std::string* LbsInfo::mutable_id() {
  // @@protoc_insertion_point(field_mutable:lbs.LbsInfo.id)
  return _internal_mutable_id();
}
inline const std::string& LbsInfo::_internal_id() const {
  return id_.GetNoArena();
}
inline void LbsInfo::_internal_set_id(const std::string& value) {
  
  id_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void LbsInfo::set_id(std::string&& value) {
  
  id_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:lbs.LbsInfo.id)
}
inline void LbsInfo::set_id(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  id_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:lbs.LbsInfo.id)
}
inline void LbsInfo::set_id(const char* value, size_t size) {
  
  id_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:lbs.LbsInfo.id)
}
inline std::string* LbsInfo::_internal_mutable_id() {
  
  return id_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* LbsInfo::release_id() {
  // @@protoc_insertion_point(field_release:lbs.LbsInfo.id)
  
  return id_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void LbsInfo::set_allocated_id(std::string* id) {
  if (id != nullptr) {
    
  } else {
    
  }
  id_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), id);
  // @@protoc_insertion_point(field_set_allocated:lbs.LbsInfo.id)
}

// int32 strength = 2;
inline void LbsInfo::clear_strength() {
  strength_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 LbsInfo::_internal_strength() const {
  return strength_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 LbsInfo::strength() const {
  // @@protoc_insertion_point(field_get:lbs.LbsInfo.strength)
  return _internal_strength();
}
inline void LbsInfo::_internal_set_strength(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  strength_ = value;
}
inline void LbsInfo::set_strength(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_strength(value);
  // @@protoc_insertion_point(field_set:lbs.LbsInfo.strength)
}

// -------------------------------------------------------------------

// LbsList

// repeated .lbs.LbsInfo info = 1;
inline int LbsList::_internal_info_size() const {
  return info_.size();
}
inline int LbsList::info_size() const {
  return _internal_info_size();
}
inline void LbsList::clear_info() {
  info_.Clear();
}
inline ::lbs::LbsInfo* LbsList::mutable_info(int index) {
  // @@protoc_insertion_point(field_mutable:lbs.LbsList.info)
  return info_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::lbs::LbsInfo >*
LbsList::mutable_info() {
  // @@protoc_insertion_point(field_mutable_list:lbs.LbsList.info)
  return &info_;
}
inline const ::lbs::LbsInfo& LbsList::_internal_info(int index) const {
  return info_.Get(index);
}
inline const ::lbs::LbsInfo& LbsList::info(int index) const {
  // @@protoc_insertion_point(field_get:lbs.LbsList.info)
  return _internal_info(index);
}
inline ::lbs::LbsInfo* LbsList::_internal_add_info() {
  return info_.Add();
}
inline ::lbs::LbsInfo* LbsList::add_info() {
  // @@protoc_insertion_point(field_add:lbs.LbsList.info)
  return _internal_add_info();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::lbs::LbsInfo >&
LbsList::info() const {
  // @@protoc_insertion_point(field_list:lbs.LbsList.info)
  return info_;
}

// string imei = 2;
inline void LbsList::clear_imei() {
  imei_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& LbsList::imei() const {
  // @@protoc_insertion_point(field_get:lbs.LbsList.imei)
  return _internal_imei();
}
inline void LbsList::set_imei(const std::string& value) {
  _internal_set_imei(value);
  // @@protoc_insertion_point(field_set:lbs.LbsList.imei)
}
inline std::string* LbsList::mutable_imei() {
  // @@protoc_insertion_point(field_mutable:lbs.LbsList.imei)
  return _internal_mutable_imei();
}
inline const std::string& LbsList::_internal_imei() const {
  return imei_.GetNoArena();
}
inline void LbsList::_internal_set_imei(const std::string& value) {
  
  imei_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void LbsList::set_imei(std::string&& value) {
  
  imei_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:lbs.LbsList.imei)
}
inline void LbsList::set_imei(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  imei_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:lbs.LbsList.imei)
}
inline void LbsList::set_imei(const char* value, size_t size) {
  
  imei_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:lbs.LbsList.imei)
}
inline std::string* LbsList::_internal_mutable_imei() {
  
  return imei_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* LbsList::release_imei() {
  // @@protoc_insertion_point(field_release:lbs.LbsList.imei)
  
  return imei_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void LbsList::set_allocated_imei(std::string* imei) {
  if (imei != nullptr) {
    
  } else {
    
  }
  imei_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), imei);
  // @@protoc_insertion_point(field_set_allocated:lbs.LbsList.imei)
}

// string method = 3;
inline void LbsList::clear_method() {
  method_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& LbsList::method() const {
  // @@protoc_insertion_point(field_get:lbs.LbsList.method)
  return _internal_method();
}
inline void LbsList::set_method(const std::string& value) {
  _internal_set_method(value);
  // @@protoc_insertion_point(field_set:lbs.LbsList.method)
}
inline std::string* LbsList::mutable_method() {
  // @@protoc_insertion_point(field_mutable:lbs.LbsList.method)
  return _internal_mutable_method();
}
inline const std::string& LbsList::_internal_method() const {
  return method_.GetNoArena();
}
inline void LbsList::_internal_set_method(const std::string& value) {
  
  method_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void LbsList::set_method(std::string&& value) {
  
  method_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:lbs.LbsList.method)
}
inline void LbsList::set_method(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  method_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:lbs.LbsList.method)
}
inline void LbsList::set_method(const char* value, size_t size) {
  
  method_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:lbs.LbsList.method)
}
inline std::string* LbsList::_internal_mutable_method() {
  
  return method_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* LbsList::release_method() {
  // @@protoc_insertion_point(field_release:lbs.LbsList.method)
  
  return method_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void LbsList::set_allocated_method(std::string* method) {
  if (method != nullptr) {
    
  } else {
    
  }
  method_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), method);
  // @@protoc_insertion_point(field_set_allocated:lbs.LbsList.method)
}

// -------------------------------------------------------------------

// LbsRec

// string bsid = 1;
inline void LbsRec::clear_bsid() {
  bsid_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& LbsRec::bsid() const {
  // @@protoc_insertion_point(field_get:lbs.LbsRec.bsid)
  return _internal_bsid();
}
inline void LbsRec::set_bsid(const std::string& value) {
  _internal_set_bsid(value);
  // @@protoc_insertion_point(field_set:lbs.LbsRec.bsid)
}
inline std::string* LbsRec::mutable_bsid() {
  // @@protoc_insertion_point(field_mutable:lbs.LbsRec.bsid)
  return _internal_mutable_bsid();
}
inline const std::string& LbsRec::_internal_bsid() const {
  return bsid_.GetNoArena();
}
inline void LbsRec::_internal_set_bsid(const std::string& value) {
  
  bsid_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void LbsRec::set_bsid(std::string&& value) {
  
  bsid_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:lbs.LbsRec.bsid)
}
inline void LbsRec::set_bsid(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  bsid_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:lbs.LbsRec.bsid)
}
inline void LbsRec::set_bsid(const char* value, size_t size) {
  
  bsid_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:lbs.LbsRec.bsid)
}
inline std::string* LbsRec::_internal_mutable_bsid() {
  
  return bsid_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* LbsRec::release_bsid() {
  // @@protoc_insertion_point(field_release:lbs.LbsRec.bsid)
  
  return bsid_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void LbsRec::set_allocated_bsid(std::string* bsid) {
  if (bsid != nullptr) {
    
  } else {
    
  }
  bsid_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), bsid);
  // @@protoc_insertion_point(field_set_allocated:lbs.LbsRec.bsid)
}

// double lon = 2;
inline void LbsRec::clear_lon() {
  lon_ = 0;
}
inline double LbsRec::_internal_lon() const {
  return lon_;
}
inline double LbsRec::lon() const {
  // @@protoc_insertion_point(field_get:lbs.LbsRec.lon)
  return _internal_lon();
}
inline void LbsRec::_internal_set_lon(double value) {
  
  lon_ = value;
}
inline void LbsRec::set_lon(double value) {
  _internal_set_lon(value);
  // @@protoc_insertion_point(field_set:lbs.LbsRec.lon)
}

// double lat = 3;
inline void LbsRec::clear_lat() {
  lat_ = 0;
}
inline double LbsRec::_internal_lat() const {
  return lat_;
}
inline double LbsRec::lat() const {
  // @@protoc_insertion_point(field_get:lbs.LbsRec.lat)
  return _internal_lat();
}
inline void LbsRec::_internal_set_lat(double value) {
  
  lat_ = value;
}
inline void LbsRec::set_lat(double value) {
  _internal_set_lat(value);
  // @@protoc_insertion_point(field_set:lbs.LbsRec.lat)
}

// double accuracy = 4;
inline void LbsRec::clear_accuracy() {
  accuracy_ = 0;
}
inline double LbsRec::_internal_accuracy() const {
  return accuracy_;
}
inline double LbsRec::accuracy() const {
  // @@protoc_insertion_point(field_get:lbs.LbsRec.accuracy)
  return _internal_accuracy();
}
inline void LbsRec::_internal_set_accuracy(double value) {
  
  accuracy_ = value;
}
inline void LbsRec::set_accuracy(double value) {
  _internal_set_accuracy(value);
  // @@protoc_insertion_point(field_set:lbs.LbsRec.accuracy)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace lbs

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_lbs_2eproto