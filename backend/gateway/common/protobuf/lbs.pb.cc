// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: lbs.proto

#include "lbs.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
extern PROTOBUF_INTERNAL_EXPORT_lbs_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_LbsInfo_lbs_2eproto;
namespace lbs {
class LbsInfoDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<LbsInfo> _instance;
} _LbsInfo_default_instance_;
class LbsListDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<LbsList> _instance;
} _LbsList_default_instance_;
class LbsRecDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<LbsRec> _instance;
} _LbsRec_default_instance_;
}  // namespace lbs
static void InitDefaultsscc_info_LbsInfo_lbs_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::lbs::_LbsInfo_default_instance_;
    new (ptr) ::lbs::LbsInfo();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::lbs::LbsInfo::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_LbsInfo_lbs_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_LbsInfo_lbs_2eproto}, {}};

static void InitDefaultsscc_info_LbsList_lbs_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::lbs::_LbsList_default_instance_;
    new (ptr) ::lbs::LbsList();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::lbs::LbsList::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_LbsList_lbs_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 1, 0, InitDefaultsscc_info_LbsList_lbs_2eproto}, {
      &scc_info_LbsInfo_lbs_2eproto.base,}};

static void InitDefaultsscc_info_LbsRec_lbs_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::lbs::_LbsRec_default_instance_;
    new (ptr) ::lbs::LbsRec();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::lbs::LbsRec::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_LbsRec_lbs_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_LbsRec_lbs_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_lbs_2eproto[3];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_lbs_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_lbs_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_lbs_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::lbs::LbsInfo, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::lbs::LbsInfo, id_),
  PROTOBUF_FIELD_OFFSET(::lbs::LbsInfo, strength_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::lbs::LbsList, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::lbs::LbsList, info_),
  PROTOBUF_FIELD_OFFSET(::lbs::LbsList, imei_),
  PROTOBUF_FIELD_OFFSET(::lbs::LbsList, method_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::lbs::LbsRec, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::lbs::LbsRec, bsid_),
  PROTOBUF_FIELD_OFFSET(::lbs::LbsRec, lon_),
  PROTOBUF_FIELD_OFFSET(::lbs::LbsRec, lat_),
  PROTOBUF_FIELD_OFFSET(::lbs::LbsRec, accuracy_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::lbs::LbsInfo)},
  { 7, -1, sizeof(::lbs::LbsList)},
  { 15, -1, sizeof(::lbs::LbsRec)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::lbs::_LbsInfo_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::lbs::_LbsList_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::lbs::_LbsRec_default_instance_),
};

const char descriptor_table_protodef_lbs_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\tlbs.proto\022\003lbs\"\'\n\007LbsInfo\022\n\n\002id\030\001 \001(\t\022"
  "\020\n\010strength\030\002 \001(\005\"C\n\007LbsList\022\032\n\004info\030\001 \003"
  "(\0132\014.lbs.LbsInfo\022\014\n\004imei\030\002 \001(\t\022\016\n\006method"
  "\030\003 \001(\t\"B\n\006LbsRec\022\014\n\004bsid\030\001 \001(\t\022\013\n\003lon\030\002 "
  "\001(\001\022\013\n\003lat\030\003 \001(\001\022\020\n\010accuracy\030\004 \001(\001b\006prot"
  "o3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_lbs_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_lbs_2eproto_sccs[3] = {
  &scc_info_LbsInfo_lbs_2eproto.base,
  &scc_info_LbsList_lbs_2eproto.base,
  &scc_info_LbsRec_lbs_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_lbs_2eproto_once;
static bool descriptor_table_lbs_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_lbs_2eproto = {
  &descriptor_table_lbs_2eproto_initialized, descriptor_table_protodef_lbs_2eproto, "lbs.proto", 202,
  &descriptor_table_lbs_2eproto_once, descriptor_table_lbs_2eproto_sccs, descriptor_table_lbs_2eproto_deps, 3, 0,
  schemas, file_default_instances, TableStruct_lbs_2eproto::offsets,
  file_level_metadata_lbs_2eproto, 3, file_level_enum_descriptors_lbs_2eproto, file_level_service_descriptors_lbs_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_lbs_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_lbs_2eproto), true);
namespace lbs {

// ===================================================================

void LbsInfo::InitAsDefaultInstance() {
}
class LbsInfo::_Internal {
 public:
};

LbsInfo::LbsInfo()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:lbs.LbsInfo)
}
LbsInfo::LbsInfo(const LbsInfo& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_id().empty()) {
    id_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.id_);
  }
  strength_ = from.strength_;
  // @@protoc_insertion_point(copy_constructor:lbs.LbsInfo)
}

void LbsInfo::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_LbsInfo_lbs_2eproto.base);
  id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  strength_ = 0;
}

LbsInfo::~LbsInfo() {
  // @@protoc_insertion_point(destructor:lbs.LbsInfo)
  SharedDtor();
}

void LbsInfo::SharedDtor() {
  id_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void LbsInfo::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const LbsInfo& LbsInfo::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_LbsInfo_lbs_2eproto.base);
  return *internal_default_instance();
}


void LbsInfo::Clear() {
// @@protoc_insertion_point(message_clear_start:lbs.LbsInfo)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  id_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  strength_ = 0;
  _internal_metadata_.Clear();
}

const char* LbsInfo::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // string id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          auto str = _internal_mutable_id();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "lbs.LbsInfo.id"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 strength = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          strength_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* LbsInfo::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:lbs.LbsInfo)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string id = 1;
  if (this->id().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_id().data(), static_cast<int>(this->_internal_id().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "lbs.LbsInfo.id");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_id(), target);
  }

  // int32 strength = 2;
  if (this->strength() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(2, this->_internal_strength(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:lbs.LbsInfo)
  return target;
}

size_t LbsInfo::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:lbs.LbsInfo)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string id = 1;
  if (this->id().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_id());
  }

  // int32 strength = 2;
  if (this->strength() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_strength());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void LbsInfo::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:lbs.LbsInfo)
  GOOGLE_DCHECK_NE(&from, this);
  const LbsInfo* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<LbsInfo>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:lbs.LbsInfo)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:lbs.LbsInfo)
    MergeFrom(*source);
  }
}

void LbsInfo::MergeFrom(const LbsInfo& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:lbs.LbsInfo)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.id().size() > 0) {

    id_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.id_);
  }
  if (from.strength() != 0) {
    _internal_set_strength(from._internal_strength());
  }
}

void LbsInfo::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:lbs.LbsInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void LbsInfo::CopyFrom(const LbsInfo& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:lbs.LbsInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LbsInfo::IsInitialized() const {
  return true;
}

void LbsInfo::InternalSwap(LbsInfo* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  id_.Swap(&other->id_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(strength_, other->strength_);
}

::PROTOBUF_NAMESPACE_ID::Metadata LbsInfo::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void LbsList::InitAsDefaultInstance() {
}
class LbsList::_Internal {
 public:
};

LbsList::LbsList()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:lbs.LbsList)
}
LbsList::LbsList(const LbsList& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      info_(from.info_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  imei_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_imei().empty()) {
    imei_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.imei_);
  }
  method_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_method().empty()) {
    method_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.method_);
  }
  // @@protoc_insertion_point(copy_constructor:lbs.LbsList)
}

void LbsList::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_LbsList_lbs_2eproto.base);
  imei_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  method_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

LbsList::~LbsList() {
  // @@protoc_insertion_point(destructor:lbs.LbsList)
  SharedDtor();
}

void LbsList::SharedDtor() {
  imei_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  method_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void LbsList::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const LbsList& LbsList::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_LbsList_lbs_2eproto.base);
  return *internal_default_instance();
}


void LbsList::Clear() {
// @@protoc_insertion_point(message_clear_start:lbs.LbsList)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  info_.Clear();
  imei_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  method_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  _internal_metadata_.Clear();
}

const char* LbsList::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // repeated .lbs.LbsInfo info = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_info(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<10>(ptr));
        } else goto handle_unusual;
        continue;
      // string imei = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          auto str = _internal_mutable_imei();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "lbs.LbsList.imei"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string method = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          auto str = _internal_mutable_method();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "lbs.LbsList.method"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* LbsList::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:lbs.LbsList)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .lbs.LbsInfo info = 1;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_info_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(1, this->_internal_info(i), target, stream);
  }

  // string imei = 2;
  if (this->imei().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_imei().data(), static_cast<int>(this->_internal_imei().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "lbs.LbsList.imei");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_imei(), target);
  }

  // string method = 3;
  if (this->method().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_method().data(), static_cast<int>(this->_internal_method().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "lbs.LbsList.method");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_method(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:lbs.LbsList)
  return target;
}

size_t LbsList::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:lbs.LbsList)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .lbs.LbsInfo info = 1;
  total_size += 1UL * this->_internal_info_size();
  for (const auto& msg : this->info_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // string imei = 2;
  if (this->imei().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_imei());
  }

  // string method = 3;
  if (this->method().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_method());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void LbsList::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:lbs.LbsList)
  GOOGLE_DCHECK_NE(&from, this);
  const LbsList* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<LbsList>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:lbs.LbsList)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:lbs.LbsList)
    MergeFrom(*source);
  }
}

void LbsList::MergeFrom(const LbsList& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:lbs.LbsList)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  info_.MergeFrom(from.info_);
  if (from.imei().size() > 0) {

    imei_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.imei_);
  }
  if (from.method().size() > 0) {

    method_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.method_);
  }
}

void LbsList::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:lbs.LbsList)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void LbsList::CopyFrom(const LbsList& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:lbs.LbsList)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LbsList::IsInitialized() const {
  return true;
}

void LbsList::InternalSwap(LbsList* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  info_.InternalSwap(&other->info_);
  imei_.Swap(&other->imei_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  method_.Swap(&other->method_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
}

::PROTOBUF_NAMESPACE_ID::Metadata LbsList::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void LbsRec::InitAsDefaultInstance() {
}
class LbsRec::_Internal {
 public:
};

LbsRec::LbsRec()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:lbs.LbsRec)
}
LbsRec::LbsRec(const LbsRec& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  bsid_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_bsid().empty()) {
    bsid_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.bsid_);
  }
  ::memcpy(&lon_, &from.lon_,
    static_cast<size_t>(reinterpret_cast<char*>(&accuracy_) -
    reinterpret_cast<char*>(&lon_)) + sizeof(accuracy_));
  // @@protoc_insertion_point(copy_constructor:lbs.LbsRec)
}

void LbsRec::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_LbsRec_lbs_2eproto.base);
  bsid_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&lon_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&accuracy_) -
      reinterpret_cast<char*>(&lon_)) + sizeof(accuracy_));
}

LbsRec::~LbsRec() {
  // @@protoc_insertion_point(destructor:lbs.LbsRec)
  SharedDtor();
}

void LbsRec::SharedDtor() {
  bsid_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void LbsRec::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const LbsRec& LbsRec::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_LbsRec_lbs_2eproto.base);
  return *internal_default_instance();
}


void LbsRec::Clear() {
// @@protoc_insertion_point(message_clear_start:lbs.LbsRec)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  bsid_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&lon_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&accuracy_) -
      reinterpret_cast<char*>(&lon_)) + sizeof(accuracy_));
  _internal_metadata_.Clear();
}

const char* LbsRec::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // string bsid = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          auto str = _internal_mutable_bsid();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "lbs.LbsRec.bsid"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // double lon = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 17)) {
          lon_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else goto handle_unusual;
        continue;
      // double lat = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 25)) {
          lat_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else goto handle_unusual;
        continue;
      // double accuracy = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 33)) {
          accuracy_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* LbsRec::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:lbs.LbsRec)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string bsid = 1;
  if (this->bsid().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_bsid().data(), static_cast<int>(this->_internal_bsid().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "lbs.LbsRec.bsid");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_bsid(), target);
  }

  // double lon = 2;
  if (!(this->lon() <= 0 && this->lon() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(2, this->_internal_lon(), target);
  }

  // double lat = 3;
  if (!(this->lat() <= 0 && this->lat() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(3, this->_internal_lat(), target);
  }

  // double accuracy = 4;
  if (!(this->accuracy() <= 0 && this->accuracy() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(4, this->_internal_accuracy(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:lbs.LbsRec)
  return target;
}

size_t LbsRec::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:lbs.LbsRec)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string bsid = 1;
  if (this->bsid().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_bsid());
  }

  // double lon = 2;
  if (!(this->lon() <= 0 && this->lon() >= 0)) {
    total_size += 1 + 8;
  }

  // double lat = 3;
  if (!(this->lat() <= 0 && this->lat() >= 0)) {
    total_size += 1 + 8;
  }

  // double accuracy = 4;
  if (!(this->accuracy() <= 0 && this->accuracy() >= 0)) {
    total_size += 1 + 8;
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void LbsRec::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:lbs.LbsRec)
  GOOGLE_DCHECK_NE(&from, this);
  const LbsRec* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<LbsRec>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:lbs.LbsRec)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:lbs.LbsRec)
    MergeFrom(*source);
  }
}

void LbsRec::MergeFrom(const LbsRec& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:lbs.LbsRec)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.bsid().size() > 0) {

    bsid_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.bsid_);
  }
  if (!(from.lon() <= 0 && from.lon() >= 0)) {
    _internal_set_lon(from._internal_lon());
  }
  if (!(from.lat() <= 0 && from.lat() >= 0)) {
    _internal_set_lat(from._internal_lat());
  }
  if (!(from.accuracy() <= 0 && from.accuracy() >= 0)) {
    _internal_set_accuracy(from._internal_accuracy());
  }
}

void LbsRec::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:lbs.LbsRec)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void LbsRec::CopyFrom(const LbsRec& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:lbs.LbsRec)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LbsRec::IsInitialized() const {
  return true;
}

void LbsRec::InternalSwap(LbsRec* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  bsid_.Swap(&other->bsid_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(lon_, other->lon_);
  swap(lat_, other->lat_);
  swap(accuracy_, other->accuracy_);
}

::PROTOBUF_NAMESPACE_ID::Metadata LbsRec::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace lbs
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::lbs::LbsInfo* Arena::CreateMaybeMessage< ::lbs::LbsInfo >(Arena* arena) {
  return Arena::CreateInternal< ::lbs::LbsInfo >(arena);
}
template<> PROTOBUF_NOINLINE ::lbs::LbsList* Arena::CreateMaybeMessage< ::lbs::LbsList >(Arena* arena) {
  return Arena::CreateInternal< ::lbs::LbsList >(arena);
}
template<> PROTOBUF_NOINLINE ::lbs::LbsRec* Arena::CreateMaybeMessage< ::lbs::LbsRec >(Arena* arena) {
  return Arena::CreateInternal< ::lbs::LbsRec >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>