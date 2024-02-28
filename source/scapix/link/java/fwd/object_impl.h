/*
	scapix/link/java/fwd/object_impl.h

	Copyright (c) 2019-2023 Boris Rasin (boris@scapix.com)
*/

#ifndef SCAPIX_LINK_JAVA_FWD_OBJECT_IMPL_H
#define SCAPIX_LINK_JAVA_FWD_OBJECT_IMPL_H

#include <tuple>
#include <scapix/core/fixed_string.h>
#include <scapix/link/java/object_traits.h>

namespace scapix::jni {

template <fixed_string ClassName>
class object_impl;

template <fixed_string ClassName>
struct object_traits<object_impl<ClassName>>
{
	static constexpr auto class_name = ClassName;
	using base_classes = std::tuple<>;
};

} // namespace scapix::jni

#endif // SCAPIX_LINK_JAVA_FWD_OBJECT_IMPL_H
