# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: customer_tree.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='customer_tree.proto',
  package='customer_tree',
  syntax='proto3',
  serialized_options=None,
  serialized_pb=_b('\n\x13\x63ustomer_tree.proto\x12\rcustomer_tree\"<\n\x0c\x43ustomerInfo\x12\x0b\n\x03\x65id\x18\x01 \x01(\x05\x12\x0b\n\x03pid\x18\x02 \x01(\x05\x12\x12\n\nlogin_name\x18\x03 \x01(\t2V\n\x05\x43Tree\x12M\n\x11getCustomInfoByID\x12\x1b.customer_tree.CustomerInfo\x1a\x1b.customer_tree.CustomerInfob\x06proto3')
)




_CUSTOMERINFO = _descriptor.Descriptor(
  name='CustomerInfo',
  full_name='customer_tree.CustomerInfo',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='eid', full_name='customer_tree.CustomerInfo.eid', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='pid', full_name='customer_tree.CustomerInfo.pid', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='login_name', full_name='customer_tree.CustomerInfo.login_name', index=2,
      number=3, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=38,
  serialized_end=98,
)

DESCRIPTOR.message_types_by_name['CustomerInfo'] = _CUSTOMERINFO
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

CustomerInfo = _reflection.GeneratedProtocolMessageType('CustomerInfo', (_message.Message,), {
  'DESCRIPTOR' : _CUSTOMERINFO,
  '__module__' : 'customer_tree_pb2'
  # @@protoc_insertion_point(class_scope:customer_tree.CustomerInfo)
  })
_sym_db.RegisterMessage(CustomerInfo)



_CTREE = _descriptor.ServiceDescriptor(
  name='CTree',
  full_name='customer_tree.CTree',
  file=DESCRIPTOR,
  index=0,
  serialized_options=None,
  serialized_start=100,
  serialized_end=186,
  methods=[
  _descriptor.MethodDescriptor(
    name='getCustomInfoByID',
    full_name='customer_tree.CTree.getCustomInfoByID',
    index=0,
    containing_service=None,
    input_type=_CUSTOMERINFO,
    output_type=_CUSTOMERINFO,
    serialized_options=None,
  ),
])
_sym_db.RegisterServiceDescriptor(_CTREE)

DESCRIPTOR.services_by_name['CTree'] = _CTREE

# @@protoc_insertion_point(module_scope)
