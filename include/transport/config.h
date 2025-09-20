// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#ifndef TRANSPORT_CONFIG_H_
#define TRANSPORT_CONFIG_H_

#define TRANSPORT_VERSION_MAJOR 1
#define TRANSPORT_VERSION_MINOR 1
#define TRANSPORT_VERSION_MICRO 0
#define TRANSPORT_VERSION_STR "1.1.0"

// C++20 support defines
#ifndef HAVE_CXX20
#define HAVE_CXX20 0
#endif /* ifndef HAVE_CXX20 */

// C++17 support defines
#ifndef HAVE_CXX17
#define HAVE_CXX17 1
#endif /* ifndef HAVE_CXX17 */

// C++14 support defines
#ifndef HAVE_CXX14
#define HAVE_CXX14 1
#endif /* ifndef HAVE_CXX14 */

// C++1Y support defines
#ifndef HAVE_CXX1Y
#define HAVE_CXX1Y 1
#endif /* ifndef HAVE_CXX1Y */

// C++11 support defines
#ifndef HAVE_CXX11
#define HAVE_CXX11 1
#endif /* ifndef HAVE_CXX11 */

// Endianness defines
#ifndef IS_BIG_ENDIAN
#define IS_BIG_ENDIAN 0
#endif /* ifndef IS_BIG_ENDIAN */

// Security
#ifndef HAVE_SECURITY
#define HAVE_SECURITY 0
#endif /* ifndef HAVE_SECURITY */

// TLS support
#ifndef TLS_FOUND
#define TLS_FOUND 0
#endif /* ifndef TLS_FOUND */

#endif // TRANSPORT_CONFIG_H_
