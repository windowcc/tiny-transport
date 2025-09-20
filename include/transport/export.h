// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file Export.hpp
 *
 */

#if defined(Q_DECL_EXPORT) && defined(Q_DECL_IMPORT)

#define DDS_DECL_EXPORT Q_DECL_EXPORT
#define DDS_DECL_IMPORT Q_DECL_IMPORT

#else // defined(Q_DECL_EXPORT) && defined(Q_DECL_IMPORT)

/*
 * Compiler & system detection for DDS_DECL_EXPORT & DDS_DECL_IMPORT.
 * Not using QtCore cause it shouldn't depend on Qt.
 */

#if defined(_MSC_VER)
#define DDS_DECL_EXPORT __declspec(dllexport)
#define DDS_DECL_IMPORT __declspec(dllimport)
#elif defined(__ARMCC__) || defined(__CC_ARM)
#if defined(ANDROID) || defined(__linux__) || defined(__linux)
#define DDS_DECL_EXPORT __attribute__((visibility("default")))
#define DDS_DECL_IMPORT __attribute__((visibility("default")))
#else
#define DDS_DECL_EXPORT __declspec(dllexport)
#define DDS_DECL_IMPORT __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || \
    defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#define DDS_DECL_EXPORT __declspec(dllexport)
#define DDS_DECL_IMPORT __declspec(dllimport)
#else
#define DDS_DECL_EXPORT __attribute__((visibility("default")))
#define DDS_DECL_IMPORT __attribute__((visibility("default")))
#endif
#else
#define DDS_DECL_EXPORT __attribute__((visibility("default")))
#define DDS_DECL_IMPORT __attribute__((visibility("default")))
#endif

#endif // defined(Q_DECL_EXPORT) && defined(Q_DECL_IMPORT)

/*
 * Define DllExport for exporting function & class.
 */

#ifndef DllExport
#if defined(LIBTRANSPORT_LIBRARY_SHARED_BUILDING__)
#define DllExport DDS_DECL_EXPORT
#elif defined(LIBTRANSPORT_LIBRARY_SHARED_USING__)
#define DllExport DDS_DECL_IMPORT
#else
#define DllExport
#endif
#endif /*DllExport*/