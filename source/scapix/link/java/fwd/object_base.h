/*
	scapix/link/java/fwd/object_base.h

	Copyright (c) 2019-2023 Boris Rasin (boris@scapix.com)
*/

#ifndef SCAPIX_LINK_JAVA_FWD_OBJECT_BASE_H
#define SCAPIX_LINK_JAVA_FWD_OBJECT_BASE_H

#include <tuple>
#include <scapix/core/fixed_string.h>
#include <scapix/link/java/object_traits.h>

namespace scapix::jni {

template <fixed_string ClassName, typename ...Bases>
class object_base;

template <fixed_string ClassName, typename ...Bases>
struct object_traits<object_base<ClassName, Bases...>>
{
	static constexpr auto class_name = ClassName;
	using base_classes = std::tuple<Bases...>;
};

} // namespace scapix::jni

#endif // SCAPIX_LINK_JAVA_FWD_OBJECT_BASE_H
