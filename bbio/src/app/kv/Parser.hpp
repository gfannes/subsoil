#ifndef HEADER_app_kv_Parser_hpp_ALREADY_INCLUDED
#define HEADER_app_kv_Parser_hpp_ALREADY_INCLUDED

#include <app/kv/KeyValue.hpp>
#include <gubg/mss.hpp>
#include <functional>
#include <map>

namespace app { namespace kv { 

    class Parser
    {
    public:
        template <typename Ftor>
        bool on(const std::string &key, Ftor &&ftor)
        {
            MSS_BEGIN(bool);
            auto it = key__ftor_.find(key);
            MSS(it == key__ftor_.end(), std::cout << "Error: handler for \"" << key << "\"" << " already present" << std::endl);
            key__ftor_[key] = ftor;
            MSS_END();
        }
        template <typename Ftor>
        bool on_other(Ftor &&ftor)
        {
            MSS_BEGIN(bool);
            MSS(!other_, std::cout << "Error: handler for _other_ already present" << std::endl);
            other_ = ftor;
            MSS_END();
        }

        bool operator()(const kv::KeyValues &kvs)
        {
            MSS_BEGIN(bool);

            for (const auto &kv: kvs)
            {
                auto it = key__ftor_.find(kv.key);
                if (it != key__ftor_.end())
                {
                    MSS(it->second(kv.value),   std::cout << "Error: could not handle value for " << kv << std::endl);
                }
                else
                {
                    MSS(!!other_, std::cout << "Error: no handler found for " << kv << std::endl);
                    MSS(other_(kv.key, kv.value));
                }
            }

            MSS_END();
        }

    private:
        using HandleValue = std::function<bool(const std::string &)>;
        using HandleKeyValue = std::function<bool(const std::string &, const std::string &)>;

        std::map<std::string, HandleValue> key__ftor_;
        HandleKeyValue other_;
    };

} } 

#endif
