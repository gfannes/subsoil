#ifndef HEADER_laurot_App_hpp_ALREADY_INCLUDED
#define HEADER_laurot_App_hpp_ALREADY_INCLUDED

#include "laurot/Types.hpp"
#include "laurot/Options.hpp"
#include "laurot/Master.hpp"
#include "laurot/Slave.hpp"
#include "gubg/rs485/Bus.hpp"
#include "gubg/mss.hpp"
#include <list>

namespace laurot { 

    class App
    {
    public:
        App(): master_(bus_.create_endpoint()) {}

        bool process(int argc, const char **argv)
        {
            MSS_BEGIN(bool);
            MSS(options_.parse(argc, argv));
            MSS_END();
        }

        bool run()
        {
            MSS_BEGIN(bool, "");

            for (Id id = 0u; id < options_.nr_slaves; ++id)
                add_slave_(id);

            for (auto i = 0u; i < options_.nr_iterations; ++i)
            {
                L(C(i));
                bus_.process();
                MSS(master_.process());
                for (auto &slave: slaves_)
                {
                    MSS(slave.process());
                }
            }

            MSS_END();
        }

    private:
        void add_slave_(Id id)
        {
            slaves_.emplace_back(id, bus_.create_endpoint());
        }

        Options options_;

        gubg::rs485::Bus bus_;
        Master master_;
        using Slaves = std::list<Slave>;
        Slaves slaves_;
    };

} 

#endif
