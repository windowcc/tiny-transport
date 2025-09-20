#include <cstring>
#include <cstdint>
#include <thread>

#include <Handle.h>
#include "test.h"

using namespace ipc::detail;

namespace {

TEST(SHM, acquire) {
    Handle shm_hd;
    EXPECT_FALSE(shm_hd.valid());

    EXPECT_TRUE(shm_hd.acquire("my-test-1", 1024));
    EXPECT_TRUE(shm_hd.valid());
    EXPECT_STREQ(shm_hd.name(), "my-test-1");

    EXPECT_TRUE(shm_hd.acquire("my-test-2", 2048));
    EXPECT_TRUE(shm_hd.valid());
    EXPECT_STREQ(shm_hd.name(), "my-test-2");

    EXPECT_TRUE(shm_hd.acquire("my-test-3", 4096));
    EXPECT_TRUE(shm_hd.valid());
    EXPECT_STREQ(shm_hd.name(), "my-test-3");
}

TEST(SHM, release) {
    Handle shm_hd;
    EXPECT_FALSE(shm_hd.valid());
    shm_hd.release();
    EXPECT_FALSE(shm_hd.valid());
    EXPECT_TRUE(shm_hd.acquire("release-test-1", 512));
    EXPECT_TRUE(shm_hd.valid());
    shm_hd.release();
    EXPECT_FALSE(shm_hd.valid());
}

TEST(SHM, get) {
    Handle shm_hd;
    EXPECT_TRUE(shm_hd.get() == nullptr);
    EXPECT_TRUE(shm_hd.acquire("get-test", 2048));

    auto mem = shm_hd.get();
    EXPECT_TRUE(mem != nullptr);
    EXPECT_TRUE(mem == shm_hd.get());

    std::uint8_t buf[1024] = {};
    EXPECT_TRUE(memcmp(mem, buf, sizeof(buf)) == 0);

    Handle shm_other(shm_hd.name(), shm_hd.size());
    EXPECT_TRUE(shm_other.get() != shm_hd.get());
}

TEST(SHM, hello) {
    Handle shm_hd;
    EXPECT_TRUE(shm_hd.acquire("hello-test", 128));
    auto mem = shm_hd.get();
    EXPECT_TRUE(mem != nullptr);

    constexpr char hello[] = "hello!";
    std::memcpy(mem, hello, sizeof(hello));
    EXPECT_STREQ((char const *)shm_hd.get(), hello);

    shm_hd.release();
    EXPECT_TRUE(shm_hd.get() == nullptr);
    EXPECT_TRUE(shm_hd.acquire("hello-test", 1024));

    mem = shm_hd.get();
    EXPECT_TRUE(mem != nullptr);
    std::uint8_t buf[1024] = {};
    EXPECT_TRUE(memcmp(mem, buf, sizeof(buf)) == 0);

    std::memcpy(mem, hello, sizeof(hello));
    EXPECT_STREQ((char const *)shm_hd.get(), hello);
}

TEST(SHM, mt) {
    Handle shm_hd;
    EXPECT_TRUE(shm_hd.acquire("mt-test", 256));
    constexpr char hello[] = "hello!";
    std::memcpy(shm_hd.get(), hello, sizeof(hello));

    std::thread {
        [&shm_hd] {
            Handle shm_mt(shm_hd.name(), shm_hd.size());
            shm_hd.release();
            constexpr char hello[] = "hello!";
            EXPECT_STREQ((char const *)shm_mt.get(), hello);
        }
    }.join();

    EXPECT_TRUE(shm_hd.get() == nullptr);
    EXPECT_FALSE(shm_hd.valid());

    EXPECT_TRUE(shm_hd.acquire("mt-test", 1024));
    std::uint8_t buf[1024] = {};
    EXPECT_TRUE(memcmp(shm_hd.get(), buf, sizeof(buf)) == 0);
}

} // internal-linkage
