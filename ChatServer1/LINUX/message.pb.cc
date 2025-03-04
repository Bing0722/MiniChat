// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: message.proto
// Protobuf C++ Version: 5.29.2

#include "message.pb.h"

#include <algorithm>
#include <type_traits>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/generated_message_tctable_impl.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/generated_message_util.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::google::protobuf;
namespace _pbi = ::google::protobuf::internal;
namespace _fl = ::google::protobuf::internal::field_layout;
namespace message {

inline constexpr GetVerifyRsp::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : email_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        code_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        error_{0},
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR GetVerifyRsp::GetVerifyRsp(::_pbi::ConstantInitialized)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(_class_data_.base()),
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(),
#endif  // PROTOBUF_CUSTOM_VTABLE
      _impl_(::_pbi::ConstantInitialized()) {
}
struct GetVerifyRspDefaultTypeInternal {
  PROTOBUF_CONSTEXPR GetVerifyRspDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~GetVerifyRspDefaultTypeInternal() {}
  union {
    GetVerifyRsp _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 GetVerifyRspDefaultTypeInternal _GetVerifyRsp_default_instance_;

inline constexpr GetVerifyReq::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : email_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR GetVerifyReq::GetVerifyReq(::_pbi::ConstantInitialized)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(_class_data_.base()),
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(),
#endif  // PROTOBUF_CUSTOM_VTABLE
      _impl_(::_pbi::ConstantInitialized()) {
}
struct GetVerifyReqDefaultTypeInternal {
  PROTOBUF_CONSTEXPR GetVerifyReqDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~GetVerifyReqDefaultTypeInternal() {}
  union {
    GetVerifyReq _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 GetVerifyReqDefaultTypeInternal _GetVerifyReq_default_instance_;
}  // namespace message
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_message_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_message_2eproto = nullptr;
const ::uint32_t
    TableStruct_message_2eproto::offsets[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
        protodesc_cold) = {
        ~0u,  // no _has_bits_
        PROTOBUF_FIELD_OFFSET(::message::GetVerifyReq, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::message::GetVerifyReq, _impl_.email_),
        ~0u,  // no _has_bits_
        PROTOBUF_FIELD_OFFSET(::message::GetVerifyRsp, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::message::GetVerifyRsp, _impl_.error_),
        PROTOBUF_FIELD_OFFSET(::message::GetVerifyRsp, _impl_.email_),
        PROTOBUF_FIELD_OFFSET(::message::GetVerifyRsp, _impl_.code_),
};

static const ::_pbi::MigrationSchema
    schemas[] ABSL_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
        {0, -1, -1, sizeof(::message::GetVerifyReq)},
        {9, -1, -1, sizeof(::message::GetVerifyRsp)},
};
static const ::_pb::Message* const file_default_instances[] = {
    &::message::_GetVerifyReq_default_instance_._instance,
    &::message::_GetVerifyRsp_default_instance_._instance,
};
const char descriptor_table_protodef_message_2eproto[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
    protodesc_cold) = {
    "\n\rmessage.proto\022\007message\"\035\n\014GetVerifyReq"
    "\022\r\n\005email\030\001 \001(\t\":\n\014GetVerifyRsp\022\r\n\005error"
    "\030\001 \001(\005\022\r\n\005email\030\002 \001(\t\022\014\n\004code\030\003 \001(\t2P\n\rV"
    "erifyService\022\?\n\rGetVerifyCode\022\025.message."
    "GetVerifyReq\032\025.message.GetVerifyRsp\"\000b\006p"
    "roto3"
};
static ::absl::once_flag descriptor_table_message_2eproto_once;
PROTOBUF_CONSTINIT const ::_pbi::DescriptorTable descriptor_table_message_2eproto = {
    false,
    false,
    205,
    descriptor_table_protodef_message_2eproto,
    "message.proto",
    &descriptor_table_message_2eproto_once,
    nullptr,
    0,
    2,
    schemas,
    file_default_instances,
    TableStruct_message_2eproto::offsets,
    file_level_enum_descriptors_message_2eproto,
    file_level_service_descriptors_message_2eproto,
};
namespace message {
// ===================================================================

class GetVerifyReq::_Internal {
 public:
};

GetVerifyReq::GetVerifyReq(::google::protobuf::Arena* arena)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:message.GetVerifyReq)
}
inline PROTOBUF_NDEBUG_INLINE GetVerifyReq::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from, const ::message::GetVerifyReq& from_msg)
      : email_(arena, from.email_),
        _cached_size_{0} {}

GetVerifyReq::GetVerifyReq(
    ::google::protobuf::Arena* arena,
    const GetVerifyReq& from)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  GetVerifyReq* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_, from);

  // @@protoc_insertion_point(copy_constructor:message.GetVerifyReq)
}
inline PROTOBUF_NDEBUG_INLINE GetVerifyReq::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : email_(arena),
        _cached_size_{0} {}

inline void GetVerifyReq::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
}
GetVerifyReq::~GetVerifyReq() {
  // @@protoc_insertion_point(destructor:message.GetVerifyReq)
  SharedDtor(*this);
}
inline void GetVerifyReq::SharedDtor(MessageLite& self) {
  GetVerifyReq& this_ = static_cast<GetVerifyReq&>(self);
  this_._internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  ABSL_DCHECK(this_.GetArena() == nullptr);
  this_._impl_.email_.Destroy();
  this_._impl_.~Impl_();
}

inline void* GetVerifyReq::PlacementNew_(const void*, void* mem,
                                        ::google::protobuf::Arena* arena) {
  return ::new (mem) GetVerifyReq(arena);
}
constexpr auto GetVerifyReq::InternalNewImpl_() {
  return ::google::protobuf::internal::MessageCreator::CopyInit(sizeof(GetVerifyReq),
                                            alignof(GetVerifyReq));
}
PROTOBUF_CONSTINIT
PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::google::protobuf::internal::ClassDataFull GetVerifyReq::_class_data_ = {
    ::google::protobuf::internal::ClassData{
        &_GetVerifyReq_default_instance_._instance,
        &_table_.header,
        nullptr,  // OnDemandRegisterArenaDtor
        nullptr,  // IsInitialized
        &GetVerifyReq::MergeImpl,
        ::google::protobuf::Message::GetNewImpl<GetVerifyReq>(),
#if defined(PROTOBUF_CUSTOM_VTABLE)
        &GetVerifyReq::SharedDtor,
        ::google::protobuf::Message::GetClearImpl<GetVerifyReq>(), &GetVerifyReq::ByteSizeLong,
            &GetVerifyReq::_InternalSerialize,
#endif  // PROTOBUF_CUSTOM_VTABLE
        PROTOBUF_FIELD_OFFSET(GetVerifyReq, _impl_._cached_size_),
        false,
    },
    &GetVerifyReq::kDescriptorMethods,
    &descriptor_table_message_2eproto,
    nullptr,  // tracker
};
const ::google::protobuf::internal::ClassData* GetVerifyReq::GetClassData() const {
  ::google::protobuf::internal::PrefetchToLocalCache(&_class_data_);
  ::google::protobuf::internal::PrefetchToLocalCache(_class_data_.tc_table);
  return _class_data_.base();
}
PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<0, 1, 0, 34, 2> GetVerifyReq::_table_ = {
  {
    0,  // no _has_bits_
    0, // no _extensions_
    1, 0,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967294,  // skipmap
    offsetof(decltype(_table_), field_entries),
    1,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    _class_data_.base(),
    nullptr,  // post_loop_handler
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::message::GetVerifyReq>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    // string email = 1;
    {::_pbi::TcParser::FastUS1,
     {10, 63, 0, PROTOBUF_FIELD_OFFSET(GetVerifyReq, _impl_.email_)}},
  }}, {{
    65535, 65535
  }}, {{
    // string email = 1;
    {PROTOBUF_FIELD_OFFSET(GetVerifyReq, _impl_.email_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
  }},
  // no aux_entries
  {{
    "\24\5\0\0\0\0\0\0"
    "message.GetVerifyReq"
    "email"
  }},
};

PROTOBUF_NOINLINE void GetVerifyReq::Clear() {
// @@protoc_insertion_point(message_clear_start:message.GetVerifyReq)
  ::google::protobuf::internal::TSanWrite(&_impl_);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.email_.ClearToEmpty();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::uint8_t* GetVerifyReq::_InternalSerialize(
            const MessageLite& base, ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) {
          const GetVerifyReq& this_ = static_cast<const GetVerifyReq&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::uint8_t* GetVerifyReq::_InternalSerialize(
            ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) const {
          const GetVerifyReq& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(serialize_to_array_start:message.GetVerifyReq)
          ::uint32_t cached_has_bits = 0;
          (void)cached_has_bits;

          // string email = 1;
          if (!this_._internal_email().empty()) {
            const std::string& _s = this_._internal_email();
            ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
                _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "message.GetVerifyReq.email");
            target = stream->WriteStringMaybeAliased(1, _s, target);
          }

          if (PROTOBUF_PREDICT_FALSE(this_._internal_metadata_.have_unknown_fields())) {
            target =
                ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
                    this_._internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
          }
          // @@protoc_insertion_point(serialize_to_array_end:message.GetVerifyReq)
          return target;
        }

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::size_t GetVerifyReq::ByteSizeLong(const MessageLite& base) {
          const GetVerifyReq& this_ = static_cast<const GetVerifyReq&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::size_t GetVerifyReq::ByteSizeLong() const {
          const GetVerifyReq& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(message_byte_size_start:message.GetVerifyReq)
          ::size_t total_size = 0;

          ::uint32_t cached_has_bits = 0;
          // Prevent compiler warnings about cached_has_bits being unused
          (void)cached_has_bits;

           {
            // string email = 1;
            if (!this_._internal_email().empty()) {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                              this_._internal_email());
            }
          }
          return this_.MaybeComputeUnknownFieldsSize(total_size,
                                                     &this_._impl_._cached_size_);
        }

void GetVerifyReq::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<GetVerifyReq*>(&to_msg);
  auto& from = static_cast<const GetVerifyReq&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:message.GetVerifyReq)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_email().empty()) {
    _this->_internal_set_email(from._internal_email());
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void GetVerifyReq::CopyFrom(const GetVerifyReq& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:message.GetVerifyReq)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}


void GetVerifyReq::InternalSwap(GetVerifyReq* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.email_, &other->_impl_.email_, arena);
}

::google::protobuf::Metadata GetVerifyReq::GetMetadata() const {
  return ::google::protobuf::Message::GetMetadataImpl(GetClassData()->full());
}
// ===================================================================

class GetVerifyRsp::_Internal {
 public:
};

GetVerifyRsp::GetVerifyRsp(::google::protobuf::Arena* arena)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:message.GetVerifyRsp)
}
inline PROTOBUF_NDEBUG_INLINE GetVerifyRsp::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from, const ::message::GetVerifyRsp& from_msg)
      : email_(arena, from.email_),
        code_(arena, from.code_),
        _cached_size_{0} {}

GetVerifyRsp::GetVerifyRsp(
    ::google::protobuf::Arena* arena,
    const GetVerifyRsp& from)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  GetVerifyRsp* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_, from);
  _impl_.error_ = from._impl_.error_;

  // @@protoc_insertion_point(copy_constructor:message.GetVerifyRsp)
}
inline PROTOBUF_NDEBUG_INLINE GetVerifyRsp::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : email_(arena),
        code_(arena),
        _cached_size_{0} {}

inline void GetVerifyRsp::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  _impl_.error_ = {};
}
GetVerifyRsp::~GetVerifyRsp() {
  // @@protoc_insertion_point(destructor:message.GetVerifyRsp)
  SharedDtor(*this);
}
inline void GetVerifyRsp::SharedDtor(MessageLite& self) {
  GetVerifyRsp& this_ = static_cast<GetVerifyRsp&>(self);
  this_._internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  ABSL_DCHECK(this_.GetArena() == nullptr);
  this_._impl_.email_.Destroy();
  this_._impl_.code_.Destroy();
  this_._impl_.~Impl_();
}

inline void* GetVerifyRsp::PlacementNew_(const void*, void* mem,
                                        ::google::protobuf::Arena* arena) {
  return ::new (mem) GetVerifyRsp(arena);
}
constexpr auto GetVerifyRsp::InternalNewImpl_() {
  return ::google::protobuf::internal::MessageCreator::CopyInit(sizeof(GetVerifyRsp),
                                            alignof(GetVerifyRsp));
}
PROTOBUF_CONSTINIT
PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::google::protobuf::internal::ClassDataFull GetVerifyRsp::_class_data_ = {
    ::google::protobuf::internal::ClassData{
        &_GetVerifyRsp_default_instance_._instance,
        &_table_.header,
        nullptr,  // OnDemandRegisterArenaDtor
        nullptr,  // IsInitialized
        &GetVerifyRsp::MergeImpl,
        ::google::protobuf::Message::GetNewImpl<GetVerifyRsp>(),
#if defined(PROTOBUF_CUSTOM_VTABLE)
        &GetVerifyRsp::SharedDtor,
        ::google::protobuf::Message::GetClearImpl<GetVerifyRsp>(), &GetVerifyRsp::ByteSizeLong,
            &GetVerifyRsp::_InternalSerialize,
#endif  // PROTOBUF_CUSTOM_VTABLE
        PROTOBUF_FIELD_OFFSET(GetVerifyRsp, _impl_._cached_size_),
        false,
    },
    &GetVerifyRsp::kDescriptorMethods,
    &descriptor_table_message_2eproto,
    nullptr,  // tracker
};
const ::google::protobuf::internal::ClassData* GetVerifyRsp::GetClassData() const {
  ::google::protobuf::internal::PrefetchToLocalCache(&_class_data_);
  ::google::protobuf::internal::PrefetchToLocalCache(_class_data_.tc_table);
  return _class_data_.base();
}
PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<2, 3, 0, 38, 2> GetVerifyRsp::_table_ = {
  {
    0,  // no _has_bits_
    0, // no _extensions_
    3, 24,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967288,  // skipmap
    offsetof(decltype(_table_), field_entries),
    3,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    _class_data_.base(),
    nullptr,  // post_loop_handler
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::message::GetVerifyRsp>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    {::_pbi::TcParser::MiniParse, {}},
    // int32 error = 1;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(GetVerifyRsp, _impl_.error_), 63>(),
     {8, 63, 0, PROTOBUF_FIELD_OFFSET(GetVerifyRsp, _impl_.error_)}},
    // string email = 2;
    {::_pbi::TcParser::FastUS1,
     {18, 63, 0, PROTOBUF_FIELD_OFFSET(GetVerifyRsp, _impl_.email_)}},
    // string code = 3;
    {::_pbi::TcParser::FastUS1,
     {26, 63, 0, PROTOBUF_FIELD_OFFSET(GetVerifyRsp, _impl_.code_)}},
  }}, {{
    65535, 65535
  }}, {{
    // int32 error = 1;
    {PROTOBUF_FIELD_OFFSET(GetVerifyRsp, _impl_.error_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kInt32)},
    // string email = 2;
    {PROTOBUF_FIELD_OFFSET(GetVerifyRsp, _impl_.email_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
    // string code = 3;
    {PROTOBUF_FIELD_OFFSET(GetVerifyRsp, _impl_.code_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
  }},
  // no aux_entries
  {{
    "\24\0\5\4\0\0\0\0"
    "message.GetVerifyRsp"
    "email"
    "code"
  }},
};

PROTOBUF_NOINLINE void GetVerifyRsp::Clear() {
// @@protoc_insertion_point(message_clear_start:message.GetVerifyRsp)
  ::google::protobuf::internal::TSanWrite(&_impl_);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.email_.ClearToEmpty();
  _impl_.code_.ClearToEmpty();
  _impl_.error_ = 0;
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::uint8_t* GetVerifyRsp::_InternalSerialize(
            const MessageLite& base, ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) {
          const GetVerifyRsp& this_ = static_cast<const GetVerifyRsp&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::uint8_t* GetVerifyRsp::_InternalSerialize(
            ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) const {
          const GetVerifyRsp& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(serialize_to_array_start:message.GetVerifyRsp)
          ::uint32_t cached_has_bits = 0;
          (void)cached_has_bits;

          // int32 error = 1;
          if (this_._internal_error() != 0) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<1>(
                    stream, this_._internal_error(), target);
          }

          // string email = 2;
          if (!this_._internal_email().empty()) {
            const std::string& _s = this_._internal_email();
            ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
                _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "message.GetVerifyRsp.email");
            target = stream->WriteStringMaybeAliased(2, _s, target);
          }

          // string code = 3;
          if (!this_._internal_code().empty()) {
            const std::string& _s = this_._internal_code();
            ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
                _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "message.GetVerifyRsp.code");
            target = stream->WriteStringMaybeAliased(3, _s, target);
          }

          if (PROTOBUF_PREDICT_FALSE(this_._internal_metadata_.have_unknown_fields())) {
            target =
                ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
                    this_._internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
          }
          // @@protoc_insertion_point(serialize_to_array_end:message.GetVerifyRsp)
          return target;
        }

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::size_t GetVerifyRsp::ByteSizeLong(const MessageLite& base) {
          const GetVerifyRsp& this_ = static_cast<const GetVerifyRsp&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::size_t GetVerifyRsp::ByteSizeLong() const {
          const GetVerifyRsp& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(message_byte_size_start:message.GetVerifyRsp)
          ::size_t total_size = 0;

          ::uint32_t cached_has_bits = 0;
          // Prevent compiler warnings about cached_has_bits being unused
          (void)cached_has_bits;

          ::_pbi::Prefetch5LinesFrom7Lines(&this_);
           {
            // string email = 2;
            if (!this_._internal_email().empty()) {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                              this_._internal_email());
            }
            // string code = 3;
            if (!this_._internal_code().empty()) {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                              this_._internal_code());
            }
            // int32 error = 1;
            if (this_._internal_error() != 0) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_error());
            }
          }
          return this_.MaybeComputeUnknownFieldsSize(total_size,
                                                     &this_._impl_._cached_size_);
        }

void GetVerifyRsp::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<GetVerifyRsp*>(&to_msg);
  auto& from = static_cast<const GetVerifyRsp&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:message.GetVerifyRsp)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_email().empty()) {
    _this->_internal_set_email(from._internal_email());
  }
  if (!from._internal_code().empty()) {
    _this->_internal_set_code(from._internal_code());
  }
  if (from._internal_error() != 0) {
    _this->_impl_.error_ = from._impl_.error_;
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void GetVerifyRsp::CopyFrom(const GetVerifyRsp& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:message.GetVerifyRsp)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}


void GetVerifyRsp::InternalSwap(GetVerifyRsp* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.email_, &other->_impl_.email_, arena);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.code_, &other->_impl_.code_, arena);
        swap(_impl_.error_, other->_impl_.error_);
}

::google::protobuf::Metadata GetVerifyRsp::GetMetadata() const {
  return ::google::protobuf::Message::GetMetadataImpl(GetClassData()->full());
}
// @@protoc_insertion_point(namespace_scope)
}  // namespace message
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(global_scope)
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::std::false_type
    _static_init2_ PROTOBUF_UNUSED =
        (::_pbi::AddDescriptors(&descriptor_table_message_2eproto),
         ::std::false_type{});
#include "google/protobuf/port_undef.inc"
