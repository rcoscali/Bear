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

#include "collect/Reporter.h"
#include "intercept/Flags.h"

#include <spdlog/spdlog.h>

#include <utility>

namespace ic {

    rust::Result<Reporter::Ptr> Reporter::from(const flags::Arguments& flags) {
        return flags
                .as_string(OUTPUT)
                .and_then<ic::collect::db::EventsDatabaseWriter::Ptr>([](auto file) {
                    return ic::collect::db::EventsDatabaseWriter::create(file);
                })
                .map<Reporter::Ptr>([](auto events) {
                    return std::make_shared<Reporter>(events);
                });
    }

    Reporter::Reporter(ic::collect::db::EventsDatabaseWriter::Ptr database)
            : database_(std::move(database))
            , mutex_()
    { }

    void Reporter::report(const rpc::Event& event) {
        std::lock_guard<std::mutex> lock(mutex_);

        database_->insert_event(event)
                .on_error([](auto error) {
                    spdlog::warn("Writing event into database failed: {} Ignored.", error.what());
                });
    }
}
