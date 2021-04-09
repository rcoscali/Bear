/*  Copyright (C) 2012-2021 by László Nagy
    This file is part of Bear.

    Bear is a tool to generate compilation database for clang tooling.

    Bear is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bear is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "report/wrapper/EventFactory.h"
#include "Convert.h"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <random>
#include <cstdint>
#include <string>

namespace {

    std::uint64_t generate_unique_id() {
        std::random_device random_device;
        std::mt19937_64 generator(random_device());
        std::uniform_int_distribution<std::uint64_t> distribution;

        return distribution(generator);
    }

    std::string now_as_string() {
        const auto now = std::chrono::system_clock::now();
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());

        return fmt::format("{:%Y-%m-%dT%H:%M:%S}.{:06d}Z",
                           fmt::localtime(std::chrono::system_clock::to_time_t(now)),
                           micros.count() % 1000000);
    }
}

namespace wr {

    EventFactory::EventFactory() noexcept
            : rid_(generate_unique_id())
    { }

    rpc::Event EventFactory::start(ProcessId pid, ProcessId ppid, const Execution &execution) const {
        rpc::Event event;
        event.set_rid(rid_);
        event.set_timestamp(now_as_string());
        {
            rpc::Event_Started &event_started = *event.mutable_started();
            event_started.set_pid(pid);
            event_started.set_ppid(ppid);
            *event_started.mutable_execution() = into(execution);
        }
        return event;
    }

    rpc::Event EventFactory::signal(int number) const {
        rpc::Event result;
        result.set_rid(rid_);
        result.set_timestamp(now_as_string());
        {
            rpc::Event_Signalled &event = *result.mutable_signalled();
            event.set_number(number);
        }
        return result;
    }

    rpc::Event EventFactory::terminate(int code) const {
        rpc::Event result;
        result.set_rid(rid_);
        result.set_timestamp(now_as_string());
        {
            rpc::Event_Terminated &event = *result.mutable_terminated();
            event.set_status(code);
        }
        return result;
    }
}
