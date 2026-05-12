#include <atomic>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <gtest/gtest.h>

namespace bip = boost::interprocess;

class SharedChat
{
    struct FluxControl
    {
        bip::interprocess_mutex mtx;

        bip::interprocess_condition cv;
    };

    using Region = bip::managed_shared_memory;

    using MemAlloc = bip::allocator<char, Region::segment_manager>;

    using Blob = bip::basic_string<char, std::char_traits<char>, MemAlloc>;

    using BlobAlloc = bip::allocator<Blob, Region::segment_manager>;

    using LogQueue = bip::deque<Blob, BlobAlloc>;

public:

    static constexpr const char* SHM_ID = "flux_bridge_v1";

    static constexpr std::size_t SHM_CAP = 1 << 20;

    SharedChat(bool is_host) : host_mode(is_host), heap(bip::open_or_create, SHM_ID, SHM_CAP)
    {
        ctrl = heap.find_or_construct<FluxControl>("ctrl")();

        stream = heap.find_or_construct<LogQueue>("data")(heap.get_segment_manager());
    }

    void broadcast(const std::string& raw)
    {
        std::scoped_lock<bip::interprocess_mutex> guard(ctrl->mtx);

        Blob entry(heap.get_segment_manager());

        entry += (host_mode ? "H:" : "P:");

        entry += raw.c_str();

        stream->push_back(entry);

        ctrl->cv.notify_all();
    }

    std::string await_next(std::size_t& cursor, std::chrono::milliseconds timeout = std::chrono::milliseconds(1000))
    {
        std::unique_lock<bip::interprocess_mutex> guard(ctrl->mtx);

        auto expire = boost::posix_time::microsec_clock::universal_time() + boost::posix_time::milliseconds(timeout.count());

        auto predicate = [&] { return cursor < stream->size(); };

        while (!predicate())
        {
            if (!ctrl->cv.timed_wait(guard, expire)) return {};
        }

        std::string payload = (*stream)[cursor++].c_str();

        return (host_mode ? payload.rfind("P:", 0) : payload.rfind("H:", 0)) == 0
               ? payload.substr(2)
               : await_next(cursor, timeout);
    }

    void run(std::istream& src, std::ostream& dst)
    {
        std::atomic<bool> active{true};

        std::size_t idx;
        {
            std::scoped_lock<bip::interprocess_mutex> guard(ctrl->mtx);
            idx = stream->size();
        }

        std::thread out_pump([&]
        {
            std::string buf;

            while (std::getline(src, buf))
            {
                if (buf == "/exit")
                {
                    break;
                }

                broadcast(buf);
            }

            active = false;

            ctrl->cv.notify_all();
        });

        std::thread in_pump([&]
        {
            while (active)
            {
                auto msg = await_next(idx, std::chrono::milliseconds(200));

                if (!msg.empty())
                {
                    dst << msg << std::endl;
                }
            }
        });

        out_pump.join();

        in_pump.join();
    }

    static void purge() { bip::shared_memory_object::remove(SHM_ID); }

private:

    bool host_mode;

    Region heap;

    FluxControl* ctrl;

    LogQueue* stream;
};

TEST(Bridge, HostToPeer)
{
    SharedChat::purge();

    SharedChat h(true), p(false);

    std::size_t pos = 0;

    h.broadcast("hello");

    EXPECT_EQ(p.await_next(pos), "hello");

    SharedChat::purge();
}

TEST(Bridge, PeerToHost)
{
    SharedChat::purge();

    SharedChat h(true), p(false);

    std::size_t pos = 0;

    p.broadcast("reply");

    EXPECT_EQ(h.await_next(pos), "reply");

    SharedChat::purge();
}

TEST(Bridge, Sequence)
{
    SharedChat::purge();

    SharedChat h(true), p(false);

    std::size_t pos = 0;

    for(auto& s : {"one", "two", "three"})
    {
        h.broadcast(s);
    }

    for(auto& s : {"one", "two", "three"})
    {
        EXPECT_EQ(p.await_next(pos), s);
    }

    SharedChat::purge();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
