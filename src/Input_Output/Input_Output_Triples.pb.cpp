// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Input_Output_Triples.proto

#include "Input_Output_Triples.pb.h"

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

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

PROTOBUF_CONSTEXPR AssociatedTriple::AssociatedTriple(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.share_a_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.share_b_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.share_c_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct AssociatedTripleDefaultTypeInternal {
  PROTOBUF_CONSTEXPR AssociatedTripleDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~AssociatedTripleDefaultTypeInternal() {}
  union {
    AssociatedTriple _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 AssociatedTripleDefaultTypeInternal _AssociatedTriple_default_instance_;
PROTOBUF_CONSTEXPR TripleShare::TripleShare(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.associated_a_)*/nullptr
  , /*decltype(_impl_.associated_b_)*/nullptr
  , /*decltype(_impl_.associated_c_)*/nullptr
  , /*decltype(_impl_.share_a_)*/int64_t{0}
  , /*decltype(_impl_.share_b_)*/int64_t{0}
  , /*decltype(_impl_.share_c_)*/int64_t{0}
  , /*decltype(_impl_.player_nr_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct TripleShareDefaultTypeInternal {
  PROTOBUF_CONSTEXPR TripleShareDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~TripleShareDefaultTypeInternal() {}
  union {
    TripleShare _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 TripleShareDefaultTypeInternal _TripleShare_default_instance_;
static ::_pb::Metadata file_level_metadata_Input_5fOutput_5fTriples_2eproto[2];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_Input_5fOutput_5fTriples_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_Input_5fOutput_5fTriples_2eproto = nullptr;

const uint32_t TableStruct_Input_5fOutput_5fTriples_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::AssociatedTriple, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::AssociatedTriple, _impl_.share_a_),
  PROTOBUF_FIELD_OFFSET(::AssociatedTriple, _impl_.share_b_),
  PROTOBUF_FIELD_OFFSET(::AssociatedTriple, _impl_.share_c_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::TripleShare, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::TripleShare, _impl_.player_nr_),
  PROTOBUF_FIELD_OFFSET(::TripleShare, _impl_.share_a_),
  PROTOBUF_FIELD_OFFSET(::TripleShare, _impl_.share_b_),
  PROTOBUF_FIELD_OFFSET(::TripleShare, _impl_.share_c_),
  PROTOBUF_FIELD_OFFSET(::TripleShare, _impl_.associated_a_),
  PROTOBUF_FIELD_OFFSET(::TripleShare, _impl_.associated_b_),
  PROTOBUF_FIELD_OFFSET(::TripleShare, _impl_.associated_c_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::AssociatedTriple)},
  { 9, -1, -1, sizeof(::TripleShare)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::_AssociatedTriple_default_instance_._instance,
  &::_TripleShare_default_instance_._instance,
};

const char descriptor_table_protodef_Input_5fOutput_5fTriples_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\032Input_Output_Triples.proto\"E\n\020Associat"
  "edTriple\022\017\n\007share_a\030\001 \001(\t\022\017\n\007share_b\030\002 \001"
  "(\t\022\017\n\007share_c\030\003 \001(\t\"\316\001\n\013TripleShare\022\021\n\tp"
  "layer_nr\030\001 \001(\005\022\017\n\007share_a\030\002 \001(\003\022\017\n\007share"
  "_b\030\003 \001(\003\022\017\n\007share_c\030\004 \001(\003\022\'\n\014associated_"
  "a\030\005 \001(\0132\021.AssociatedTriple\022\'\n\014associated"
  "_b\030\006 \001(\0132\021.AssociatedTriple\022\'\n\014associate"
  "d_c\030\007 \001(\0132\021.AssociatedTripleb\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_Input_5fOutput_5fTriples_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_Input_5fOutput_5fTriples_2eproto = {
    false, false, 316, descriptor_table_protodef_Input_5fOutput_5fTriples_2eproto,
    "Input_Output_Triples.proto",
    &descriptor_table_Input_5fOutput_5fTriples_2eproto_once, nullptr, 0, 2,
    schemas, file_default_instances, TableStruct_Input_5fOutput_5fTriples_2eproto::offsets,
    file_level_metadata_Input_5fOutput_5fTriples_2eproto, file_level_enum_descriptors_Input_5fOutput_5fTriples_2eproto,
    file_level_service_descriptors_Input_5fOutput_5fTriples_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_Input_5fOutput_5fTriples_2eproto_getter() {
  return &descriptor_table_Input_5fOutput_5fTriples_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_Input_5fOutput_5fTriples_2eproto(&descriptor_table_Input_5fOutput_5fTriples_2eproto);

// ===================================================================

class AssociatedTriple::_Internal {
 public:
};

AssociatedTriple::AssociatedTriple(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:AssociatedTriple)
}
AssociatedTriple::AssociatedTriple(const AssociatedTriple& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  AssociatedTriple* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.share_a_){}
    , decltype(_impl_.share_b_){}
    , decltype(_impl_.share_c_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.share_a_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.share_a_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_share_a().empty()) {
    _this->_impl_.share_a_.Set(from._internal_share_a(), 
      _this->GetArenaForAllocation());
  }
  _impl_.share_b_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.share_b_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_share_b().empty()) {
    _this->_impl_.share_b_.Set(from._internal_share_b(), 
      _this->GetArenaForAllocation());
  }
  _impl_.share_c_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.share_c_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_share_c().empty()) {
    _this->_impl_.share_c_.Set(from._internal_share_c(), 
      _this->GetArenaForAllocation());
  }
  // @@protoc_insertion_point(copy_constructor:AssociatedTriple)
}

inline void AssociatedTriple::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.share_a_){}
    , decltype(_impl_.share_b_){}
    , decltype(_impl_.share_c_){}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.share_a_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.share_a_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.share_b_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.share_b_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.share_c_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.share_c_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

AssociatedTriple::~AssociatedTriple() {
  // @@protoc_insertion_point(destructor:AssociatedTriple)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void AssociatedTriple::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.share_a_.Destroy();
  _impl_.share_b_.Destroy();
  _impl_.share_c_.Destroy();
}

void AssociatedTriple::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void AssociatedTriple::Clear() {
// @@protoc_insertion_point(message_clear_start:AssociatedTriple)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.share_a_.ClearToEmpty();
  _impl_.share_b_.ClearToEmpty();
  _impl_.share_c_.ClearToEmpty();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* AssociatedTriple::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string share_a = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_share_a();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "AssociatedTriple.share_a"));
        } else
          goto handle_unusual;
        continue;
      // string share_b = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          auto str = _internal_mutable_share_b();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "AssociatedTriple.share_b"));
        } else
          goto handle_unusual;
        continue;
      // string share_c = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 26)) {
          auto str = _internal_mutable_share_c();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "AssociatedTriple.share_c"));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* AssociatedTriple::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:AssociatedTriple)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string share_a = 1;
  if (!this->_internal_share_a().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_share_a().data(), static_cast<int>(this->_internal_share_a().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "AssociatedTriple.share_a");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_share_a(), target);
  }

  // string share_b = 2;
  if (!this->_internal_share_b().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_share_b().data(), static_cast<int>(this->_internal_share_b().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "AssociatedTriple.share_b");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_share_b(), target);
  }

  // string share_c = 3;
  if (!this->_internal_share_c().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_share_c().data(), static_cast<int>(this->_internal_share_c().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "AssociatedTriple.share_c");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_share_c(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:AssociatedTriple)
  return target;
}

size_t AssociatedTriple::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:AssociatedTriple)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string share_a = 1;
  if (!this->_internal_share_a().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_share_a());
  }

  // string share_b = 2;
  if (!this->_internal_share_b().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_share_b());
  }

  // string share_c = 3;
  if (!this->_internal_share_c().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_share_c());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData AssociatedTriple::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    AssociatedTriple::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*AssociatedTriple::GetClassData() const { return &_class_data_; }


void AssociatedTriple::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<AssociatedTriple*>(&to_msg);
  auto& from = static_cast<const AssociatedTriple&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:AssociatedTriple)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_share_a().empty()) {
    _this->_internal_set_share_a(from._internal_share_a());
  }
  if (!from._internal_share_b().empty()) {
    _this->_internal_set_share_b(from._internal_share_b());
  }
  if (!from._internal_share_c().empty()) {
    _this->_internal_set_share_c(from._internal_share_c());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void AssociatedTriple::CopyFrom(const AssociatedTriple& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:AssociatedTriple)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool AssociatedTriple::IsInitialized() const {
  return true;
}

void AssociatedTriple::InternalSwap(AssociatedTriple* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.share_a_, lhs_arena,
      &other->_impl_.share_a_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.share_b_, lhs_arena,
      &other->_impl_.share_b_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.share_c_, lhs_arena,
      &other->_impl_.share_c_, rhs_arena
  );
}

::PROTOBUF_NAMESPACE_ID::Metadata AssociatedTriple::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_Input_5fOutput_5fTriples_2eproto_getter, &descriptor_table_Input_5fOutput_5fTriples_2eproto_once,
      file_level_metadata_Input_5fOutput_5fTriples_2eproto[0]);
}

// ===================================================================

class TripleShare::_Internal {
 public:
  static const ::AssociatedTriple& associated_a(const TripleShare* msg);
  static const ::AssociatedTriple& associated_b(const TripleShare* msg);
  static const ::AssociatedTriple& associated_c(const TripleShare* msg);
};

const ::AssociatedTriple&
TripleShare::_Internal::associated_a(const TripleShare* msg) {
  return *msg->_impl_.associated_a_;
}
const ::AssociatedTriple&
TripleShare::_Internal::associated_b(const TripleShare* msg) {
  return *msg->_impl_.associated_b_;
}
const ::AssociatedTriple&
TripleShare::_Internal::associated_c(const TripleShare* msg) {
  return *msg->_impl_.associated_c_;
}
TripleShare::TripleShare(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:TripleShare)
}
TripleShare::TripleShare(const TripleShare& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  TripleShare* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.associated_a_){nullptr}
    , decltype(_impl_.associated_b_){nullptr}
    , decltype(_impl_.associated_c_){nullptr}
    , decltype(_impl_.share_a_){}
    , decltype(_impl_.share_b_){}
    , decltype(_impl_.share_c_){}
    , decltype(_impl_.player_nr_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_associated_a()) {
    _this->_impl_.associated_a_ = new ::AssociatedTriple(*from._impl_.associated_a_);
  }
  if (from._internal_has_associated_b()) {
    _this->_impl_.associated_b_ = new ::AssociatedTriple(*from._impl_.associated_b_);
  }
  if (from._internal_has_associated_c()) {
    _this->_impl_.associated_c_ = new ::AssociatedTriple(*from._impl_.associated_c_);
  }
  ::memcpy(&_impl_.share_a_, &from._impl_.share_a_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.player_nr_) -
    reinterpret_cast<char*>(&_impl_.share_a_)) + sizeof(_impl_.player_nr_));
  // @@protoc_insertion_point(copy_constructor:TripleShare)
}

inline void TripleShare::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.associated_a_){nullptr}
    , decltype(_impl_.associated_b_){nullptr}
    , decltype(_impl_.associated_c_){nullptr}
    , decltype(_impl_.share_a_){int64_t{0}}
    , decltype(_impl_.share_b_){int64_t{0}}
    , decltype(_impl_.share_c_){int64_t{0}}
    , decltype(_impl_.player_nr_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

TripleShare::~TripleShare() {
  // @@protoc_insertion_point(destructor:TripleShare)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void TripleShare::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  if (this != internal_default_instance()) delete _impl_.associated_a_;
  if (this != internal_default_instance()) delete _impl_.associated_b_;
  if (this != internal_default_instance()) delete _impl_.associated_c_;
}

void TripleShare::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void TripleShare::Clear() {
// @@protoc_insertion_point(message_clear_start:TripleShare)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArenaForAllocation() == nullptr && _impl_.associated_a_ != nullptr) {
    delete _impl_.associated_a_;
  }
  _impl_.associated_a_ = nullptr;
  if (GetArenaForAllocation() == nullptr && _impl_.associated_b_ != nullptr) {
    delete _impl_.associated_b_;
  }
  _impl_.associated_b_ = nullptr;
  if (GetArenaForAllocation() == nullptr && _impl_.associated_c_ != nullptr) {
    delete _impl_.associated_c_;
  }
  _impl_.associated_c_ = nullptr;
  ::memset(&_impl_.share_a_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.player_nr_) -
      reinterpret_cast<char*>(&_impl_.share_a_)) + sizeof(_impl_.player_nr_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* TripleShare::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 player_nr = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.player_nr_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int64 share_a = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          _impl_.share_a_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int64 share_b = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 24)) {
          _impl_.share_b_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int64 share_c = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 32)) {
          _impl_.share_c_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .AssociatedTriple associated_a = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 42)) {
          ptr = ctx->ParseMessage(_internal_mutable_associated_a(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .AssociatedTriple associated_b = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 50)) {
          ptr = ctx->ParseMessage(_internal_mutable_associated_b(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .AssociatedTriple associated_c = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 58)) {
          ptr = ctx->ParseMessage(_internal_mutable_associated_c(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* TripleShare::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:TripleShare)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 player_nr = 1;
  if (this->_internal_player_nr() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(1, this->_internal_player_nr(), target);
  }

  // int64 share_a = 2;
  if (this->_internal_share_a() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt64ToArray(2, this->_internal_share_a(), target);
  }

  // int64 share_b = 3;
  if (this->_internal_share_b() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt64ToArray(3, this->_internal_share_b(), target);
  }

  // int64 share_c = 4;
  if (this->_internal_share_c() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt64ToArray(4, this->_internal_share_c(), target);
  }

  // .AssociatedTriple associated_a = 5;
  if (this->_internal_has_associated_a()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(5, _Internal::associated_a(this),
        _Internal::associated_a(this).GetCachedSize(), target, stream);
  }

  // .AssociatedTriple associated_b = 6;
  if (this->_internal_has_associated_b()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(6, _Internal::associated_b(this),
        _Internal::associated_b(this).GetCachedSize(), target, stream);
  }

  // .AssociatedTriple associated_c = 7;
  if (this->_internal_has_associated_c()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(7, _Internal::associated_c(this),
        _Internal::associated_c(this).GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:TripleShare)
  return target;
}

size_t TripleShare::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:TripleShare)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // .AssociatedTriple associated_a = 5;
  if (this->_internal_has_associated_a()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.associated_a_);
  }

  // .AssociatedTriple associated_b = 6;
  if (this->_internal_has_associated_b()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.associated_b_);
  }

  // .AssociatedTriple associated_c = 7;
  if (this->_internal_has_associated_c()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.associated_c_);
  }

  // int64 share_a = 2;
  if (this->_internal_share_a() != 0) {
    total_size += ::_pbi::WireFormatLite::Int64SizePlusOne(this->_internal_share_a());
  }

  // int64 share_b = 3;
  if (this->_internal_share_b() != 0) {
    total_size += ::_pbi::WireFormatLite::Int64SizePlusOne(this->_internal_share_b());
  }

  // int64 share_c = 4;
  if (this->_internal_share_c() != 0) {
    total_size += ::_pbi::WireFormatLite::Int64SizePlusOne(this->_internal_share_c());
  }

  // int32 player_nr = 1;
  if (this->_internal_player_nr() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_player_nr());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData TripleShare::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    TripleShare::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*TripleShare::GetClassData() const { return &_class_data_; }


void TripleShare::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<TripleShare*>(&to_msg);
  auto& from = static_cast<const TripleShare&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:TripleShare)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_associated_a()) {
    _this->_internal_mutable_associated_a()->::AssociatedTriple::MergeFrom(
        from._internal_associated_a());
  }
  if (from._internal_has_associated_b()) {
    _this->_internal_mutable_associated_b()->::AssociatedTriple::MergeFrom(
        from._internal_associated_b());
  }
  if (from._internal_has_associated_c()) {
    _this->_internal_mutable_associated_c()->::AssociatedTriple::MergeFrom(
        from._internal_associated_c());
  }
  if (from._internal_share_a() != 0) {
    _this->_internal_set_share_a(from._internal_share_a());
  }
  if (from._internal_share_b() != 0) {
    _this->_internal_set_share_b(from._internal_share_b());
  }
  if (from._internal_share_c() != 0) {
    _this->_internal_set_share_c(from._internal_share_c());
  }
  if (from._internal_player_nr() != 0) {
    _this->_internal_set_player_nr(from._internal_player_nr());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void TripleShare::CopyFrom(const TripleShare& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:TripleShare)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool TripleShare::IsInitialized() const {
  return true;
}

void TripleShare::InternalSwap(TripleShare* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(TripleShare, _impl_.player_nr_)
      + sizeof(TripleShare::_impl_.player_nr_)
      - PROTOBUF_FIELD_OFFSET(TripleShare, _impl_.associated_a_)>(
          reinterpret_cast<char*>(&_impl_.associated_a_),
          reinterpret_cast<char*>(&other->_impl_.associated_a_));
}

::PROTOBUF_NAMESPACE_ID::Metadata TripleShare::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_Input_5fOutput_5fTriples_2eproto_getter, &descriptor_table_Input_5fOutput_5fTriples_2eproto_once,
      file_level_metadata_Input_5fOutput_5fTriples_2eproto[1]);
}

// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::AssociatedTriple*
Arena::CreateMaybeMessage< ::AssociatedTriple >(Arena* arena) {
  return Arena::CreateMessageInternal< ::AssociatedTriple >(arena);
}
template<> PROTOBUF_NOINLINE ::TripleShare*
Arena::CreateMaybeMessage< ::TripleShare >(Arena* arena) {
  return Arena::CreateMessageInternal< ::TripleShare >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
