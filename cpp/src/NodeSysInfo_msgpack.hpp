
#ifndef MODULES_NODESYSINFOMANAGER_NODESYSINFO_msgpack_HPP_
#define MODULES_NODESYSINFOMANAGER_NODESYSINFO_msgpack_HPP_

#include "NodeSysInfo.hpp"
#include <msgpack/adaptor/define.hpp>


template<>
struct NodeSysInfo::SerializationHelper<msgpack::v1::object> {
    template<typename TNodeSysInfo>
    static auto &versions(TNodeSysInfo &_self) {
        return _self.swversion;
    }

    template<typename TNodeSysInfo>
    static auto &additionals(TNodeSysInfo &_self) {
        return _self.m_addInfos;
    }
};

namespace msgpack {
    inline namespace v1 /* MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)*/ {
        namespace adaptor {
            //https://github.com/msgpack/msgpack-c/wiki/v2_0_cpp_adaptor

            // convert          Converting from msgpack::object to T.
            // pack             Packing from T into msgpack::packer.
            // object           Set msgpack::object by T.
            // object_with_zone Set msgpack::object::with_zone by T.
            // When you use msgpack::object::as member function template in convert class template specilization, , temporary objects are created.


            template<>
            struct convert<NodeSysInfo> {
                msgpack::v1::object const &operator()(msgpack::v1::object const &o, NodeSysInfo &v) const {
                    //TODO: if o is empty -> then ok ... empty v.
                    msgpack::type::make_define_array(
                        NodeSysInfo::SerializationHelper<msgpack::v1::object>::versions(v).elems,
                        NodeSysInfo::SerializationHelper<msgpack::v1::object>::additionals(v)).msgpack_unpack(o);
                    return o;
                }
            };

            template<>
            struct pack<NodeSysInfo> {
                template<typename Stream>
                packer<Stream> &operator()(msgpack::packer<Stream> &o, NodeSysInfo const &v) const {
                    //TODO: if swversion is empty and m_addInfos is empty -> then create empty array ? or NULL
                    msgpack::type::make_define_array(
                        NodeSysInfo::SerializationHelper<msgpack::v1::object>::versions(v).elems,
                        NodeSysInfo::SerializationHelper<msgpack::v1::object>::additionals(v)).msgpack_pack(o);
                    return o;
                }
            };

            template<>
            struct object_with_zone<NodeSysInfo> {
                void operator()(msgpack::object::with_zone &o, NodeSysInfo const &v) const {
                    msgpack::type::make_define_array(
                        NodeSysInfo::SerializationHelper<msgpack::v1::object>::versions(v).elems,
                        NodeSysInfo::SerializationHelper<msgpack::v1::object>::additionals(v)).msgpack_object(
                        &o, o.zone);
                }
            };

            //MSGPACK_DEFINE(a);

            template<>
            struct convert<CNodeSystemVersion> {
                msgpack::v1::object const &operator()(msgpack::v1::object const &o, CNodeSystemVersion &v) const {
                    //msgpack::type::make_define_array(  v.ProjectName() , v.Subsystem(),  static_cast<CNodeVersion&>(v) ).msgpack_unpack( o);
                    if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
                    if (o.via.array.size == 3) {
                        v = CNodeSystemVersion(
                            o.via.array.ptr[0].as<std::string>(),
                            o.via.array.ptr[1].as<std::string>(),
                            o.via.array.ptr[2].as<CNodeVersion>());
                    } else if (o.via.array.size == 0) {
                        v = CNodeSystemVersion();
                    } else throw msgpack::type_error();

                    return o;
                }
            };

            template<>
            struct object_with_zone<CNodeSystemVersion> {
                void operator()(msgpack::object::with_zone &o, CNodeSystemVersion const &v) const {
                    if (!v.ProjectName().empty() || !v.Subsystem().empty() || v.MainVersion() || v.SubVersion() || v.
                        Revision() || v.Valid())
                        msgpack::type::make_define_array(v.ProjectName(), v.Subsystem(),
                                                         static_cast<const CNodeVersion &>(v)).msgpack_object(
                            &o, o.zone);
                    else {
                        o.type = type::ARRAY;
                        o.via.array.size = 0;
                    }
                }
            };

            template<>
            struct convert<CNodeVersion> {
                msgpack::v1::object const &operator()(msgpack::v1::object const &o, CNodeVersion &v) const {
                    //msgpack::type::make_define_array(  v.MainVersion() , v.SubVersion(),  v.Revision(), v.Valid() ).msgpack_unpack( o);
                    if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
                    if (o.via.array.size == 4) {
                        v = CNodeVersion(
                            o.via.array.ptr[0].as<CNodeVersion::VersionNumberType>(),
                            o.via.array.ptr[1].as<CNodeVersion::VersionNumberType>(),
                            o.via.array.ptr[2].as<CNodeVersion::VersionNumberType>(),
                            o.via.array.ptr[3].as<bool>());
                    } else if (o.via.array.size == 3) {
                        v = CNodeVersion(
                            o.via.array.ptr[0].as<CNodeVersion::VersionNumberType>(),
                            o.via.array.ptr[1].as<CNodeVersion::VersionNumberType>(),
                            o.via.array.ptr[2].as<CNodeVersion::VersionNumberType>());
                    } else if (o.via.array.size == 0) {
                        v = CNodeVersion();
                    } else throw msgpack::type_error();
                    return o;
                }
            };

            template<>
            struct object_with_zone<CNodeVersion> {
                void operator()(msgpack::object::with_zone &o, CNodeVersion const &v) const {
                    if (v.MainVersion() || v.SubVersion() || v.Revision() || v.Valid())
                        msgpack::type::make_define_array(v.MainVersion(), v.SubVersion(), v.Revision(), v.Valid()).
                                msgpack_object(&o, o.zone);
                    else {
                        o.type = type::ARRAY;
                        o.via.array.size = 0;
                    }
                }
            };
        } // namespace adaptor
    } // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack


#endif
